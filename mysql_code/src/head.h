#include <iostream>
#include<cstring>
#include<string>
#include<map>
#include<mysql.h>
#include<sstream> // 字符串流
#include<thread>//线程处理 用到sleep_for
#include<chrono> //C++时间库实现计时
#include<fstream>
// using namespace std::chrono; //涉及命名空间
using namespace std;
//debug release 配置不同
//-》链接器-》输入-》-》附加依赖项编辑动态/静态库 (libmysql.lib;%(AdditionalDependencies)/mysqlclient.lib;%(AdditionalDependencies))

//动态库libmysql.lib 两个模式均可  静态链接库mysqlclient.lib 只能用在release模式

//解决方案属性-》C++常规 -》附加包含目录-》include的位置

//-》链接器-》常规-》附加库目录-》lib位置

//属性-》调试-》工作目录-》libmysql.dll的目录

//以下7个参数需加密处理并单独存放
//const char* host = "localhost";
//const char* tl_host = "170.0.0.1";
//const char* user = "root";
//const char* pass = "frankwu";
//const char* db = "frank_sql";//数据库名称
//const unsigned int port = 3306;//端口
//const char* unix_socket = 0;
//const unsigned long clientflag = 0;

void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();
void test8();
void test9();
void test10();
void test11();

