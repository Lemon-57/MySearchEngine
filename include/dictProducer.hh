#ifndef _DICTPRODUCER_HH_
#define _DICTPRODUCER_HH_

#include<dirent.h>
#include<string.h>
#include <codecvt>
#include <locale>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<utility>
#include<algorithm>

using std::istringstream;
using std::set;
using std::map;
using std::string;
using std::vector;
using std::pair;
using std::ofstream;
using std::endl;
using std::locale;
using std::wstring;

class Configuration;
class splitToolCppJieBa;

class DictProducer {
public: 
    DictProducer( Configuration& conf,
                splitToolCppJieBa& tool);
    
    void createKey();
    
    void buildDict();

    void createIndex();

    void store();
    
private: 
    static string removeLetter(const string line);

    static string removePunctuation(const string line);
    
    void buildEnDict();

    void buildCnDict();
    
    vector<string> _en_files;
    vector<string> _cn_files;
    vector<pair<string,int>> _dict;
    map<string,set<int>> _index;
    splitToolCppJieBa& _cuttor;
    Configuration& _conf;
};

#endif //_DICTPRODUCER_H
