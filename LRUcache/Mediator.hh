#ifndef Mediator_HH_
#define Mediator_HH_

#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::thread;
using std::bind;
using std::vector;
using std::string;

class BaseComponent;

class Mediator
{
public:
    virtual vector<string> Notify(BaseComponent* sender, string keyword) const = 0;
};

#endif