#ifndef _KEYRECOMMANDER_H
#define _KEYRECOMMANDER_H

#include "../include/editDistance.hh"
#include "../include/dictionary.hh"
#include "../include/redisCache.hh"
#include<string>
#include<queue>
#include<vector>
#include<set>
#include<unordered_set>

using std::set;
using std::vector;
using std::string;
using std::priority_queue;

class CandidateResult{
public:
    CandidateResult(string, int, int);

    string _word;
    int _freq;
    int _dist;
};

class CandidateCompare{
public:
    bool operator()(const CandidateResult&, const CandidateResult&);
};

class KeyRecommander {
public: 
    KeyRecommander(Dictionary& dict, RedisCache& redis);
    void doQuery(const string& queryWord);
    int distance(const string& rhs);
    vector<string>& response(int resultNum);
    void getCache();//将cache里的数据存入_cacheData中，成功返回1，空返回0

private:
    Dictionary& _dict;
    RedisCache& _redis;
    string _queryWord;//word from client
    priority_queue<CandidateResult, 
    vector<CandidateResult>, CandidateCompare> _resultQue;
    vector<string> _cacheData;
};

#endif //_KEYRECOMMANDER_H
