#include "../include/ConcreteMediator.hh"

vector<string> empty1;

ConcreteMediator* ConcreteMediator::_med{nullptr};
mutex ConcreteMediator::_mutex;

ConcreteMediator::ConcreteMediator(vector<Component*> vec)
:_vec(vec)
{
    for(auto tp : _vec)
    {
        cout << "set" << endl;
        tp->set_mediator(this);
    }
}

vector<string> ConcreteMediator::Notify(BaseComponent* sender, string keyword) const
{
    for(auto tmp : this->_vec)
    {
        vector<string> res = tmp->searchSelf(keyword);
        if(!res.empty())
            return res;
    }
    return empty1;
}

ConcreteMediator* ConcreteMediator::getInstance(const vector<Component*> vec)
{
    lock_guard<mutex> lock(_mutex);
    if(_med == nullptr)
        _med = new ConcreteMediator(vec);
    return _med;
}