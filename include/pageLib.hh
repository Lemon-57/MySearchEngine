#ifndef pageLib_hh_
#define pageLib_hh_

#include <string>
#include <map>
#include <vector>
#include <string>

using std::vector;
using std::pair;
using std::string;
using std::map;

class DirScanner;
class Configuration;
class FileProcessor;

class PageLib
{
public:
    PageLib(Configuration& conf);
    ~PageLib();
    void createWebInit();
    void create();
    void store();
private:
    Configuration& _conf;
    vector<vector<string>> _pages;
    map<int,pair<int,int>> _offsetLib;
};



#endif
