#ifndef fileProcessor_hh_
#define fileProcessor_hh_

#include <string>
#include <vector>

using std::string;
using std::vector;

struct RssItem {
    string title;
    string description;
};

class FileProcessor {
public:
    FileProcessor(int docid);
    vector<string> process(const string& fileName);//解析xml文件
private:
    int _docid;//docid要连续
    vector<RssItem> _rss;
};


#endif
