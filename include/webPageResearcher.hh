#ifndef webPageResearcher_hh_
#define webPageResearcher_hh_

#include "redisCache.hh"
#include "splitToolCppJieBa.hh"
#include "webPageQuery.hh"
#include <string>

using std::cout;
using std::endl;

class WebPageResearcher
{
public:
    WebPageResearcher(splitToolCppJieBa &pjieba,webPageQuery &webPageQuery,RedisCache &redisCache);
    string doQuery(string msg);

private:
    string _result;
    vector<string> _resJson;
    splitToolCppJieBa & _jieba;
    webPageQuery &_webPageQuery;
    RedisCache &_redisCache;
};

#endif