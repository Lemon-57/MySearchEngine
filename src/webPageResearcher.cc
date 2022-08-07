#include "../include/webPageResearcher.hh"
#include "../include/myLog.hh"

using std::string;
// using std::cout;
// using std::endl;
WebPageResearcher::WebPageResearcher(splitToolCppJieBa &pjieba,webPageQuery &webPageQuery,RedisCache &redisCache)
    :_jieba(pjieba)
    ,_webPageQuery(webPageQuery)
    ,_redisCache(redisCache)
    {

    }

string WebPageResearcher::doQuery(string msg){
    _resJson.clear();
    _redisCache.getWebCache(msg,_resJson);
    if(_resJson.empty()){//redis中未找到则进行计算
        _result =_webPageQuery.doQuery(msg);
        //将分词生成的string数组传入webQuery,将其返回的string数组传给_sought
        _redisCache.updateWebCache(msg,_result);
        LogInfo((char*)"search web from lib success");
        return _result;
    }else{
        LogInfo((char*)"search web from cache success");
        return _resJson[0];
    }
    
}