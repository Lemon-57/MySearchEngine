#include "../include/dictProducer.hh"
#include "../include/configuration.hh"
#include"../include/splitToolCppJieBa.hh"
#include "../include/myLog.hh"

//artDirPath是语料路径
DictProducer::DictProducer( Configuration& conf, 
                            splitToolCppJieBa& tool)
:_cuttor(tool), _conf(conf){
    string enArtDirPath = conf.getConfigMap()["en_art"];
    DIR* enDirp = opendir(enArtDirPath.c_str());//打开目录流
    if(enDirp == nullptr){
        perror("opendir");
        return;
    }
    //循环把语料目录中的文件名读出，存到_files容器中
    while(1){
        struct dirent* pdr = readdir(enDirp);
        if(pdr == nullptr) break;//.和..不读
        if(strcmp(pdr->d_name, ".") && strcmp(pdr->d_name, "..")){
            string path;
            //文件名拼接artDirPath变成绝对路径
            path += enArtDirPath;
            path += "/";
            path += pdr->d_name;
            _en_files.push_back(path);
        }
    }

    LogInfo((char*)"load en_corpus success.");

    closedir(enDirp);
    string cnArtDirPath = conf.getConfigMap()["cn_art"];
    DIR* cnDirp = opendir(cnArtDirPath.c_str());//打开目录流
    if(cnDirp == nullptr){
        perror("opendir");
        return;
    }
    while(1){
        struct dirent* pdr = readdir(cnDirp);
        if(pdr == nullptr) break;//.和..不读
        if(strcmp(pdr->d_name, ".") && strcmp(pdr->d_name, "..")){
            string path;
            path += cnArtDirPath;
            path += "/";
            path += pdr->d_name;
            _cn_files.push_back(path);
        }
    }
    closedir(cnDirp);

    LogInfo((char*)"load cn_corpus success.");
}

void DictProducer::buildEnDict() {
    //读语料库文章,files里是文件名，遍历读完vec中所有文章
    for(auto& filename : _en_files){
        //每个文件名，读一行，分成词，大小写转换，去除标点，得到一个词
        ifstream ifs(filename); 
        if(!ifs) perror("ifstream");
        string line;
        while(getline(ifs, line)){//读一行到line
            istringstream iss(line);
            string word;
            for(auto& ch : line){//对line一个一个字符的读
                if(isalpha(ch)){//这个是字母
                    word += tolower(ch);//拼接到word尾部
                }else{//否则
                    if(word.size()){//如果word中有字母的，考虑往dict里面插
                        auto dictIt = _dict.begin();
                        for(; dictIt != _dict.end(); ++dictIt){
                            //这个词在vec.pair.first已经有了，int++
                            if(dictIt->first == word){
                                ++dictIt->second;
                                break;
                            }
                        }
                        //没有，push_back一个pair,first是这个词，int=1
                        if(dictIt == _dict.end()){
                            _dict.push_back(std::make_pair(word, 1));
                        }
                        word.clear();
                    }
                }
            }
        }
        ifs.close();
    }
}

void DictProducer::createIndex() {
    int dictLineNum = 1;
    for(auto& dictElem : _dict){
        string word = dictElem.first;
        size_t idx = 0;
        while(idx < word.size()){
            if(word[idx] & (char)128){
                _index[word.substr(idx, 3)].insert(dictLineNum);
                idx += 3;
            }else{
                _index[word.substr(idx, 1)].insert(dictLineNum);
                ++idx;
            }
        }
        ++dictLineNum;
    } 
}

//存储成磁盘文件
void DictProducer::store() {
    //这部分改文件名可以移植到中文
    ofstream ofsEnDict(_conf.getConfigMap()["dict"]);
    for(auto& dictElem : _dict){
        string dictLine;
        dictLine += dictElem.first;
        dictLine += " ";
        dictLine += std::to_string(dictElem.second);
        ofsEnDict << dictLine << endl;
    }
    ofsEnDict.close();

    LogInfo((char*)"create dict.dat success.");

    ofstream ofsEnIndex(_conf.getConfigMap()["index_table"]);
    for(auto& indexElem : _index){
        string indexLine;
        indexLine += indexElem.first;
        for(auto& lineNum : indexElem.second){
            indexLine += " ";
            indexLine += std::to_string(lineNum);
        }
        ofsEnIndex<< indexLine << endl;
    }
    ofsEnIndex.close();

    LogInfo((char*)"create index_table.dat success.");

}

string DictProducer::removeLetter(const string line)
{
    string newline;

    int i = 0;
    while(line[i] != 0)
    {
        if(line[i] & 0x80 && line[i] & 0x40 && line[i] & 0x20)
        {
            int byteCount = 0;
            if(line[i] & 0x10)
                byteCount = 4;
            else
                byteCount = 3;
            for(int j = 0; j < byteCount; ++j)
            {
                newline += line[i];
                ++i;
            }
        }
        else if(line[i] & 0x80 && line[i] && 0x40)
        {
            i += 2;
        }
        else
        {
            i += 1;
        }
    }
    return newline;
}

string DictProducer::removePunctuation(string line)
{
    locale loc("en_US.UTF-8");
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    wstring ws = conv.from_bytes(line);
    wstring newws;
    for(wchar_t ch : ws)
    {
        if(!ispunct(ch, loc) && !iswspace(ch))
            newws.push_back(ch);
    }
    string newline = conv.to_bytes(newws);
    return newline;
}

void DictProducer::buildCnDict() 
{
    map<string, int> mp;
    for(auto& filename : _cn_files)
    {
        ifstream ifs(filename);
        if(!ifs)
        {
            perror("ifstream");
            exit(-1);
        }
        string line;
        while(getline(ifs, line))
        {
            string newlinefirst = DictProducer::removeLetter(line);
            string newlinesecond = DictProducer::removePunctuation(newlinefirst);
            auto splitWords = _cuttor.cut(newlinesecond);
            for(auto& it : splitWords)
            {
                mp[it] += 1;
            }
        }
        ifs.close();
    }
    _dict.insert(_dict.end(), mp.begin(), mp.end());
}

void DictProducer::buildDict(){
    buildEnDict();
    buildCnDict();
    //形成字典后，遍历并对比停用词集合存在的就删掉
    for(auto dictIt = _dict.begin(); dictIt != _dict.end();){
        if(_conf.getStopWordList().count(dictIt->first)){
            _dict.erase(dictIt);
            continue;
        }
        ++dictIt;
    }
    //得到最后的字典可以收缩一下
    _dict.shrink_to_fit();
}

void DictProducer::createKey(){
    buildDict();
    createIndex();
    store();
}

