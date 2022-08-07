#include "../include/Component.hh"

vector<string> empty2;

vector<string>& Component::searchSelf(string keyword)
{
    cout << "thread_search" << endl;
    cout << std::this_thread::get_id() <<endl;
    if(this->_keymap.count(keyword) == 1)
        return (*(this->_keymap[keyword])).second;
    else
        return empty2;
}

vector<string> Component::nonResult(string keyword)
{
    if(_mediator == nullptr)
        cout << "_mediator" << endl;
    return this->_mediator->Notify(this, keyword);
}