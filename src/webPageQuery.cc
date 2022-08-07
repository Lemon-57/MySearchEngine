#include "../include/webPageQuery.hh"
#include "../include/simhash.hh"
#include "../include/configuration.hh"
#include "../include/splitToolCppJieBa.hh"
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <cstring>
#include <cstdio>
#include <math.h>
#include <algorithm>    
#include <iterator>    
#include <regex>

using std::ifstream;
using std::ofstream;
using std::make_pair;
using std::unordered_set;
using std::set_intersection;
using std::insert_iterator;

webPageQuery::webPageQuery(splitToolCppJieBa& jieba, Configuration& conf)
:_jieba(jieba)
,_conf(conf)
{
    loadLibrary();
}

webPageQuery::~webPageQuery()
{

}

string webPageQuery::doQuery(const string & str) //执行查询，返回结果
{
    vector<string>queryWords = _jieba.cut(str);
    if(executeQuery(queryWords))
    {

        return createJson();
    }
    else
    {
        return returnNoAnswer();
    }

} 

void webPageQuery::loadLibrary() //加载库文件
{
    auto map = _conf.getConfigMap();
    _pageLibPath = map["ripepage"]; //网页库
    string offsetLibPath = map["offset_table"]; //网页偏移表
    string invertIndexLibPath = map["invert_index_table"]; //倒排索引表

    //读取ripepage.dat offset_table.dat
    ifstream f_off(offsetLibPath);
    ifstream f_invert(invertIndexLibPath);
   
    int docId = 0;
    int offset = 0;
    int length = 0;
    
    while(f_off >> docId >> offset >> length)
    {
        _offsetLib.insert(make_pair(docId, make_pair(offset, length)));//在offsetLib中插入元素
    }

    string key;
    int doc_id=0;
    double weight = 0;

    while(f_invert >> key >> doc_id >> weight)
    {
        _invertIndexTable[key].insert(make_pair(doc_id,weight));//在倒排索引表中进行插入
    }
} 
    
vector<double> webPageQuery::getQueryWordsWeightVector(const vector<string> & queryWords){
    int TF = 0;
    int DF = 0;
    double IDF = 0;
    vector<double> w; 
    for(auto& it : queryWords){
        TF = 0;
        for(auto& it2 : queryWords){
            if(it == it2){
                ++TF;
            }
        }
        DF = _invertIndexTable[it].size();
        if(DF == _offsetLib.size() - 1 || DF == _offsetLib.size()){
            DF--;
        }
        IDF = log(_offsetLib.size()/(DF+1))/log(2);
        w.push_back(TF * IDF);
    }
    //归一化
    double normal;
    for(auto& wit : w){
        normal += wit;
    }
    normal = sqrt(normal);
    for(auto& wit : w){
        wit /= normal;
    }

    return w;

}

//传入 查询单词分词后的 vector , 
bool webPageQuery::executeQuery(const vector<string> & queryWords)//执行查询
{
    _docCosine.clear();
    _queryWords = queryWords;
    //先找出含单词的所有set
    //再对set中的id取交集

    //先找单词
    //再找对应文章id 的 权重
    vector<unordered_map<int, double>> temp;
    for(auto words : queryWords)
    {
        auto it = _invertIndexTable.find(words);     
        if(it != _invertIndexTable.end())
        {
            temp.push_back(it->second);// it->sencond is unordered_map<int, double>    
        }
        else
        {
            return false;
        }
    }

    // vector<unordered_map<int, double>> temp
    //1、取出每个unordered_map中first
    vector<set<int>> tempId;
    for(auto tempMap : temp) //
    {
        set<int>temp2;
        for(auto it : tempMap)
        {
            temp2.insert(it.first);  
        }
        tempId.push_back(temp2);        
    }

    set<int> resDocId;//存储都含有关键词的文章id
    set<int> tempIdSet = *tempId.begin();

    auto it1 = ++tempId.begin();
    for(auto it2 = it1; it1 != tempId.end(); it1++)
    {
        set_intersection(tempIdSet.begin(), tempIdSet.end(), it2->begin(), it2->end(), 
                         inserter(resDocId, resDocId.begin()));
        
        tempIdSet.clear();

        tempIdSet = resDocId;
    }

    resDocId = tempIdSet;

    //2、记录每个文章id 中所有word的权重
    vector<pair<int, vector<double>>> resultVecTemp;
    resultVecTemp.clear();
    
    for(auto itDocid : resDocId)
    {
        vector<double> weight;
        for(auto word : queryWords)
        {
            auto it = _invertIndexTable.find(word);
            // cout << "word   " << word <<endl;
            if(it != _invertIndexTable.end())
            {
                unordered_map<int, double> temp = it->second;
                auto w = temp.find(itDocid);//文章中每个单词的权重
                if(w != temp.end())
                {
                    double wordWeight = w->second;
                    weight.push_back(wordWeight);
                }
            }
            else
            {
                return false;
            }
        }
        resultVecTemp.push_back(make_pair(itDocid, weight));
    }

    //获取基准向量
    vector<double>Base = getQueryWordsWeightVector(queryWords);
    //vector<pair<int, vector<double>>> &resultVec
    //vector<double> weight;

    for(auto itresultVecTemp = resultVecTemp.begin(); itresultVecTemp != resultVecTemp.end(); ++itresultVecTemp)
    {
        auto vecWeight = itresultVecTemp->second;
        double XY = 0;
        double absolute_X = 0, absolute_Y = 0; //X Y的绝对值
        for(auto wordWeight = vecWeight.begin(), baseWeight = Base.begin();  
            wordWeight != vecWeight.end() && baseWeight != Base.end();
            wordWeight++, baseWeight++)
        {
            // cout << "*wordWeight:       " << *wordWeight << " baseWeight:     " << *baseWeight <<endl;                     
            XY += (*wordWeight) * (*baseWeight);
            //cout << XY << endl;
            absolute_X += (*wordWeight) * (*wordWeight);
            absolute_Y += (*baseWeight) * (*baseWeight);
        }
        double tmp1 = sqrt(absolute_X); 
        double tmp2 = sqrt(absolute_Y);
        double tmp = tmp1 * tmp2;
        //cout << "XY : " << XY << "  tmp: " << tmp <<endl;
        double cos = XY /tmp ;
        // cout << "cos : " << cos << endl;
        _docCosine.insert(make_pair(cos, itresultVecTemp->first));
        vecWeight.clear(); 
    }

    return true;
} 

