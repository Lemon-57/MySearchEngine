#include "../include/LRUCache.hh"
#include "../include/keyRecommander.hh"

class KeyRecommander;

//vector<string> empty;
extern vector<Component*> m;
//extern KeyRecommander kr;

mutex LRUCache::_mutex;

LRUCache::LRUCache(int capacity)
:_capacity(capacity)
{
    cout << "create cache" << endl;
    cout << std::this_thread::get_id() <<endl;
    _cache = new Component(this->_cMap);
    m.push_back(_cache);
}

void thread_obj(KeyRecommander& kr)
{
    kr.doQuery();
}

vector<string> LRUCache::getLRUCache(string keyword)
{
    if (_cMap.find(keyword) == _cMap.end()) 
    {
        KeyRecommander kr(keyword);
        //auto obj = bind(thread_obj, kr);
        //thread threadfind(obj);
        if(_cache == nullptr)
            cout << "_cache" << endl;
        _result = this->_cache->nonResult(keyword);
        //threadfind.join();
        if(_result.empty())
        {
            _result = kr.doQuery();
        }
        return _result;
    }
    else
    {
        pair<string, vector<string>> node = *_cMap[keyword];
        _cList.erase(_cMap[keyword]);
        _cList.push_front(node);
        _cMap[keyword] = _cList.begin();
        _result = node.second;
        cout << "cache" <<endl;
        return _result;
    }
}

void LRUCache::updataLRUCache(string keyword, vector<string> value) 
{
    lock_guard<mutex> lock(_mutex);
    auto newNode = make_pair(keyword, value);

    if (_cMap.count(keyword)) {
        _cList.erase(_cMap[keyword]);
    }
    else 
    {
    if (_cList.size() == _capacity) 
    {
        _cMap.erase(_cList.back().first);
        _cList.pop_back();
    }
}

    _cList.push_front(newNode);
    _cMap[keyword] = _cList.begin();

    cout << "updata" << endl;
}

string LRUCache::printResult()
{
    string res;
    for(auto& n : _result){
        res += "[";
        res += n;
        res += "] ";
    }
    res += "\n";
    return res;
}
