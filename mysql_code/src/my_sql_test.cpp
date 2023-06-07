#include"head.h"
void test9() {
	mysql_library_init(0, 0, 0);
	MYSQL* mysql = mysql_init(0);
	const char* host = "localhost";
	const char* user = "root";
	const char* pass = "frankwu";
	const char* db = "frank_sql";
	const unsigned int port = 3306;
	const char* unix_socket = 0;
	unsigned long clientflag = 0;

	if (mysql_real_connect(mysql, host, user, pass, db, port, unix_socket, clientflag)) {
		std::cout << "链接" << host << "成功!\n";
	}
	else {
		std::cout << "mysql_real_connect error :" << mysql_error(mysql) << "\n";
	}
	
	std::string sql = "set names gbk;";
	mysql_query(mysql, sql.c_str());
	sql = "select * from student";
	int re = mysql_query(mysql, sql.c_str());
	if (re != 0) {
		std::cout << "mysql_query error : " << mysql_error(mysql) << "\n";
	}

	MYSQL_RES *result =  mysql_use_result(mysql);

	MYSQL_FIELD* field;
	while (field = mysql_fetch_field(result)) {
		std::cout << field->name << "         ";
	}
	std::cout << "\n";
	MYSQL_ROW row;
	while (row = mysql_fetch_row(result)) {
		std::cout << row[0] << "    " << row[1] << "          " << row[2] << "           " << row[3] << "             " << row[4] << "\n";
	}
	mysql_free_result(result);
	mysql_close(mysql);
	mysql_library_end();
	getchar();
	return;
}