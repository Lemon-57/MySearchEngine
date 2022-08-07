#ifndef BaseComponent_HH_
#define BaseComponent_HH_

#include "../include/Mediator.hh"
#include <iostream>
#include <thread>
#include <functional>
#include <string>
#include <vector>
#include <map>

using std::cout;
using std::endl;
using std::thread;
using std::string;
using std::bind;
using std::vector;
using std::map;

class BaseComponent
{
public:
    BaseComponent(Mediator* mediator = nullptr) 
    : _mediator(mediator)
    {

    }

    void set_mediator(Mediator* mediator)
    {
        this->_mediator = mediator;
    }

protected:
    Mediator* _mediator;

};

#endif