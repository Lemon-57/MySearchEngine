#include"../include/socketIO.hh"
#include"../include/func.h"
#include<iostream>
#include<sstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <ctime>
#include <map>
#include <string>

using std::make_pair;
using std::vector;
using std::ostringstream;
using std::endl;
using std::cout;
using std::cin;
using std::string;
using std::map;
using json = nlohmann::json;


string clientSend(string _msg){
    //取出前面的0 1
    string mod(_msg.substr(0, 1));
    //关键词转化为json
    json j(_msg.substr(1));
    //json转化为原生字符串msg
    ostringstream oss;
    oss << j;
    string jmsg = oss.str();
    int msgLen = jmsg.size() + 1;
    //装载小火车: msgLen + modNum + msg
    string train;
    train.append((char*)&msgLen, (char*)&msgLen + 4);
    train += mod;
    train += jmsg;
    return train;
}

string recvn(SocketIO sio, int len){
    string res;
    int left = len;
    while(left > 0){
        char buf[65535] = {0};
        int ret = sio.readn(buf, left);
        res += buf;
        left -= ret;
    }
    return res;
}

void showKeyRec(string msg){
    json jrecv = json::parse(msg);
    vector<string> result = jrecv.get<vector<string>>();
    cout << "结果" << result.size() << "个 ";
    for(auto& n : result){
        cout << "[" << n << "]" << " ";
    }
    cout << endl;
}

void showWebPage(string msg){
	const char * buff = msg.c_str();

    json recvdata = json::parse(buff);

    map<string, string> titleMap;//id titlte
    map<string, string> summaryMap; //id summary
    map<string, string> contentMap;//id content

    for (json::iterator it = recvdata.begin(); it != recvdata.end(); ++it) 
    {
        string temp = it.key();
        
        if('t' == *temp.begin())
        {
            titleMap.insert(make_pair(it.key(), it.value()));
        }
        if('s' == *temp.begin())
        {
            summaryMap.insert(make_pair(it.key(), it.value()));
        }
        if('c' == *temp.begin())
        {
            contentMap.insert(make_pair(it.key(),it.value()));
        }
        // else{
        //     cout << "no find" << endl;
        //     break;
        // }
        // std::cout << it.key() << " : " << it.value() << "\n";
    }

    for(auto it1 = titleMap.begin(); 
        it1 !=  titleMap.end(); it1++)
    {
        cout << it1->first << "   "<<it1->second << endl;
    }


    // for(auto it1 = titleMap.begin(),  it2 = summaryMap.begin(); 
    //     it1 !=  titleMap.end() && it2 !=  summaryMap.end(); it1++, it2++)
    // {
    //     cout << it1->first << "   "<<it1->second << endl;
    //     cout << it2->first << "   "<<it2->second << endl;
    // }

    cout << "please cin your want doctitle" <<endl;

loop:
    string cinTitle = {0};
    cin >> cinTitle;
    string jsonTitlte = {0};
    auto findTitle = titleMap.find(cinTitle);
    if( findTitle!= titleMap.end())
    {
        // jsonTitlte = doc.substr(titlePos, 5)
        cout << "your search doc title is " << endl << findTitle->second << endl;   
        auto titlePos = (findTitle->first).find("title");     
        if(titlePos != string::npos)
        {
            string titlteNumber = cinTitle.substr(titlePos + 5 , cinTitle.size() - 5);
            cout << titlteNumber << endl;
            string cinContentKey = "content" + titlteNumber;
            cout << cinContentKey << endl;
            auto it = contentMap.find(cinContentKey);
            if(it != contentMap.end())
            {
                cout <<it->first << it->second << endl;
            }
       }
    }
    else
    {
        cout << "your input doctitle is error" << endl;
        cout << "please cin your want doctitle again!" <<endl;
        goto loop;
    }

}

int main(int args, char* argv[]){
    int serverfd =  socket(AF_INET, SOCK_STREAM, 0);
    errorCheck(serverfd, -1, "socket");
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8888);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int ret = connect(serverfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    errorCheck(ret, -1, "connect");
    //连接服务器的socketIO
    SocketIO sio(serverfd);

    while(1){
        cout << "---------------------------------------------------" << endl;
        cout << "< 请输入 0+关键词：关键词推荐，1+关键词：网页搜索 >" << endl;
        //客户键入关键词queryword，只能以0或1开头，否则报错
        string queryWord;
        cin >> queryWord;
        if(!(queryWord[0] == '0' || queryWord[0] == '1')){
            cout << "< 格式错误，请重新输入 >" << endl;
            continue;
        }
        clock_t begin, end;
        begin = clock();
        
        //将queryWord用JSON封包，装载小火车，用tcp发送
        string msgSend = clientSend(queryWord);
        sio.writen(msgSend.c_str(), msgSend.size());

        //卸载服务器发来的小火车，得到01标志位+原生JSON字符串
        char msgLenBuf[4] = {0};
        string msgLenString = recvn(sio, 4);
        strcpy(msgLenBuf, msgLenString.data());
        int msgLen = *(int*)msgLenBuf;
        string msgRecv = recvn(sio, msgLen);

        //返回本次查询时间
        end = clock();
        cout << "查询用时：" << double(end - begin) / CLOCKS_PER_SEC * 1000 << "ms" << endl;

        //根据首标志位，执行对应模块的打印结果函数
        if(msgRecv[0] == '0'){
            showKeyRec(msgRecv.substr(1));
        }else if(msgRecv[0] == '1'){
            showWebPage(msgRecv.substr(1));
        }else{
            cout << "< 服务器查询错误 >" << endl; 
        }
    }
    return 0;
}

