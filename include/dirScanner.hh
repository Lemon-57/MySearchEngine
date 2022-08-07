#ifndef dirScanner_hh_
#define dirScanner_hh_

#include<vector>
#include<string>

using std::vector;
using std::string;

class DirScanner
{
public:
    DirScanner();
    ~DirScanner();
    void operator()(const string &dirname);
    vector<string> & files();
    void traverse(const string &dirname);
private:
    vector<string> _files;
};

#endif 
