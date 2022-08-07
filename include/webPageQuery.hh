#pragma once

#include "webPage.hh"
#include "pageLibPreProcessor.hh"
#include "../include/nlohmann/json.hpp"
#include <set>
#include <string>
#include <map>
#include <unordered_map>
#include <utility>
#include <functional>

using json = nlohmann::json; 

using std::map;
using std::set;
using std::string;
using std::unordered_map;
using std::pair;
using std::multimap;

class webPageQuery
{
public:
   webPageQuery(splitToolCppJieBa& jieba, Configuration& _conf);
    ~webPageQuery();
    string doQuery(const string & str); //执行查询，返回结果 
    void loadLibrary(); //加载库文件 
    vector<double> getQueryWordsWeightVector(const vector<string> & queryWords);//计算查询词的权重值 
    bool executeQuery( const vector<string> & queryWords); //执行查询 
    string createJson(); 
    string returnNoAnswer();
private:
    splitToolCppJieBa& _jieba; //Jieba 分词库对象
    Configuration& _conf;

    vector<string> _queryWords;
    string _pageLibPath;
    unordered_map<int, pair<int, int> > _offsetLib; //偏移库
    //unordered_map 的 string 为出现在文档中的某个词语，
    //对应的 value 为包含该 词语的文档 ID 的集合以及该词语的权重值 w。
    // word <id, weight>
    unordered_map<string, unordered_map<int, double>>_invertIndexTable;
    multimap<double, int> _docCosine; //余弦值 和 文章id  
};


