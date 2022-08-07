#include "../include/pageLibPreProcessor.hh"
#include "../include/configuration.hh"
#include "../include/webPage.hh"
#include "../include/simhash.hh"
#include "../include/myLog.hh"
#include "../include/pageLib.hh"
#include <fstream>
#include <math.h>
#include <algorithm>

//this define can avoid some logs which you don't need to care about.
#define LOGGER_LEVEL LL_WARN 

using std::ifstream;
using std::ofstream;
using std::make_pair;
using std::endl;

using namespace simhash;

PageLibPreProcessor::PageLibPreProcessor(Configuration& conf, 
                                         splitToolCppJieBa& jieba,
                                         SimHash& simhash)
: _conf(conf), _jieba(jieba), _sim(simhash){
    PageLib pb(_conf);
    pb.createWebInit();
    readInfoFromFile();
}

void PageLibPreProcessor::doProcess(){
    cutRedundantPages();
    buildInvetIndex();
    store();
}

void PageLibPreProcessor::readInfoFromFile(){
    auto map = _conf.getConfigMap();
    string ripePath = map["ripepage"];
    string offPath = map["offset_table"];

    //读取ripepage.dat offset_table.dat
    ifstream f_ripe(ripePath);
    ifstream f_off(offPath);
    
    string doc;//一篇文章
    while(getline(f_ripe, doc)){
        WebPage page(doc, _sim.getSim(doc), _conf, _jieba);
        _pageLib.emplace_back(page);
    }
}



void PageLibPreProcessor::cutRedundantPages(){

    set<WebPage> afterCutpage; //存去重后的

    for(auto it = _pageLib.begin(); it != _pageLib.end(); ++it){
         afterCutpage.insert(*it);
     }

     _pageLib.clear();
     _pageLib.shrink_to_fit();
    
    //去重后重新存入
     for(auto& it : afterCutpage){
         _pageLib.push_back(it);
     }
}

void PageLibPreProcessor::buildInvetIndex(){
    map<string, vector<pair<int, int>>> words;//string单词，pair<docId,词频>
    //vector<double> winit(_pageLib.back().getDocId() + 1, 0);//保存初始w
    vector<double> poww2(_pageLib.back().getDocId() + 1, 0);//double存每篇doc权重平方和,下标对应docId

    //保存所有词频与对应docId
    map<string, int> tmp;
    int i = 0;
    for(auto& it : _pageLib){
        tmp = it.getWordsMap();
        for(auto& it2: tmp){
            words[it2.first].push_back(make_pair(it.getDocId(), it2.second));
            i++;
        }  
    }
    vector<pair<string, double>> winit;//保存初始w
    
    //TFIDF算法
    int TF = 0;
    size_t DF = 0;
    double IDF = 0;
    double w = 0;
    for(auto& it : words){
        DF = it.second.size();
        if(DF == _pageLib.size() || DF == _pageLib.size() - 1){
            //防止IDF为0
            --DF;
        }
        IDF = log(_pageLib.size()/(DF+1))/log(2);
        for(auto& it2: it.second){
            TF = it2.second;
            w = TF * IDF;//计算未归一化w
            winit.push_back(make_pair(it.first, w));//存w
            poww2[it2.first] += pow(w, 2);
        }   
    }
    //归一化处理
    double normal;
    i = 0;
    for(auto& it : words){
        for(auto& it2 : it.second){
            normal = sqrt(poww2[it2.first]);
            _invertIndexLib[it.first].push_back(make_pair(it2.first, 
                                                        winit[i++].second/normal));
        } 
    }
}

void PageLibPreProcessor::store(){
    //保存去重后的网页库和网页偏移库
    ofstream pageLib(string(_conf.getConfigMap()["ripepage"]));
    int base=0;
    int len=0;
    for(auto& it : _pageLib){
        pageLib << it.getDoc() << endl;
        len=it.getDoc().size();//当前网页长度
        _offsetLib.insert(make_pair(it.getDocId(),make_pair(base,len)));//将当前网页在库中的位置信息写入网页偏移map
        base = pageLib.tellp();//更新文件写入的基准位置
    }

    LogInfo((char*)"create ripepage.dat(cut-redundant) success.");

    ofstream offset(_conf.getConfigMap()["offset_table"]);//通过配置文件打开网页偏移库文件
    for(auto& it:_offsetLib){//遍历map
        if(offset.is_open()){
            offset<<it.first<<" "<<it.second.first<<" "<<it.second.second<<endl;//格式化写入文件，三个值以空格间隔，每行存储一个网页的信息
        }
    }

    LogInfo((char*)"create offset_table.dat(cut-redundant) success.");

    //保存倒排索引
    ofstream invertIndexLib(string(_conf.getConfigMap()["invert_index_table"]));
    for(auto& it:_invertIndexLib){
        for(auto is:it.second){
            invertIndexLib<<it.first<<" "<<is.first<<" "<<is.second<<endl;//vector中的每个docid与权重均以空格间隔，每行对应一个vector元素
        }
    }

    LogInfo((char*)"create invert_index_table.dat success.");
}




