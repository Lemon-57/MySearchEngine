#ifndef protocolParser_hh_
#define protocolParser_hh_

#include "webPageResearcher.hh"
#include "keyRecommander.hh"
#include <string>

using std::string;

class ProtocolParser
{
public:
    ProtocolParser(string msg, 
                    KeyRecommander & keyRecommander,
                    WebPageResearcher & webPageResearcher);

    string doParse();
private:
    string _msg;
    vector<string> _queryWords;
    KeyRecommander & _keyRecommander; 
    WebPageResearcher & _webPageResearcher;
};

#endif 