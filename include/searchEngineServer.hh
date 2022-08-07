#ifndef reactorServer_hh_
#define reactorServer_hh_

#include "tcpServer.hh"
#include "threadPoolBO.hh"
#include"../include/keyRecommander.hh"
#include "../include/webPageResearcher.hh"

class MyTask{
public:
    MyTask(const string& msg, 
        const TcpTransmitPtr& ptrans,
        WebPageResearcher& web,
        KeyRecommander& key);
    ~MyTask();
    void process();
private:
    WebPageResearcher& _web;
    KeyRecommander& _key;
    string _msg;
    TcpTransmitPtr _ptrans;
};

class SearchEngineServer{
public:
    SearchEngineServer( Configuration& conf,
                        splitToolCppJieBa& jieba,
                        SimHash& sim);
    ~SearchEngineServer();
    void start();
    void stop();
    void onConnection(const TcpTransmitPtr& ptrans);
    void onMessage(const TcpTransmitPtr& ptrans, WebPageResearcher& web, KeyRecommander& key);
    void onClose(const TcpTransmitPtr& ptrans);
private:
    Configuration& _conf;
    TcpServer _server;
    ThreadPool _pool;
    splitToolCppJieBa& _jieba;
    SimHash& _sim;
};

#endif 