#ifndef _DICTIONARY_H
#define _DICTIONARY_H
#include "../include/configuration.hh"

#include<memory>
#include<sstream>
#include <string>
#include<vector>
#include<map>
#include<set>

using std::shared_ptr;
using std::istringstream;
using std::vector;
using std::string;
using std::pair;
using std::set;
using std::map;

class Dictionary {
public: 
    //static Dictionary* getInstance();

    vector<pair<string, int>>& getDict();

    map<string, set<int>>& getIndex();

    //Dictionary(const Dictionary&) = delete;//拷贝构造、赋值运算删掉

    //Dictionary& operator=(const Dictionary&) = delete;

    Dictionary(Configuration& conf);

    ~Dictionary(){}

private: 
    //class AutoRelease{//程序结束,AR对象释放时调用析构，自动delete堆区的单例对象
    //public:
    //    AutoRelease(){}//_pdictAR是静态成员不需要在类内初始化

    //    ~AutoRelease(){
    //        if(_pdict){
    //            delete _pdict;
    //            _pdict = nullptr;//释放后置空指针
    //        }
    //    }
    //};

    vector<pair<string, int>> _dict;
    map<string, set<int>> _index;
    Configuration& _conf;
    //static Dictionary* _pdict;
    //static AutoRelease _ar;
};

#endif //_DICTIONARY_H
