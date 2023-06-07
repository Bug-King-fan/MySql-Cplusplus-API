#include"head.h"
void test8() {
	mysql_library_init(0, 0, 0);
	MYSQL* mysql = mysql_init(0);
	const char* host = "localhost";
	const char* tl_host = "170.0.0.1";
	const char* user = "root";
	const char* pass = "frankwu";
	const char* db = "frank_sql";
	const unsigned int port = 3306;
	const char* unix_socket = 0;

	//CLIENT_MULTI_STATEMENTS 参数代表支持一次执行多条sql语句
	if (!mysql_real_connect(mysql, host, user, pass, db, port, unix_socket, CLIENT_MULTI_STATEMENTS)) {
		std::cout << "链接失败" << mysql_error(mysql) << "\n";
	}
	else {
		std::cout << "链接 " << host << " 成功\n";
	}


	//创建表 
	std::string sql = "create table if not exists `t_image`(\
		`id` int auto_increment,\
		`name` varchar(1024),\
		`path` varchar(2046),\
		`size` int,\
		primary key(`id`)\
		); ";
	sql += "set names gbk; ";
	//清空空间
	sql += "truncate t_image; ";
	//插入
	for (int i = 1; i <= 100; i++) {
		//string流控制 将整形插入字符串中
		std::stringstream ss;
		ss << "insert into `t_image`(`name`,`path`,`size`) values('test";
		ss << i << ".jpg','D://image.text',10240); ";
		sql += ss.str();
	}
	//修改数据
	sql += "update t_image set size=3000 where id = 1; ";
	
	//删除数据
	sql += "delete from t_image where id = 2; ";
	
	//查询数据
	sql += "select count(*) from t_image; ";
	sql += "drop table t_image;";
	int re = mysql_query(mysql, sql.c_str());
	//执行SQL语句立刻返回 但语句并未全部执行好 需要获取结果集
	//把SQL语句整个发送给mysql ，mysql一条条执行，并一条条返回结果
	//单独执行一句SQL语句则不需要获取结果集
	if (re != 0) {
		std::cout << "mysql_query error" << mysql_error(mysql) << "\n";
	}
	//std::cout << sql << "\n";

	//有多个返回结果 
	//需要msql_next_result(MYSQL mysql);获取下一条结果 所以需要用do while执行
	//返回值 0表示有下一条结果 非0 表示没有
	//一次执行多个sql语句必须遍历结果集 并释放需要释放结果集的操作（select）否则会出错  
	do {
		std::cout << "[result] ";
		MYSQL_RES *result = mysql_use_result(mysql);
		if (result) { //select ... 有结果集的
			MYSQL_ROW row;
			std::cout << "select mysql_num_rows = " << mysql_num_rows(result) << "\n";
			while (row = mysql_fetch_row(result)) {
				std::cout << row[0] << "  " << row[1] << "  " << row[2] << "  " << row[3] << "\n";
			}
			mysql_free_result(result);
		}
		else {  //insert update create delete drop ...没有结果集的
			//如果存在字段且没有结果集 则代表select执行出错
			if (mysql_field_count(mysql) > 0 ) { 
				std::cout << "Not retrieve result! " << mysql_error(mysql) << "\n";
			}
			else { //没有字段 insert update create delete drop等执行出错
				//等待服务器的处理结果
				std::cout << mysql_affected_rows(mysql) << " rows affected!" << "\n";
			}
		}
	} while (mysql_next_result(mysql) == 0);

	mysql_close(mysql);
	mysql_library_end();
	std::cout << "MySQL 8.0 API" << "\n";
	getchar();
	return;
}