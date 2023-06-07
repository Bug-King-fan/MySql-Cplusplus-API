#include"head.h"
void test2(){
	mysql_library_init(0, 0, 0);
	MYSQL* mysql = mysql_init(0); //必须赋初始值
	const char* host = "localhost";
	const char* tl_host = "170.0.0.1";
	const char* user = "root";
	const char* pass = "frankwu";
	const char* db = "book_reader_db";//数据库名称
	const unsigned int port = 3306;//端口
	const char* unix_socket = 0;
	const unsigned long clientflag = 0;
	//链接登录数据库
//	mysql_real_connect(mysql, host, user, password, database, port, unix_socket, clientflag)
//	连接成功返回true 失败返回false
	if (!mysql_real_connect(mysql, host, user, pass, db, port, unix_socket, clientflag)) {
		std::cout << "链接失败" << mysql_error(mysql) << "\n";
	}
	else {
		std::cout << "链接 " << host << " 成功\n";
	}
	mysql_close(mysql);
	mysql_library_end();
	//会释放所有资源 人为主动清理
	std::cout << "Hello World!\n";
	getchar();
	return;
}