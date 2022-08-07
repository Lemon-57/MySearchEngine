#ifndef ConcreteMediator_HH_
#define ConcreteMediator_HH_

#include "../include/Mediator.hh"
#include "../include/Component.hh"
#include <iostream>
#include <string>
#include <thread>
#include <functional>
#include <vector>
#include <map>
#include <mutex>

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::bind;
using std::vector;
using std::map;
using std::mutex;
using std::lock_guard;

class ConcreteMediator
: public Mediator
{
public:
    ConcreteMediator(ConcreteMediator &other) = delete;
    void operator=(const ConcreteMediator &) = delete;
    static ConcreteMediator* getInstance(const vector<Component*> vec);

    vector<string> Notify(BaseComponent* sender, string keyword) const override;

protected:
    vector<Component*> _vec;
    ConcreteMediator(const vector<Component*> vec);

private:
    static ConcreteMediator* _med;
    static mutex _mutex;
};

#endif