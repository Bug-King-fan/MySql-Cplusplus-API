#include"head.h"
void test4() {
	mysql_library_init(0, 0, 0);
	MYSQL* mysql = mysql_init(0); //必须赋初始值
	const char* host = "localhost";
	const char* tl_host = "170.0.0.1";
	const char* user = "root";
	const char* pass = "frankwu";
	const char* db = "book_reader_db";//数据库名称
	const unsigned int port = 3306;//端口
	const char* unix_socket = 0;
	unsigned long clientflag = 0;
	const int to = 5;
	int re3 = mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &to);
	if (re3 != 0) {
		std::cout << "链接超时" << mysql_error(mysql) << "\n";
	}
	//自动重连
	const int recon = 1;
	int re = mysql_options(mysql, MYSQL_OPT_RECONNECT, &recon); //设置自动重连
	if (re != 0) {
		std::cout << "连接失败" << mysql_error(mysql) << "\n";
	}
	//开始链接
	if (!mysql_real_connect(mysql, host, user, pass, db, port, unix_socket, clientflag)) {
		std::cout << "链接失败" << mysql_error(mysql) << "\n";
	}
	else {
		std::cout << "链接 " << host << " 成功\n";
	}
	//不断进行ping查看链接情况
	for (int i = 0; i < 1000; i++) {
		int re2 = mysql_ping(mysql);
		if (re2 == 0) {
			std::cout << host << "连接成功!\n";
		}
		else {
			std::cout << host << "链接失败" << mysql_error(mysql) << "\n";
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));//沉睡1s
		//std::this_thread::sleep_for()使用了C++11的标准库chrono，
		//其中chrono是一种时间抽象。
		//这个库中定义了时间单元（如秒、毫秒、微秒等），
		//可以表示一段时间的长度。std::chrono::seconds就是一个时间长度的类型
		//它表示一个秒数，用来作为sleep_for()函数的参数。
	}
	mysql_close(mysql);
	mysql_library_end();
	getchar();
	return;
}