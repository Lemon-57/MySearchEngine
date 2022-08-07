#ifndef configuration_hh_
#define configuration_hh_

#include <string>
#include <map>
#include <set>
#include <fstream>

using std::string;
using std::map;
using std::set;
using std::ifstream;

//配置文件类
class Configuration{
public:
    Configuration(const string& filePath);
    ~Configuration(); 
    map<string,string> getConfigMap();
    set<string> getStopWordList();
private:
    void readStopWord(ifstream &file);

    string _filePath;//配置文件路径
    map<string, string> _configMap;//存放配置文件内容
    set<string> _stopWordList;//停用词词集
};


#endif 