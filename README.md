searchEngine by C++

#环境  
Linux 18.04  
redis-plus-plus  
log4cpp  
cppjieba  
simhash  
nlohmann/json  

#运行程序  
/searchEngine/bin/ 运行main与client  

#编译程序    
#服务器  

```bash
/searchEngine $g++ src/*.cc -o bin/main -lpthread -llog4cpp -lhiredis -lredis++   
```
#客户端  

```bash
/searchEngine $g++ bin/client.cc  src/socketIO.cc -o bin/client   
```
#日志  
终端与log/MyLogRolling.log   

#LRU  
LRUcache/ 可替代Redis  

#离线库  
第一次运行生成data/*.dat文件  

#在线查询  
0* 推荐关键词  
1* 推荐网页 title[0-5] 显示对应网页   
