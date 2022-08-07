#ifndef webPage_hh_
#define webPage_hh_

#include <string>
#include <map>
#include <memory>

using std::string;
using std::shared_ptr;
using std::map;

class Configuration;
class splitToolCppJieBa;

class WebPage{
     friend bool operator < (const WebPage & lhs, const WebPage & rhs);
    // friend bool operator==(const WebPage & lhs, const WebPage & rhs);
public:
    WebPage(const string& doc,
            uint64_t u,
            Configuration& conf, 
            splitToolCppJieBa& jieba);
    ~WebPage();
    int getDocId();
    int getDocId() const;
    string getDoc() const;
    string getUrl() const;
    string getTitle() const;
    string getContent() const;
    uint64_t getSimHash() const;
    string getSunmary();
    map<string, int> getWordsMap() const;

private:
    //处理解析后的xml文件
    void processDoc();
    
    string _doc;//整篇xml
    int _docId;
    string _docTitle;
    string _docUrl;
    string _docContent;
    string _docSummary;//摘要
    uint64_t _u;//simhash值；
    map<string, int> _wordsMap;//词频
    splitToolCppJieBa& _jieba;
    Configuration& _conf;
};

#endif 