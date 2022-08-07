#include "../include/webPage.hh"
#include "../include/splitToolCppJieBa.hh"
#include "../include/configuration.hh"
#include "../include/simhash.hh"
#include <ctype.h>
#include <iostream>
#include <algorithm> 
#include <vector>
#include <set>
#include <stdlib.h>

using std::vector;
using std::cout;
using std::endl;
using std::set;

WebPage::WebPage(const string& doc, 
                uint64_t u,
                Configuration& conf, 
                splitToolCppJieBa& jieba)
: _doc(doc), _u(u), _conf(conf), _jieba(jieba){
    //处理
    getDocId();//更新id(去重后id不连续)
    processDoc();
}

WebPage::~WebPage(){ }

//获取文档的id
int WebPage::getDocId(){
    //<doc><docid>id
    int preSize = 12;//前面12个是<doc><docid>
    int i = preSize;

    int len = 0;
    while(isdigit(_doc[i])){
        ++len;
        ++i;
    }

    string s0 = _doc.substr(preSize, len);//提取数字子串

    _docId = atoi(s0.c_str());

    return _docId;
}

int WebPage::getDocId() const{
    return _docId;
}

//获取文档
string WebPage::getDoc() const{
    return _doc;
}

string WebPage::getUrl() const//获取URL
{
    return _docUrl;
}

string WebPage::getTitle() const//获取文章标题
{
    return _docTitle;
}    

string WebPage::getContent() const//获取文章内容
{
    return _docContent;
}

uint64_t WebPage::getSimHash() const{
    return _u;
}

//获取文档的词频统计 map
map<string, int> WebPage::getWordsMap() const {
    return _wordsMap;
}

//对格式化文档进行处理
void WebPage::processDoc(){
    //获取url
     auto start = _doc.find_first_of("<url>");
     auto end = _doc.find_first_of("</url>");
     _docUrl = _doc.substr(start + 5, start - end);

    //获取title
    start = _doc.find_first_of("<title>");
    end = _doc.find_first_of("</title>");
    _docTitle = _doc.substr(start + 7, start - end);

    //获取content
    start = _doc.find_first_of("<content>");
    end = _doc.find_first_of("</content>");
    if(start != std::string::npos){
        _docContent = _doc.substr(start + 9, start - end);
    }
    
    set<string> stopList = _conf.getStopWordList();//获取停用词词集

    vector<string> cutSentence =  _jieba.cut(_doc);//将文章分词 //每个string是一个词

    // //从分词中查找第一个title并拼接到
    // auto title = find(cutSentence.begin(), cutSentence.end(), "title");
    // if (title != cutSentence.end()){
    //     //cout << "查找成功：" << *title <<endl;
    //     ++title;//标题的第一个string位置
    //     for(; *title != "<"; ++title){
    //         _docTitle += *title;  
    //     }
    // }
    // else
    // {
    //     //cout << "查找失败" <<endl;;
    // }

    // //从分词中查找第一个url并拼接到<
    // auto url = find(cutSentence.begin(), cutSentence.end(), "url");
    // if (url != cutSentence.end()){
    //    //cout << "查找成功：" << *url << endl;
    //     ++url;//标题的第一个string位置
    //     for(; *url != "<"; ++url){
    //         _docUrl += *url;  
    //     }
    // }
    // else
    // {
    //     //cout << "查找失败" <<endl;;
    // }

    // //从分词中查找第一个content并拼接到<
    // auto content = find(cutSentence.begin(), cutSentence.end(), "content");
    // if (content != cutSentence.end()){
    //     //cout << "查找成功：" << *content << endl;
    //     ++content;//标题的第一个string位置
    //     for(; *content != "<"; ++content){
    //         _docContent += *content;  
    //     }
    // }
    // else
    // {
    //     //cout << "查找失败" <<endl;;
    // }
    

    //遍历停用词词集 从文章分词中逐个删除
    for(auto it = stopList.begin(); it != stopList.end(); ++it)
    {
        string stopword = *it;
        cutSentence.erase(std::remove(cutSentence.begin(), cutSentence.end(), stopword), cutSentence.end());
    }

    //统计词频写入_wordsMap中
    for(auto word : cutSentence){
        auto ret = _wordsMap.insert({word, 1});//插入单词，次数为1
        if(!ret.second)//返回值为键值对，且first为已存的键，second为bool值，当seconde为false时，则插入失败，表示已存在
        {
            ++_wordsMap[word];//对应的键值对的词频+1
        }    
    }
}

//文章摘要
string WebPage::getSunmary(){
    int i = 0;
    for(auto it = _wordsMap.begin(); it != _wordsMap.end(); ++it){
        i++;
        if(i < 20){
           _docSummary +=  it->first;  
        }
    }
    return _docSummary;
}

// bool operator==(const WebPage & lhs, const WebPage & rhs)  //判断两篇文档是否相等 
// {

//     //固定的传入词典路径  
//     Simhasher simhasher("../include/simhash/cppjieba/dict/jieba.dict.utf8", "../include/simhash/cppjieba/dict/hmm_model.utf8",
//                            "../include/simhash/cppjieba/dict/idf.utf8", "../include/simhash/cppjieba/dict/stop_words.utf8");

//     size_t topN = 10;//选取权重最高的10个词
//     uint64_t u1 = 0, u2 = 0;
//     vector<pair<string ,double> > res1, res2;
    
//     string s1 = lhs.getDoc();
//     simhasher.extract(s1, res1, topN);//提取关键词和权重到res
//     simhasher.make(s1, topN, u1);//计算哈希值并存到u64中

//     string s2 = rhs.getDoc();
//     simhasher.extract(s2, res2, topN);//提取关键词和权重到res
//     simhasher.make(s2, topN, u2);//计算哈希值并存到u64中

//     //isEuqal 相似返回1 不相似返回0
//     if(Simhasher::isEqual(u1, u2))
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }
// }

bool operator < (const WebPage & lhs, const WebPage & rhs){
    if(Simhasher::isEqual(lhs.getSimHash(), rhs.getSimHash())){
        return false;
    }
    else{
        if(lhs.getDocId() < rhs.getDocId()){
            return true;
        }
        else{
            return false;
        }
    }
}

    