string webPageQuery::createJson()
{
    json jsonRes;
    int i = 0;
    ifstream f_page(_pageLibPath);

    for(auto it = _docCosine.begin(); it != _docCosine.end() && i < 5; ++it, ++i) 
    {
        auto offdoc = _offsetLib.find(it->second);
        f_page.seekg(offdoc->second.first, ios::beg);
        char buf[offdoc->second.second] = {0};
        f_page.read(buf, offdoc->second.second);
        string doc(buf);
        int titlePos1 = 0;
        int titlePos2 = 0;
        int ContentPos1 = 0;
        int ContentPos2 = 0;
        int summaryPos1 = 0;
        int summaryPos2 = 0;
        string jsonTitlte = {0};
        string jsonContent = {0};
        string jsonSummary = {0};
        //包装jsonTitlte
        if((titlePos1 = doc.find("<title>")) != string::npos)//title第一次出现的位置
        {
            if( (titlePos2 = doc.find("</title>")) != string::npos)
            {
                
                jsonTitlte = doc.substr(titlePos1 + 7 , titlePos2 - titlePos1 - 7);
                //cout << "titlte---------" << titlte << endl;
            }
        }

        jsonRes["title" + std::to_string(i)] = jsonTitlte;

        //包装jsonSummary

        //for

        if((summaryPos1 = doc.find("<title>")) != string::npos)//title第一次出现的位置
        {
            if( (summaryPos2 = doc.find("</title>")) != string::npos)
            {
                
                jsonTitlte = doc.substr(titlePos1 + 7 , titlePos2 - titlePos1 - 7);
                //cout << "titlte---------" << titlte << endl;
            }
        }

        jsonRes["title" + std::to_string(i)] = jsonTitlte;

        // string summary = "summary: ";
        // auto start = doc.find("<content>");
        // if(start != string::npos){
        //     summary += doc.substr(start + 9, (size_t)30);
        // }
        // std::smatch m;
        // string regex_w;
        // for(auto& q : _queryWords){
        //     regex_w = q +"[^。]*";
        //     std::regex r(regex_w.c_str());
        //     std::regex_search(doc, m, r);
        //     summary = summary + "..." + m[0].str();
        // }
        // auto pos = std::remove(summary.begin() ,summary.end(), ' ');
        // summary.erase(pos);
        // cout<< "summary" << summary << endl;
        // _queryWords.clear();
        // jsonRes["summary" + std::to_string(i)] = summary;
       
        //包装jsonContent
        if((ContentPos1 = doc.find("<content>")) != string::npos)//title第一次出现的位置
        {
            if( (ContentPos2 = doc.find("</content>")) != string::npos)
            {
                jsonContent = doc.substr(ContentPos1 + 9 , ContentPos2 - ContentPos1 - 9);
            }
        }
        jsonRes["content" + std::to_string(i)] = jsonContent;
    }
    return jsonRes.dump();  
}

string webPageQuery::returnNoAnswer()
{
    return "No such this webPage";
}




