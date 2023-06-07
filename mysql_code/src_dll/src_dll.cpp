#include"src_dll.h"
#include"mysql.h"
//C++常规-》附加包含目录-》..\include 包含mysql头文件
//连接器-》常规-》附加库目录-》 ..\lib
//连接器-》输入-》附加依赖项-》libmysql.lib 动态链接库
#include<iostream>
using namespace std;

namespace Frank {
	bool src_dll::Init() {
		//每次初始化的时候都需要先关闭重新生成
		Close();//防止内存泄漏
		cout << "src_dll::Init()" << endl;
		//新创建一个mysql对象
		mysql = mysql_init(0);
		if (!mysql) {
			cout << "mysql init error: " << mysql_error(mysql) << endl;
			return false;
		}
		return true;
	}


	void src_dll::Close() {
		FreeResult();
		if (mysql) {
			mysql_close(mysql);
			mysql = NULL;
		}
		cout << "src_dll::Close()" << endl;
	}


	bool src_dll::Connect(const char* host, const char* user, const char* pass, const char* db, unsigned short port, unsigned long flag) {
		if (!mysql) { //如果没有mysql再初始化
			if (!Init()) { //有这个接口 用户可以自己调用init
				cout << "connect error: " << mysql_error(mysql) << endl;
				return false;
			}
		}
		if (!mysql_real_connect(mysql, host, user, pass, db, port, 0, flag)) {
			cout << "connect error :" << mysql_error(mysql) << endl;
			return false;
		}
		cout << "connect succcess!" << endl;

		return true;
	}


	bool src_dll::Query(const char* sql, unsigned long sqllen) {
		if (!mysql) {
			cout << "query error:" << mysql_error(mysql) << endl;
			return false;
		}
		if (!sql) {
			cout << "SQL is NULL!" << endl;
			return false;
		}
		if (sqllen <= 0) {
			sqllen = (unsigned long)strlen(sql);
		}
		if (sqllen <= 0) {
			cout << "SQLLEN is empty!" << endl;
			return false;
		}
		int re = mysql_real_query(mysql, sql, sqllen);
		if (re != 0) {
			cout << "query error:" << mysql_error(mysql) << endl;
			return false;
		}
		return true;
	}


	bool src_dll::Option(Data::OPTION opt, const void* arg) {
		if (!mysql) {
			cout << "mysql is null!" << mysql_error(mysql) << endl;
			return false;
		}
		int re = mysql_options(mysql, (mysql_option)opt, arg);
		if (re != 0) {
			cout << "*mysql_options error:*" << mysql_error(mysql) << endl;
			return false;
		}
		return true;
	}


	bool src_dll::SetConnectTimeOut(int sec) {
		return Option(Data::MY_OPT_CONNECT_TIMEOUT, &sec);
	}


	bool src_dll::SetReconnect(bool isre) {
		return Option(Data::MY_OPT_RECONNECT, &isre);
	}


	bool src_dll::StoreResult() {
		if (!mysql) {
			cout << "mysql is null !" << mysql_error(mysql) << endl;
			return false;
		}
		FreeResult();
		result = mysql_store_result(mysql);
		if (!result) {
			cout << "mysql_store_result error:" << mysql_error(mysql) << endl;
			return false;
		}
		return true;
	}


	bool src_dll::UseResult() {
		if (!mysql) {
			cout << "mysql is null !" << mysql_error(mysql) << endl;
			return false;
		}
		FreeResult();
		result = mysql_use_result(mysql);
		if (!result) {
			cout << "mysql_use_result error:" << mysql_error(mysql) << endl;
			return false;
		}
		return true;
	}


	bool src_dll::FreeResult() {
		if (result) {
			mysql_free_result(result);
			result = NULL;
		}
		return true;
	}


	std::vector<Data::DATA> src_dll::FetchRow() {
		std::vector<Data::DATA> fe;
		if (!result) { //空vector
			return fe;
		}
		MYSQL_ROW row = mysql_fetch_row(result);
		if (!row) {//没有
			return fe;
		}
		int number = mysql_num_fields(result);//列
		unsigned long* lens = mysql_fetch_lengths(result);
		for (int i = 0; i < number; i++) {
			Data::DATA da;
			da.data = row[i];
			da.size = lens[i];
			auto ans = mysql_fetch_field_direct(result, i);
			da.type = (Data::FIELD_TYPE)ans->type;
			fe.push_back(da);
		}
		return fe;
	}


	std::string src_dll::GetInsertSql(Data::INDATA kv, std::string table){
		std::string sql2 = "";
		if (kv.empty() || table.empty()) {
			return "";
		}
		sql2 = "insert into ";
		sql2 += table;
		string key = "";
		string value = "";

		//map的迭代器
		for (auto ptr = kv.begin(); ptr != kv.end(); ptr++) {
			key += "`";
			key += ptr->first;
			key += "`,";
			value += "'";
			value += ptr->second.data;
			value += "',";
		}
		key[key.size() - 1] = ' ';
		value[value.size() - 1] = ' ';
		
		sql2 += "(";
		sql2 += key;
		sql2 += ") values(";
		sql2 += value;
		sql2 += ")";
		return sql2;
	}

