#include"head.h"
void test7() {
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


	//创建表  不需要清除结果集 没有结果集
	//句尾加 \ 表示宏处理将其链接起来
	std::string sql="create table if not exists `t_image`(\
		`id` int auto_increment,\
		`name` varchar(1024),\
		`path` varchar(2046),\
		`size` int,\
		primary key(`id`)\
		);";

	int re = mysql_query(mysql, sql.c_str());
	if (re != 0) {
		std::cout << "mysql_query() error" << mysql_error(mysql) << "\n";
	}

	sql = "set names gbk";
	mysql_query(mysql, sql.c_str());
	
	for (int i = 1; i <= 1000; i++) {
		//插入数据
		sql = "insert into `t_image`(`name`,`path`,`size`) values('test.jpg','D://image.text//test.jpg', 10240); ";
		
		//string流控制 将整形插入字符串中
		std::stringstream ss;
		ss << "insert into `t_image`(`name`,`path`,`size`) values('test";
		ss << i << ".jpg','D://image.text',10240)";
		sql = ss.str();
		
		re = mysql_query(mysql, sql.c_str());
		
		//查询影响行数
		if (re == 0) {
			int count = mysql_affected_rows(mysql);
			//获取id号 mysql_insert_id(MYSQL mysql);
			std::cout << count << "行受影响" << "id :" << mysql_insert_id(mysql) << "\n";
		}
		else {
			std::cout << "insert error " << mysql_error(mysql) << "\n";
		}
	}
	
	//修改数据

	sql = "update t_image set size=3000 where id = 1";
	re = mysql_query(mysql, sql.c_str());
	if (re == 0) {
		int count = mysql_affected_rows(mysql);
		std::cout << count << "行受影响" << "\n";
	}
	else {
		std::cout << "update error " << mysql_error(mysql) << "\n";
	}
	//特殊用户传map 进行修改 key/values  !!!!!IMPORTANT
	std::map<std::string,std::string>kv;
	kv.insert(std::make_pair("name", "image_update001.png"));
	kv.insert(std::make_pair("size", "5000"));
	std::string where = " where id=2 ";//简单使用
	//拼接字符串
	std::string temp = "";
	for (auto ptr = kv.begin(); ptr != kv.end(); ptr++) {
		temp += "`";
		temp += ptr->first;
		temp += "`='";
		temp += ptr->second;
		temp += "',";
	}
	temp += " id=id ";
	sql = "update t_image set ";
	sql += temp;
	sql += where;
	re = mysql_query(mysql, sql.c_str());
	if (re == 0) {
		int count = mysql_affected_rows(mysql);
		std::cout << count << "行受影响" << "\n";
	}
	else {
		std::cout << "update error " << mysql_error(mysql) << "\n";
	}

	//删除数据
	sql = "truncate t_image";
	re = mysql_query(mysql, sql.c_str());
	if (re != 0) {
		std::cout << "truncate error " << mysql_error(mysql) << "\n";
	}


	mysql_close(mysql);
	mysql_library_end();
	std::cout << "MySQL 8.0 API" << "\n";
	getchar();
	return;
}