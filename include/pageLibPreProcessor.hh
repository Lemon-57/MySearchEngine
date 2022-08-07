#ifndef pageLibPreProcessor_hh_
#define pageLibPreProcessor_hh_

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <set>
#include <list>

using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::pair;
using std::string;
using std::set;
using std::list;

class Configuration;
class splitToolCppJieBa;
class WebPage;
class SimHash;

class PageLibPreProcessor{
public:
    PageLibPreProcessor(Configuration& conf, 
                        splitToolCppJieBa& jieba,
                        SimHash& simhash);
    void doProcess();
    
private:
    //读取ripepage与offset库
    void readInfoFromFile();
    //去重
    void cutRedundantPages();
    //建倒排索引
    void buildInvetIndex();
    void store();

    splitToolCppJieBa& _jieba;
    Configuration& _conf;
    SimHash& _sim;
    vector<WebPage> _pageLib;
    unordered_map<int, pair<int, int>> _offsetLib;
    unordered_map<string, vector<pair<int, double>>> _invertIndexLib;
};

#endif 