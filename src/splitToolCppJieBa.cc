#include "../include/splitToolCppJieBa.hh"

#include <sstream>

using std::istringstream;

splitToolCppJieBa::splitToolCppJieBa()
//: _jieba( cppjieba::Jieba(DICT_PATH, HMM_PATH, IDF_PATH)){ 
: _jieba(DICT_PATH, HMM_PATH, IDF_PATH){ 
    //创建jieba对象
}

splitToolCppJieBa::~splitToolCppJieBa(){ }

vector<string> splitToolCppJieBa::cut(const string& sentence){

    vector<string> splitResult;

    vector<string> words;

    _jieba.CutForSearch(sentence, words);
    
    //分割，以空格划分
    string splitWords = limonp::Join(words.begin(), words.end(), " ");

    string splitWord;//单个分割词
    istringstream doc(splitWords);//处理string
    while(doc >> splitWord){
        //将每个词存入vector
        splitResult.push_back(splitWord);
    }

    return splitResult;
}
