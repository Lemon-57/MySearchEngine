#include "../include/redisCache.hh"

RedisCache::RedisCache(string address)
    :_redisCache1(Redis(address))//该redis句柄默认使用0号数据库
    ,_redisCache2(Redis(address+"/1"))//创建一个redis句柄使用1号数据库
    {}

void RedisCache::getRcmCache(string queryWord, vector<string>& cacheData){
    _redisCache1.lrange(queryWord, 0, -1, std::back_inserter(cacheData));
}

void RedisCache::updateRcmCache(string queryWord, string candiWord){
    _redisCache1.rpush(queryWord, candiWord);
}

void RedisCache::getWebCache(string queryWords, vector<string>& cacheData){
    _redisCache2.lrange(queryWords, 0, -1, std::back_inserter(cacheData));
}

void RedisCache::updateWebCache(string queryWords, string abstract){
    _redisCache2.rpush(queryWords, abstract);
}