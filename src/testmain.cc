#include "../include/searchEngineServer.hh"
#include "../include/configuration.hh"
#include "../include/splitToolCppJieBa.hh"
#include "../include/singletonTemplate.hh"
#include "../include/simhash.hh"
#include "../include/redisCache.hh"
#include "../include/myLog.hh"

int main(){
    Configuration* pconf(Singleton<Configuration>::getInstance("../conf/myConf.conf"));
    splitToolCppJieBa* pjieba(Singleton<splitToolCppJieBa>::getInstance());
    SimHash* psim(Singleton<SimHash>::getInstance());

    SearchEngineServer server(*pconf, *pjieba, *psim);

    server.start();

    server.stop();
}































// #include <iostream>
// #include <string>
// #include <map>
// #include <set>

// using std::cout;
// using std::endl;
// using std::string;
// using std::map;
// using std::set;
// using std::ofstream;

// Configuration* pconf(Singleton<Configuration>::getInstance("../conf/myConf.conf"));
// splitToolCppJieBa* pjieba(Singleton<splitToolCppJieBa>::getInstance());
// SimHash* psim(Singleton<SimHash>::getInstance());

// //测试配置类
// void testConf(){

//     auto config_map = pconf->getConfigMap();
//     //string path = config_map["xml_web"];
//     //cout << path << endl;

//     //auto stopWord_list = pconf->getStopWordList();
    
//     //ofstream testfile("testRead.txt");

//     cout << pconf->getConfigMap()["serverIP"] << endl;
//     cout << atoi( pconf->getConfigMap()["serverPort"].c_str()) << endl;
//     cout << atoi( pconf->getConfigMap()["threadNum"].c_str()) << endl;
//     cout << atoi( pconf->getConfigMap()["queSize"].c_str()) << endl;


//         // //测试_configMap内容
//         // for(auto& it : config_map){
//         //     testfile << it.first << " " << it.second << endl;
//         // }

//         // //测试stopwords内容
//         // for(auto& it2 : stopWord_list){
//         //     testfile << it2 << endl;
//         // }

// } 

