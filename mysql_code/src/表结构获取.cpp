#include"head.h"
void test6() {
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

	const char* sql = "select * from student";
	mysql_query(mysql, "set names gbk;");
	int re = mysql_real_query(mysql, sql, strlen(sql));
	if (re != 0) {
		std::cout << mysql_error(mysql) << "\n";
	}

	MYSQL_RES* result = mysql_use_result(mysql);
	if (!result) {
		std::cout << mysql_error(mysql) << "\n";
	}

	//获取表的字段名
	MYSQL_FIELD* filed;
	while (filed = mysql_fetch_field(result)) {
		std::cout << "key:" << filed->name << "  ";
	}
	std::cout << "\n";

	MYSQL_ROW  row;
	while (row = mysql_fetch_row(result)) {
		std::cout << row[0] << "   " << row[1] << "       " << row[2] << "       " << row[3] << "         " << row[4] << "\n";
	}

	std::cout << "\n";
	//获取表字段数量
	int number = mysql_num_fields(result);
	std::cout << "属性个数：" << number << "\n";
	
	mysql_free_result(result);
	mysql_close(mysql);
	mysql_library_end();
	getchar();
	return;
}