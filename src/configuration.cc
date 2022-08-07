#include "../include/configuration.hh"
#include "../include/myLog.hh"
#include <iostream>

using std::cout;
using std::endl;

map<string,string> Configuration::getConfigMap(){
    return _configMap;
}

set<string> Configuration::getStopWordList(){
    return _stopWordList;
}


Configuration::Configuration(const string& filePath)
: _filePath(filePath){ 
    //读取配置文件
    string key;//配置信息名
    string value;//配置信息内容
   
    ifstream conf(_filePath.c_str()); 

    while(conf >> key && getline(conf, value)){
        if(key[0] == '#'){
            //跳过注释行
            continue;
        }else{
            //存入_configMap,跳过前导空格
            _configMap[key] = value.substr(1);
        }
    }

    LogInfo((char*)"load myConf.conf success.");

    //读取停用词文件
    //中文停用词
    ifstream cnFile(_configMap.find("cn_stop_words")->second);
    readStopWord(cnFile);
    //英文停用词
    ifstream enFile(_configMap.find("en_stop_words")->second);
    readStopWord(enFile);

    LogInfo((char*)"load stop_words.txt success.");
}

void Configuration::readStopWord(ifstream &file){
    //读取停用词文件
    string stop_words;
    while(getline(file, stop_words)){
        _stopWordList.insert(stop_words);
    }
}

Configuration::~Configuration(){ } 
