#ifndef _REDISCACHE_HH_
#define _REDISCACHE_HH_

#include "../include/noCopy.hh"
#include <sw/redis++/redis++.h>
#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <vector>
#include <map>

using std::vector;
using namespace sw::redis;
using std::string;
using std::map;

class RedisCache
:public NoCopy
{
public:
//address为redis服务器的网络地址，格式："tcp://127.0.0.1:6379",需要写在配置文件中，并从中读取
// Connect to Redis and select the 2nd (db number starts from 0) database.
//Redis redis4("tcp://127.0.0.1:6379/2");
RedisCache(string address);

//queryWord为查询词
void getRcmCache(string queryWord, vector<string>& cacheData);
//candiWord为推荐词，将其插入缓存中
void updateRcmCache(string queryWord, string candiWord);

//网页搜索
//queryWords为查询词
void getWebCache(string queryWords, vector<string>& cacheData);
//abstract为摘要，将其插入缓存中
void updateWebCache(string queryWords, string abstract);

private:
Redis _redisCache1;
Redis _redisCache2;
};

#endif