#include"head.h"
void test3() {
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
	//链接超时设置  设定超时5秒
	const int to = 5;
	int re = mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &to); // ?have error
	//mysql_options(MYSQL* m,enum mysql_option（设置类型），const void *value)；
/*
enum mysql_option{
MYSQL_OPT_CONNECT_TIMEOUT, MYSQL_OPT_COMPRESS, MYSQL_OPT_NAMED_PIPE,
MYSQL_INIT_COMMAND, MYSQL_READ_DEFAULT_FILE, MYSQL_READ_DEFAULT_GROUP,
MYSQL_SET_CHARSET_DIR, MYSQL_SET_CHARSET_NAME, MYSQL_OPT_LOCAL_INFILE,
MYSQL_OPT_PROTOCOL, MYSQL_SHARED_MEMORY_BASE_NAME, MYSQL_OPT_READ_TIMEOUT,
MYSQL_OPT_WRITE_TIMEOUT, MYSQL_OPT_USE_RESULT,
MYSQL_OPT_USE_REMOTE_CONNECTION, MYSQL_OPT_USE_EMBEDDED_CONNECTION,
MYSQL_OPT_GUESS_CONNECTION, MYSQL_SET_CLIENT_IP, MYSQL_SECURE_AUTH,
MYSQL_REPORT_DATA_TRUNCATION, MYSQL_OPT_RECONNECT,
MYSQL_OPT_SSL_VERIFY_SERVER_CERT};
*/
	if (re != 0) {
		std::cout << "链接超时" << mysql_error(mysql) << "\n";
	}
	if (!mysql_real_connect(mysql, tl_host, user, pass, db, port, unix_socket, clientflag)) {
		std::cout << "链接失败" << mysql_error(mysql) << "\n";
	}
	else {
		std::cout << "链接 " << tl_host << " 成功\n";
	}
	mysql_close(mysql);
	mysql_library_end();
	getchar();
	return;
}