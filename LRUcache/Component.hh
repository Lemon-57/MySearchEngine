#ifndef Component_HH_
#define Component_HH_

#include "../include/BaseComponent.hh"
#include "../include/Mediator.hh"
#include <iostream>
#include <string>
#include <thread>
#include <functional>
#include <vector>
#include <map>
#include <utility>
#include <unordered_map>
#include <list>

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::bind;
using std::vector;
using std::map;
using std::pair;
using std::unordered_map;
using std::list;

class Component
: public BaseComponent
{
public:
    Component(unordered_map<string, list<pair<string, vector<string>>>::iterator>keymap)
   :_keymap(keymap)
    {

    }

    vector<string>& searchSelf(string keyword);
    vector<string> nonResult(string keyword);

private:
    unordered_map<string, list<pair<string, vector<string>>>::iterator> _keymap;
};

#endif