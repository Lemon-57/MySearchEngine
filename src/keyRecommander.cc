#include "../include/keyRecommander.hh"
#include "../include/myLog.hh"

CandidateResult::CandidateResult(string candiWord, int freq, int dist)
    :_word(candiWord)
    ,_freq(freq)
     ,_dist(dist)
{}

bool CandidateCompare::operator()(const CandidateResult& lhs, const CandidateResult& rhs){
    if(lhs._dist > rhs._dist) {
        return true;
    }else if(lhs._dist < rhs._dist){
        return false;
    }else if(lhs._freq < rhs._freq) {
        return true;
    }else if(lhs._freq > rhs._freq) {
        return false;
    }else if(lhs._word < rhs._word) {
        return false;
    }else return false;
}

KeyRecommander::KeyRecommander(Dictionary& dict, RedisCache& redis)
: _dict(dict), _redis(redis){}

void KeyRecommander::doQuery(const string& queryWord) {
    _queryWord = queryWord;
    priority_queue<CandidateResult, 
    vector<CandidateResult>, CandidateCompare> newresultQue;
    swap(_resultQue, newresultQue);
    _cacheData.clear();
    getCache();
    if(!_cacheData.empty()) return;
    set<int> lineSet; 
    for(size_t idx = 0; idx < _queryWord.size();){
        string character;//获取单个中英字符存入character
        if(_queryWord[idx] & (char)128){
            character = _queryWord.substr(idx, 3);
            idx += 3;
        }else{
            character = _queryWord.substr(idx, 1);
            ++idx;
        }
        auto idxIt = _dict.getIndex().find(character);
        if(idxIt != _dict.getIndex().end()){
            for(auto idxSetIt = idxIt->second.begin(); idxSetIt != idxIt->second.end(); ++idxSetIt){
                lineSet.insert(*idxSetIt);
            }
        }
    } 
    for(auto& lineNum : lineSet){
        string candiWord = _dict.getDict()[lineNum - 1].first;
        _resultQue.push(CandidateResult(candiWord, _dict.getDict()[lineNum - 1].second, distance(candiWord))); 
    }
}

int KeyRecommander::distance(const string& rhs){
    return editDistance(_queryWord, rhs);
}

vector<string>& KeyRecommander::response(int resultNum){
    //如果cache里有数据
    if(_cacheData.empty()){
        while(_resultQue.size() && resultNum-- > 0){//不能一直循环，要判断容器是否为空
            string word = _resultQue.top()._word;
            _cacheData.push_back(word);
            _resultQue.pop();
            _redis.updateRcmCache(_queryWord, word);
        }
        LogInfo((char*)"search keyWords from lib success");
    }else{
        LogInfo((char*)"search keyWords from cache success");
    }

    

    return _cacheData;
}

void KeyRecommander::getCache(){
    _redis.getRcmCache(_queryWord, _cacheData);
}
