#include "../include/protocolParser.hh"
#include <fstream>

using std::ostringstream;

ProtocolParser::ProtocolParser(string msg, 
                                KeyRecommander & keyRecommander,
                                WebPageResearcher & webPageResearcher)
: _msg(msg), 
_keyRecommander(keyRecommander), 
_webPageResearcher(webPageResearcher){ } 

string ProtocolParser::doParse(){
    if(_msg[0] == '0'){
        json jrecv = json::parse(_msg.substr(1));
        string msg = jrecv.get<string>(); 
        _keyRecommander.doQuery(msg);
        json jsend(_keyRecommander.response(6));//输出6个关键字
        ostringstream oss;
        oss << jsend;
        string jmsg = oss.str();
        int msgLen = jmsg.size() + 1;
        string train;
        train.append((char*)&msgLen, (char*)&msgLen + 4);
        train += '0';
        train += jmsg;
        return train;
    }
    else if(_msg[0] == '1')
    {
        json j = json::parse(_msg.substr(1));
        string msg = j.get<string>(); 
        //*************msg为客户的关键词****************
        //_webPageResearcher.doQuery(msg);
        
        //*************返回一个存储了结果的jsend********
        ostringstream oss;
        oss << _webPageResearcher.doQuery(msg);
        string jmsg = oss.str();
        int msgLen = jmsg.size() + 1;
        string train;
        train.append((char*)&msgLen, (char*)&msgLen + 4);
        train += '1';
        train += jmsg;
        return train;
    }else{
        //cout << "标志位有误" << endl;
    }
}
