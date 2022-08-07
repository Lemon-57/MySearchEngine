#ifndef splitToolCppJieBa_hh_
#define splitToolCppJieBa_hh_

#include "../include/simhash/cppjieba/Jieba.hpp"

#include <vector>
#include <string>

using std::vector;
using std::string;

const char* const DICT_PATH = "../include/simhash/cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../include/simhash/cppjieba/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../include/simhash/cppjieba/dict/user.dict.utf8";
const char* const IDF_PATH = "../include/simhash/cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../include/simhash/cppjieba/dict/stop_words.utf8";

//中文分词类
class splitToolCppJieBa{
public:

    splitToolCppJieBa();
    ~splitToolCppJieBa();
    //分词函数(对解析后的每个xml文件分词)
    vector<string> cut(const string& sentence);
private:


    cppjieba::Jieba _jieba;
};


#endif 