#include"head.h"
void test10() {
	mysql_library_init(0, 0, 0);
	MYSQL* mysql = mysql_init(0);
	const char* host = "localhost";
	const char* tl_host = "170.0.0.1";
	const char* user = "root";
	const char* pass = "frankwu";
	const char* db = "frank_sql";
	const unsigned int port = 3306;
	const char* unix_socket = 0;
	unsigned long clientflag = 0;
	if (!mysql_real_connect(mysql, host, user, pass, db, port, unix_socket, clientflag)) {
		std::cout << "链接失败" << mysql_error(mysql) << "\n";
	}
	else {
		std::cout << "链接 " << host << " 成功\n";
	}
	std::string sql = "create table if not exists `t_image`(\
		`id` int auto_increment,\
		`name` varchar(1024),\
		primary key(`id`)\
		) engine=InnoDB;";
	int re = mysql_query(mysql, sql.c_str());
	if (re != 0) {
		std::cout << "ERROE :" << mysql_error(mysql) << "\n";
	}
	//事务
	//并不是每条依次执行 而是全部做完后 提交获取结果 
	//要么全做要么都不做
	//1、开始事务 start transaction
	sql = "start transaction";
	re = mysql_query(mysql, sql.c_str());
	//2、设置手动提交 set autocommit = 0
	sql = "set autocommit = 0";
	re = mysql_query(mysql, sql.c_str());
	//3、执行SQL语句
	//假设插入三条语句后回滚
	
	for (int i = 0; i < 3; i++) {
		sql = "insert into t_image values('three test')";
		mysql_query(mysql, sql.c_str());
	}
	//4、commit提交 / rollback出错回滚
	//不回滚 3 条记录
	//回滚 0 条记录
	sql = "rollback";
	mysql_query(mysql, sql.c_str());
	
	//事务提交
	//重新插入数据
	//记录开始时间
	auto start = std::chrono::system_clock::now();
	 
	for (int i = 0; i < 500000; i++) {
		sql = "insert into t_image(name) values('three test')";
		mysql_query(mysql, sql.c_str());
	}
	sql = "commit";
	mysql_query(mysql, sql.c_str());

	//5、恢复自动提交 set autocommit = 1
	sql = "set autocommit = 1";
	mysql_query(mysql, sql.c_str());
	
	//查看数据个数
	sql = "select count(*) from t_image";
	mysql_query(mysql, sql.c_str());
	MYSQL_RES* result = mysql_use_result(mysql);
	if (result) {
		MYSQL_ROW row = mysql_fetch_row(result);
		if (row) {
			std::cout << "t_image count(*) = " << row[0] << "\n";
		}
	}
	//记录结束时间
	auto end = std::chrono::system_clock::now();
	//换算毫秒
	auto time = std::chrono::duration_cast<std::chrono::milliseconds> (end - start);
	std::cout << "time:" << time.count() / 1000.0 << "秒" << "\n";

	std::cout << "MySQL 8.0 API" << "\n";
	mysql_close(mysql);
	mysql_library_end();
	getchar();
	return;
}