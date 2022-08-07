#include "../include/dictionary.hh"



//Dictionary* Dictionary::_pdict = nullptr;
//Dictionary::AutoRelease Dictionary::_ar;//将单例的自动释放成员对象在全局构造出来，
//
Dictionary::Dictionary(Configuration& conf)
:_conf(conf){
    ifstream ifsDict(_conf.getConfigMap()["dict"]);
    if(!ifsDict.is_open()) perror("ifstream");
    string line;
    while(getline(ifsDict, line)){
        istringstream iss(line);
        string word;
        iss >> word;
        string freq;
        iss >> freq;
        _dict.push_back(std::make_pair(word, std::stoi(freq)));
    }
    ifsDict.close();

    ifstream ifsIndex(_conf.getConfigMap()["index_table"]);
    if(!ifsIndex.is_open()) perror("ifstream");
    while(getline(ifsIndex, line)){
        istringstream iss(line);
        string word;
        iss >> word;
        _index[word];
        string lineNum;
        while(iss >> lineNum){
            _index[word].insert(std::stoi(lineNum));
        }
    }
    ifsIndex.close();
}

vector<pair<string, int>>& Dictionary::getDict(){
    return _dict; 
}

map<string, set<int>>& Dictionary::getIndex(){
    return _index;
}

//Dictionary* Dictionary::getInstance(){//单例的初始化函数
//    if(!_pdict) {
//        _pdict = new Dictionary();
//    }
//    return _pdict;
//}