	std::string src_dll::GetUpdateSql(Data::INDATA kv, std::string table, std::string where){
		string sql2 = "";
		if (kv.empty() || table.empty()) {
			return "";
		}
		sql2 += "update `";
		sql2 += table;
		sql2 += "` set ";
		for (auto ptr = kv.begin(); ptr != kv.end(); ptr++) {
			sql2 += "`";
			sql2 += ptr->first;
			sql2 += "`='";
			sql2 += ptr->second.data;
			sql2 += "',";
		}
		sql2[sql2.size() - 1] = ' ';
		sql2 += where;
		return sql2;
	}


	bool src_dll::Insert(Data::INDATA kv, std::string table){
		string sql = GetInsertSql(kv, table);
		if (sql.empty()) {
			return false;
		}
		if (!Query(sql.c_str())) {
			return false;
		}
		int num = mysql_affected_rows(mysql);
		if (num <= 0) {
			return false;
		}
		return true;
	}

	bool src_dll::InsertBinary(Data::INDATA kv, std::string table){
		std::string sql2 = "";
		if (kv.empty() || table.empty() || !mysql) {
			return false;
		}
		sql2 = "insert into ";
		sql2 += table;
		string key = "";
		string value = "";
		//绑定字段
		MYSQL_BIND bind[128] = { 0 };
		int i = 0;
		//map的迭代器
		for (auto ptr = kv.begin(); ptr != kv.end(); ptr++) {
			key += "`";
			key += ptr->first;
			key += "`,";
			value += "?,";
			bind[i].buffer = (char*)ptr->second.data;
			bind[i].buffer_length = ptr->second.size;
			bind[i].buffer_type = (enum_field_types)ptr->second.type;
			i++;
		}
		key[key.size() - 1] = ' ';
		value[value.size() - 1] = ' ';

		sql2 += "(";
		sql2 += key;
		sql2 += ") values(";
		sql2 += value;
		sql2 += ")";

		MYSQL_STMT* stmt = mysql_stmt_init(mysql);
		if (!stmt) {
			cout << "mysql stmt_init error:" << mysql_error(mysql) << endl;
			return false;
		}
		
		if (mysql_stmt_prepare(stmt, sql2.c_str(), sql2.length()) != 0) {
			mysql_stmt_close(stmt);
			cout << "mysql stmt_prepare error:" << mysql_error(mysql) << endl;
			return false;
		}

		if (mysql_stmt_bind_param(stmt, bind) != 0) {
			mysql_stmt_close(stmt);
			cout << "mysql stmt_bind_param error:" << mysql_stmt_error(stmt) << endl;
			return false;
		}

		if (mysql_stmt_execute(stmt) != 0) {
			mysql_stmt_close(stmt);
			cout << "mysql stmt_execute error:" << mysql_stmt_error(stmt) << endl;
			return false;
		}

		mysql_stmt_close(stmt);

		return true;
	}

	int src_dll::Update(Data::INDATA kv, std::string table, std::string where){
		if (!mysql) {
			return -1;
		}
		std::string sql = GetUpdateSql(kv, table, where);
		if (sql.empty()) {
			return -1;
		}
		if (!Query(sql.c_str())) {
			return -1;
		}
		
		return mysql_affected_rows(mysql);
	}

	int src_dll::UpdateBinary(Data::INDATA kv, std::string table, std::string where){
		if (!mysql || kv.empty() || table.empty()) {
			return -1;
		}
		string sql2 = "";
		sql2 += "update `";
		sql2 += table;
		sql2 += "` set ";
		MYSQL_BIND bind[128] = { 0 };
		int i = 0;
		for (auto ptr = kv.begin(); ptr != kv.end(); ptr++) {
			sql2 += "`";
			sql2 += ptr->first;
			sql2 += "`=?,";
			bind[i].buffer = (char*)ptr->second.data;
			bind[i].buffer_length = ptr->second.size;
			bind[i].buffer_type = (enum_field_types)ptr->second.type;
			i++;
		}
		sql2[sql2.size() - 1] = ' ';
		sql2 += where;

		MYSQL_STMT* stmt = mysql_stmt_init(mysql);
		if (!stmt) {
			cout << "mysql stmt_init error:" << mysql_error(mysql) << endl;
			return -1;
		}

		if (mysql_stmt_prepare(stmt, sql2.c_str(), sql2.length()) != 0) {
			mysql_stmt_close(stmt);
			cout << "mysql stmt_prepare error:" << mysql_error(mysql) << endl;
			return -1;
		}

		if (mysql_stmt_bind_param(stmt, bind) != 0) {
			mysql_stmt_close(stmt);
			cout << "mysql stmt_bind_param error:" << mysql_stmt_error(stmt) << endl;
			return -1;
		}

		if (mysql_stmt_execute(stmt) != 0) {
			mysql_stmt_close(stmt);
			cout << "mysql stmt_execute error:" << mysql_stmt_error(stmt) << endl;
			return -1;
		}
		mysql_stmt_close(stmt);

		return mysql_stmt_affected_rows(stmt);
	}

	bool src_dll::StartTransaction(){
		return Query("set autocommit = 0");
	}

	bool src_dll::StopTransaction(){
		return Query("set autocommit = 1"); 
	}

	bool src_dll::Commit(){
		return Query("commit");
	}

	bool src_dll::RollBack(){
		return Query("rollback");
	}

	Data::XROWS src_dll::GetResult(const char* sql){
		FreeResult();
		Data::XROWS rows;
		if (!Query(sql)) {
			return rows;
		}
		if (!StoreResult()) {
			return rows;
		}
		for (;;) {
			auto row = FetchRow();
			if (row.empty()) {
				break;
			}
			rows.push_back(row);
		}
		return rows;
	}
	



}