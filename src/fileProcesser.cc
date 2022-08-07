#include "../include/tinyxml2.h"
#include "../include/fileProcessor.hh"
#include "../include/dirScanner.hh"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>


using namespace tinyxml2;
using std::cout;
using std::endl;
using std::ofstream;
using std::regex;
using std::regex_replace;
using std::smatch;
using std::vector;
using std::string;

const char* CHANNEL = "channel";
const char* TITLE = "title";
const char* DESCRIPTION = "description";
const char* ITEM = "item";
const char* CONTENT1 = "content";
const char* CONTENT2 = "content:encoded";

FileProcessor::FileProcessor(int docid): _docid(docid){ }

vector<string> FileProcessor::process(const string& fileName) {
    XMLDocument doc;
	
    XMLError ret = doc.LoadFile(fileName.c_str());
    if (ret != 0) {
        cout << "load file failed." << endl;
    }

    XMLElement* root = doc.RootElement();
    
    XMLElement* channel = root->FirstChildElement(CHANNEL);

    //内容
    XMLElement* item = channel->FirstChildElement(ITEM);
    while (item) {
        XMLElement* title = item->FirstChildElement(TITLE);
        XMLElement* description = item->FirstChildElement(DESCRIPTION);
        XMLElement* content1 = item->FirstChildElement(CONTENT1);//<content>
        XMLElement* content2 = item->FirstChildElement(CONTENT2);//<content:encode>

        //正则去除html标签
        regex regTitle("(<\\!\\[CDATA)|(<([^<]+)>)|(\\n)");
        regex regDes("(<([^>]+)>)|(\\n)");

        string titleStr = " ";
        string desStr = " ";

        if(content1){
            //内容标签是<content>
            description = content1;
        }
        if(content2){
            //内容标签是<content:encode>
            description = content2;
        }
        if(description){
            //有description标签
            desStr = description->GetText();
            desStr = regex_replace(desStr, regDes, "");
        }

        titleStr = title->GetText();
        titleStr = regex_replace(titleStr, regTitle, "");
        
        _rss.emplace_back(RssItem{ titleStr,
                                   desStr,});

        item = item->NextSiblingElement();
    }

    vector<string> result;
    string fmTxt;
    for(auto & item : _rss){
        fmTxt = "<doc><docid>" + std::to_string(_docid) + "</docid><url>" 
                + fileName + "</url><title>" 
                + item.title +"</title><content>" 
                + item.description + "</content></doc>";
        result.push_back(fmTxt);
        _docid++;
    }
    //清空
    _rss.clear();
    _rss.shrink_to_fit();

    return result;
 }
