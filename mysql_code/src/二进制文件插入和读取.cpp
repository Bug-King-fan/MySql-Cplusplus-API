#include"head.h"
void test11() {
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
	//1、链接
	if (!mysql_real_connect(mysql, host, user, pass, db, port, unix_socket, clientflag)) {
		cout << "链接失败" << mysql_error(mysql) << "\n";
	}
	else {
		cout << "链接 " << host << " 成功\n";
	}

	//2、创建二进制数据表	
	string sql = "create table if not exists `t_data`(\
		`id` int auto_increment,\
		`name` varchar(1024),\
		`data` blob,\
		`size` int,\
		primary key(`id`)\
		)";
	int re = mysql_query(mysql, sql.c_str());
	if (re != 0) {
		cout << mysql_error(mysql) << endl;
	}

	//写入
	//3.创建二进制操作对象
	MYSQL_STMT* stmt = mysql_stmt_init(mysql);

	//4.预处理sql语句
	sql = "insert into t_data (`name`,`data`,`size`) values(?,?,?)";
	if (mysql_stmt_prepare(stmt, sql.c_str(), sql.size())) {
		std::cerr << "mysql_stmt_prepare error:" << mysql_stmt_error(stmt) << "\n";
	}

	//5.打开并读取文件
	string filename = "E:\\mysql_code\\src\\9.jpg";

	//获取文件大小
	int filesize = 0;

	//读二进制文件
	fstream ism(filename, ios::in | ios::binary);
	if (!ism.is_open()) {
		cout << "未打开" << endl;
	}
	//文件指针移到结尾
	ism.seekg(0, ios::end);
	//计算文件大小
	filesize = ism.tellg();
	//文件移动回起始
	ism.seekg(0, ios::beg);

	char* data = new char[filesize];

	int read = 0; //读了多少
	while (!ism.eof()) {
		ism.read(data + read, filesize - read);
		if (ism.gcount() <= 0) {
			break; //读完了
		}
		read += ism.gcount();//记录读了多少字节
	}
	//6.绑定字段
	MYSQL_BIND bind[3] = { 0 };
	bind[0].buffer_type = MYSQL_TYPE_STRING; //第一个字段是name 文件名
	bind[0].buffer = (char*)filename.c_str();
	bind[0].buffer_length = filename.size();

	bind[1].buffer_type = MYSQL_TYPE_BLOB; //二进制文件
	bind[1].buffer = data;
	bind[1].buffer_length = filesize;

	bind[2].buffer_type = MYSQL_TYPE_LONG; //size 文件大小
	bind[2].buffer = &filesize;
	bind[2].buffer_length = filesize;

	if (mysql_stmt_bind_param(stmt, bind)) {
		cout << "BIND_PARAM ERROR" << mysql_stmt_error(stmt) << endl;
	}

	//7、执行stmt_sql
	if (mysql_stmt_execute(stmt) != 0) {
		cout << mysql_stmt_error(stmt) << endl;
	}

	delete[]data;
	//8、关闭stmt
	mysql_stmt_close(stmt);

	
	//查询二进制数据并保存为文件
	sql = "select * from t_data limit 1";
	re = mysql_query(mysql, sql.c_str());
	if (re != 0) {
		cout << "query error :" << mysql_error(mysql) << endl;
	}

	MYSQL_RES* result = mysql_use_result(mysql);
	MYSQL_ROW row;
	row = mysql_fetch_row(result);
	cout << row[0] << " " << row[1] << " " << row[2] << endl;

	//获取每列大小
	unsigned long* fentch = mysql_fetch_lengths(result);
	int number = mysql_num_fields(result);
	for (int i = 0; i < number; i++) {
		cout << "[" << fentch[i] << "]" << endl;
	}
	//写入文件夹
	string name = "out.jpg";
	fstream out(name, ios::out | ios::binary);
	if (!out.is_open()) {
		cout << "写入不成功" << endl;
	}
	if (row) {
		out.write(row[2], fentch[2]);
	}
	out.close();
	

	mysql_close(mysql);
	mysql_library_end();
	cout << "MySQL 8.0.3 API" << endl;
	getchar();
	return;
}