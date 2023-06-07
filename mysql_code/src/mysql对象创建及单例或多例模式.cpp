#include"head.h"
void test1(){
    //MYSQL mysql;

//  mysql_init(&mysql);//初始化 用于单线程模式，多线程不安全
//  mysql_library_init(0, 0, 0) //库初始化，用于多线程，只需要开头调用一次，结尾释放
    
    mysql_library_init(0, 0, 0);//参数固定0，0，0
    //不产生额外内存开销 固定内存
    
    while (true) {
        //三种单线程初始化和关闭方法
        
        //在不断循环中：    
        //mysql_init(&mysql);
        //不断产生额外内存开销，内存不断增长
        
        //mysql_close(&mysql);
        //每一个mysql_init要对应一个close保证内存不泄露 

        /*
            若mysql是堆上的 需主动delete
            MYSQL *mysql = new MYSQL;
            mysql_init(mysql);
            mysql_close(mysql);
            delete mysql;
        */
        
        /*
            MYSQL *mysql = mysql_init(0);
            mysql_close();
            不需要delete，mysql_init(0)在堆上申请空间，在close里就释放了         
        */
    } 
    mysql_library_end();
    //会释放所有资源 人为主动清理
    std::cout << "Hello World!\n";
    getchar();
}