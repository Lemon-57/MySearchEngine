#include "../include/simhash.hh"

SimHash::SimHash()
: _simhasher("../include/simhash/cppjieba/dict/jieba.dict.utf8", 
             "../include/simhash/cppjieba/dict/hmm_model.utf8",
             "../include/simhash/cppjieba/dict/idf.utf8", 
             "../include/simhash/cppjieba/dict/stop_words.utf8"){
    //固定的传入词典路径    
}

SimHash::~SimHash(){ }

uint64_t SimHash::getSim(const string doc1){
    size_t topN = 10;//选取权重最高的10个词
    uint64_t u1 = 0, u2 = 0;
    vector<pair<string ,double> > res1, res2;
    
    _simhasher.extract(doc1, res1, topN);//提取关键词和权重到res
    _simhasher.make(doc1, topN, u1);//计算哈希值并存到u64中

    return u1;
}

