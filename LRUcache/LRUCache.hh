#ifndef LRUCache_HH_
#define LRUCache_HH_

#include "../include/Mediator.hh"
#include "../include/BaseComponent.hh"
#include "../include/Component.hh"
#include "../include/ConcreteMediator.hh"
#include "../include/keyRecommander.hh"

#include <iostream>
#include <string>
#include <thread>
#include <functional>
#include <vector>
#include <map>
#include <mutex>
#include <list>
#include <utility>
#include <unordered_map>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::bind;
using std::vector;
using std::map;
using std::mutex;
using std::lock_guard;
using std::list;
using std::pair;
using std::unordered_map;
using std::make_pair;
using std::unique_ptr;

class LRUCache {
public:
    friend class KeyRecommander;
    LRUCache(int capacity);
    
    vector<string> getLRUCache(string keyword);

    void updataLRUCache(string, vector<string>);

    string printResult();

    list<pair<string, vector<string>>> _cList;
    unordered_map<string, list<pair<string, vector<string>>>::iterator> _cMap;
    int _capacity;
    static mutex _mutex;
    Component* _cache;
    vector<string> _result;
};

#endif