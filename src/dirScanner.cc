#include"../include/dirScanner.hh"
#include "../include/myLog.hh"
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>


using std::cout;
using std::endl;

DirScanner::DirScanner(){ }

DirScanner::~DirScanner(){ }

void DirScanner::operator()(const string &dirname){
    traverse(dirname);
}

vector<string> & DirScanner::files(){
    return _files;
}

void DirScanner::traverse(const string &dirname){
    DIR *dir = opendir(dirname.c_str());        //成功则返回DIR* 型态的目录流, 打开失败则返回NULL
        if (!dir) {
                cout << " opendir ERROR!" << endl;
        exit(-1);
        }

        struct dirent *pdirent;
        while ((pdirent = readdir(dir)) != nullptr) {   //遍历传入文件夹里的内容
                string filename(pdirent->d_name);
        if (("." == filename) || ".." == filename)
                continue;
                string s = dirname + "/" + pdirent->d_name;    // 文件名的格式调整为绝对路径
        struct stat st;
        if(lstat(s.c_str(),&st)==-1){//读取文件类型
            perror("DirScanner::traverse.lstat.");
            return;
        }
        if(S_ISDIR(st.st_mode)){
            traverse(s);
        }else{
            _files.push_back(s);  //存到语料库文件的绝对路径的vector<string> _files里
        }
    }
    closedir(dir);
    
    LogInfo((char*)"load xml files paths success.");
}
