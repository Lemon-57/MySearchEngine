#include "../include/pageLib.hh"
#include "../include/configuration.hh"
#include "../include/dirScanner.hh"
#include "../include/fileProcessor.hh"
#include "../include/myLog.hh"
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <memory>

using std::vector;
using std::ofstream;
using std::string;
using std::map;
using std::make_pair;
using std::endl;
using std::shared_ptr;

PageLib::PageLib(Configuration& conf)
:_conf(conf){
}

PageLib::~PageLib(){ }

void PageLib::createWebInit(){
    create();
    store();
}

void PageLib::create(){
    DirScanner _dirScanner;
    FileProcessor _fileProcessor(1);
    auto map =_conf.getConfigMap();
    string dirname = map["xml_web"];//获得网页文件目录名
    _dirScanner(dirname);//调用Scanner扫描目录内的所有文件
    for(auto it:_dirScanner.files()){
        _pages.push_back(_fileProcessor.process(it));
    }

    LogInfo((char*)"parse xml files success.");
}


void PageLib::store(){
    string filename="ripepage";
    ofstream out(_conf.getConfigMap()[filename]);//通过配置文件打开网页库文件
    int base=0;
    int len=0;
    int i = 1;
    int count =0;
    for(auto& it1: _pages){//遍历网页暂存vector
        if(out.is_open()){
            for(auto& it2: it1){
                out << it2.c_str() << endl;//将网页写入文件中
                len=it2.size();//当前网页长度
                _offsetLib.insert(make_pair(i++,make_pair(base,len)));//将当前网页在库中的位置信息写入网页偏移map
                base = out.tellp();//更新文件写入的基准位置
                count++;
            }
        }
    }
    out.close();//关闭网页库文件

    LogInfo((char*)"create ripepage.dat(redundant) success.");

    ofstream offset(_conf.getConfigMap()["offset_table"]);//通过配置文件打开网页偏移库文件
    for(auto it:_offsetLib){//遍历map
        if(offset.is_open()){
            offset<<it.first<<" "<<it.second.first<<" "<<it.second.second<<endl;//格式化写入文件，三个值以空格间隔，每行存储一个网页的信息
        }
    }
    offset.close();//关闭网页偏移库文件

    LogInfo((char*)"create offset_table.dat(redundant) success.");
}