// //测试分词类
// void testCppjieBa(){
//     //auto splitWords = myJieBa.cut("我今天吃饭");
//     auto splitWords = pjieba->cut("<doc><docid>1</docid><url>../data/xml_web/legal.xml</url><title>全面清理整顿“大学”“学院”名称使用乱象 教育部等八部门出台规范意见</title><content> 人民网北京5月18日电 （记者孙竞）为清理整顿“大学”“学院”名称使用乱象，规范名称登记使用行为，教育部等八部门近日印发了《关于规范“大学”“学院”名称登记使用的意见》（以下简称《意见》）。《意见》指出，除经批准设立的大学、学院以及由其设立的内部机构或由其发起并依法登记的组织机构外，其他组织机构不得在牌匾、广告等对外宣传以及其他各类活动中使用“大学”“学院”字样。  教育部发展规划司负责人表示，近年来，一些企业内设培训机构、社会组织，未经批准冒用“大学”“学院”名称，并对外开展宣传、招生等活动，造成社会公众误解，扰乱了教育秩序，产生了不良影响。根据《高等教育法》及相关法律法规，大学、学院是具有特定功能和作用的法人组织，应按照国家规定的标准和程序审批设立。在未经许可的情况下，各类机构名称或简称均不应使用“大学”“学院”字样。  如何界定“大学”“学院”名称？  《意见》明确“大学”“学院”名称经审批方可使用。大学由国务院教育行政部门依法审批或备案；学院根据办学层次、类型、法人性质等，由中央和国家有关部门或省级人民政府及其有关部门依法审批。  冠“大学”或“学院”、具有独立法人地位的事业单位，涉及机构编制事项的，按照机构编制管理权限和规定程序，报机构编制部门审批。  “大学”“学院”名称登记和使用有哪些规范？  《意见》明确了“大学”“学院”名称的登记规范和使用规范。  经审批设立的大学、学院，以及由其发起并依法设立的其他法人组织，在机构编制、民政、市场监管部门登记时可以对应使用“大学”“学院”字样。其他组织机构在登记时不得使用“大学”“学院”字样。  除经批准设立的大学、学院以及由其设立的内部机构或由其发起并依法登记的组织机构外，其他组织机构不得在牌匾、广告等对外宣传以及其他各类活动中使用“大学”“学院”字样。  企业内设培训机构不得使用“大学”“学院”  教育部发展规划司负责人表示，企业设立的、无需审批登记的内设培训机构一般面向企业内部开展培训活动，但在实际中，部分机构以“大学”对外宣传，并加挂“大学”牌子，有的甚至还对外进行招生培训、开展业务。  对于这类机构，《意见》要求有关部门要指导和督促其规范名称使用行为，不得使用“大学”“学院”字样的名称及简称开展任何形式的宣传等活动。举办机构属于国有企业的，由履行出资人职责的机构负责规范；举办机构属于银行保险机构的，由银行保险监管部门负责规范；其他的由其举办机构的业务主管部门负责规范，无业务主管部门的由教育部门会同市场监管等部门负责规范。  可以使用“XX老年大学”“XX社区学院”等专有名称  《意见》指出，经有关部门批准设立，面向特定人群开展非学历教育培训的老年大学、社区学院，不得单独使用“大学”“学院”字样，可以使用“XX老年大学”“XX社区学院”等专有名称。  教育部发展规划司负责人介绍，《意见》的出台，一方面，兼顾了其他有关部门审批设立的学院，如“干部管理培训学院”“技师学院”“会计学院”等；另一方面，对于因历史原因形成、且具有一定社会认知基础、面向特定人群开展非学历教育培训的老年大学、社区学院，可以继续使用“XX老年大学”“XX社区学院”等专有名称，但不得单独使用“大学”“学院”字样。  相关部门将开展全面摸排和清理整顿  根据《意见》，机构编制、民政、市场监管部门按照上述规定对已登记名称中含有“大学”“学院”字样的组织机构进行摸排清理，审批机关或业务主管部门予以配合。登记名称不符合上述规定的，结合实际，通过组织机构申请变更、审批机关变更、登记机关依职权变更等方式进行规范。  各地各相关部门在《意见》发布后6个月内组织开展清理整顿，对逾期仍违规使用“大学”“学院”字样开展活动的，按照主管和属地原则，由教育、机构编制、人力资源社会保障、民政、市场监管等部门依法责令改正或予以查处。对涉及虚假违法广告或宣传的，由市场监管部门责令停止，并依法处罚；构成违反治安管理行为的，依法给予治安管理处罚；构成犯罪的，依法追究刑事责任。 </content></doc>");
    
//     for(auto& it : splitWords){
//         cout << it << endl;
//     }
// }

// //测试网页库与网页偏移库
// void testWebDat(){
//     DirScanner dir;
//     FileProcessor process(1);

//     PageLib pagelib(dir, process, *pconf);
//     //生成ripepage.dat与offset_table.dat
//     pagelib.store();

// }

// //测试网页库去重与倒排索引库
// void testWebProcess(){
//     PageLibPreProcessor plp(*pconf, *pjieba, *psim);
//     plp.doProcess();    
// }

// //测试中英文词典索引
// void testCnEn(){
//     DictProducer dicp(*pconf, *pjieba);
//     dicp.buildDict();
//     dicp.createIndex();
//     dicp.store();
// }

// //测试网页库查询
// void testWebQuery(){
//     //PageLibPreProcessor plp(*pconf, *pjieba, *psim);
//     //plp.doProcess();    
//     webPageQuery wq(*pjieba, *pconf);
//     string s("造型透过");
//     string result = wq.doQuery(s);

// 	const char * buff = result.c_str();
//     json recvdata = json::parse(buff);

//     for (json::iterator it = recvdata.begin(); it != recvdata.end(); ++it) 
//     {
//         std::cout << it.key() << " : " << it.value() << "\n";
//     }

//     //cout << recvdata << endl;
// }

// //测试日志
// void testLog(){
//     //运行其他函数
// }

// void testRedis(){
//     vector<string> result;
//     string s("hello王道");
//     RedisCache cache("tcp://127.0.0.1:6379");
//     WebPageResearcher  wps(*pjieba,cache);
//     wps.doQuery(s);
//     cout<<"第二次"<<endl;
//     wps.doQuery(s);
// }

// int main(void){

//     //testConf();
//     //testCppjieBa();
//     //testWebDat();
//     //testWebProcess();
//     //testCnEn();
//     //testWebQuery();
//     return 0;
// }

