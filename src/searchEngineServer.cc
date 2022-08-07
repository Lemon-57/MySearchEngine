#include "../include/searchEngineServer.hh"
#include "../include/dictProducer.hh"
#include "../include/pageLib.hh"
#include "../include/fileProcessor.hh"
#include "../include/dirScanner.hh"
#include "../include/protocolParser.hh"
#include "../include/myLog.hh"
#include <iostream>
#include <unistd.h>
#include <fstream>

using std::cout;
using std::endl;

MyTask::MyTask(const string& msg, 
                const TcpTransmitPtr& ptrans,
                WebPageResearcher& web,
                KeyRecommander& key)
: _msg(msg), _ptrans(ptrans), _web(web), _key(key){ }

MyTask::~MyTask(){ }

void MyTask::process(){

    ProtocolParser pp(_msg, _key, _web);
    string sendMsg = pp.doParse();
    _ptrans->sendInLoop(sendMsg);

}


SearchEngineServer::SearchEngineServer(Configuration& conf,
                                        splitToolCppJieBa& jieba,
                                        SimHash& sim)
: _conf(conf)
, _jieba(jieba)
, _sim(sim)
,_server(_conf.getConfigMap()["serverIP"], (unsigned short)atoi(_conf.getConfigMap()["serverPort"].c_str()))
,_pool(atoi(_conf.getConfigMap()["threadNum"].c_str()),atoi(_conf.getConfigMap()["queSize"].c_str()))
{ 
  
}

SearchEngineServer::~SearchEngineServer(){ }

void SearchEngineServer::start(){
    _pool.start();

    //生成离线文件
     std::ifstream inFile("../data/ripepage.dat", std::ios_base::in);
     if(!inFile.is_open())
     {
            LogInfo((char*)"offline part not ready, re-create.");
            DictProducer d(_conf, _jieba);
            PageLibPreProcessor pr(_conf, _jieba, _sim);
            _pool.addTask(std::bind(&DictProducer::createKey, d));
            _pool.addTask(std::bind(&PageLibPreProcessor::doProcess, pr));
     }

    //准备在线查询
    while(1){
        std::ifstream inFile("../data/dict.dat", std::ios_base::in);
        if(inFile.is_open()){   
            inFile.seekg(0, inFile.end);
            if(inFile.tellg() ==503370){
                break;
            }
        }
    }
    
    LogInfo((char*)"online part start.");
    
    Dictionary dic(_conf);
    RedisCache redis(_conf.getConfigMap()["redisAddress"]);
    webPageQuery wq(_jieba, _conf);
    WebPageResearcher web(_jieba, wq ,redis);
    KeyRecommander key(dic, redis);
     
    using namespace std::placeholders;
    
    _server.setAllCallBack(std::bind(&SearchEngineServer::onConnection, this, _1),
                            std::bind(&SearchEngineServer::onMessage, this,  _1, web, key),
                            std::bind(&SearchEngineServer::onClose, this, _1));
    _server.start();
}

void SearchEngineServer::stop(){
    _pool.stop();
    _server.stop();
}

void SearchEngineServer::onConnection(const TcpTransmitPtr& ptrans){
    LogInfo((char*)"%s has connected !", ptrans->toStr());
    //cout << ptrans->toStr() << "has connected !" << endl;
}

void SearchEngineServer::onMessage(const TcpTransmitPtr& ptrans, WebPageResearcher& web, KeyRecommander& key){
    //获取消息长度MsgLen
    char msgLenBuf[4] = {0};
    string msgLenString = ptrans->recvn(4);
    strcpy(msgLenBuf, msgLenString.data());
    int msgLen = *(int*)msgLenBuf;

    //获取消息主体rawMsg,此时还是0 1开头+JSON字符串的消息
    string rawMsg = ptrans->recvn(msgLen);
    //cout << "recv from client msg : " << rawMsg << endl;
    
    //用消息构造task，处理交给线程池

    LogInfo((char*)"receive client msg");

    MyTask task(rawMsg, ptrans, web, key);
    _pool.addTask(std::bind(&MyTask::process, task));
}

void SearchEngineServer::onClose(const TcpTransmitPtr& ptrans){
    LogInfo((char*)"%s has closed !", ptrans->toStr());
   //cout << ptrans->toStr() << "has closed !" << endl;
}