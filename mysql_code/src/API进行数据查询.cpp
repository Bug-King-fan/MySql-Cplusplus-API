#include"head.h"
void test5() {
	//获取中文数据结果显示乱码

	mysql_library_init(0, 0, 0);
	MYSQL* mysql = mysql_init(0); //必须赋初始值
	const char* host = "localhost";
	const char* tl_host = "170.0.0.1";
	const char* user = "root";
	const char* pass = "frankwu";
	const char* db = "frank_sql";
	const unsigned int port = 3306;
	const char* unix_socket = 0;
	unsigned long clientflag = 0;

	//开始链接
	if (!mysql_real_connect(mysql, host, user, pass, db, port, unix_socket, clientflag)) {
		std::cout << "链接失败" << mysql_error(mysql) << "\n";
	}
	else {
		std::cout << "链接 " << host << " 成功\n";
	}

	std::cout << "\n";

	//select * from student
	//1、执行SQL语句
	const char* sql = "select * from sc";
	//mysql_real_query(mysql,sql,lenth); sql语句中可包含二进制数据
	//mysql_query(mysql,sql); sql语句中只能是字符串
	//返回0表示成功
	/*我的语句案例*/
	//1、解决中文乱码（临时表改格式 下一次数据库还是原来的格式）
	//int rp = mysql_query(mysql, "SET NAMES GBK;");
	int rp = mysql_real_query(mysql, "SET NAMES GBK;",(unsigned long)strlen("SET NAMES GBK;"));
	if (rp != 0) {
		std::cout << mysql_error(mysql) << "\n";
	}
	mysql_query(mysql, "select * from student");
	MYSQL_RES* myans = mysql_use_result(mysql);
	MYSQL_ROW my_row;
	while (my_row = mysql_fetch_row(myans)) {
		//std::cout << my_row[0] << " " << my_row[1] << " " << my_row[2] << "\n";
		std::cout << my_row[0] << " " << my_row[1] << " " << my_row[2] << " " << my_row[3] << " " << my_row[4] << "\n";
	}
	mysql_free_result(myans);
	//结束

	std::cout << "--------------------------------------------------\n";

	int re = mysql_real_query(mysql, sql, strlen(sql));
	if (re != 0) {
		std::cout << mysql_error(mysql) << "\n";
	}
	//执行sql语句后必须 获取结果集 并 清理

	//2、获取结果集
	//mysql_use_result(mysql); 返回一个MYSQL_RES结果集 不实际读取数据 遍历时会与mysql交互
	//mysql_store_result(mysql); 将数据库的所有内容读取到缓冲池，遍历时不交互 要注意缓存大小 默认64MB 通过MYSQL_OPT_MAX_ALLOWED_PACKET设置	
	MYSQL_RES* result = mysql_use_result(mysql);
	if (!result) {
		std::cout << mysql_error(mysql) << "\n";
	}

	//3、遍历结果集
	//mysql_fetch_row(mysql_res result); 结束无数据了返回NULL
	MYSQL_ROW row; //存放着数据 是mysql_fetch_row的返回类型
	std::cout << "   学号     " << " " << " 课程号 " << " " << " 成绩" << "\n";
	while (row = mysql_fetch_row(result)) {
	//	if (row == NULL) {
		//	break;
		//}
		unsigned long* len = mysql_fetch_lengths(result);
		//每列的长度
	// 	std::cout << "每列数据长度" << len[0] << " " << len[1] << " " << len[2] << "\n";
		std::cout  << "[" << row[0] << "      " << row[1] << "       " << row[2] << "    ]" << "\n";
		//此处len[0]是学号长度 len[1]是第二列长度....
		std::cout << "\n\n";
	}

	//4、清理结果集
	mysql_free_result(result);

	mysql_close(mysql);
	mysql_library_end();
	getchar();
	return;
}