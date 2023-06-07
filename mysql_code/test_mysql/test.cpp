#include<iostream>
#include"src_dll.h"

using namespace std;
using namespace Frank;
using namespace Data;
//属性-》调制-》工作目录-》 ..\bin
//C/C++ -》附加包含目录-》..\src_dll
//连接器-》常规-》附加库目录-》..\lib
// 连接器-》输入-》附加依赖项-》添加drc_dll.lib
//解决方案启动项-》单项-》test_mysql
//解决方案启动项-》项目依赖项-》test_mysql 依赖于src_dll 勾选
//常规-》输出路径-》..\bin
int main() {
	src_dll my;
	//初始化
	my.Init();
	//设置超时链接和自动重连
	my.SetConnectTimeOut(3);
	my.SetReconnect(true);
	//链接mysql
	if (my.Connect("localhost", "root", "frankwu", "frank_sql")) {
		cout << "my connect success!" << endl;
	}
	//mysql执行sql语句
	string sql = "create table if not exists `t_video`(\
		`id` int auto_increment,\
		`name` varchar(1024),\
		`data` blob,\
		`size` int,\
		primary key(`id`)\
		); ";
	cout << "mysql query:" << my.Query(sql.c_str()) << endl;

	//测试自动重连
	//while (true) {
	//	cout << "mysql query:" << my.Query(sql.c_str()) << endl;
	//}
	sql = "insert into t_video(name) values('test01')";
	my.Query(sql.c_str());
	my.Query(sql.c_str());
	my.Query(sql.c_str());
	my.Query(sql.c_str());
	my.Query(sql.c_str());

	//清空表
	sql = "truncate t_video";
	my.Query(sql.c_str());
	
	/*
	//插入数据
	Data::INDATA da1;
	//da1.insert({ "name",Data::DATA("test02") });
	//da1.insert({ "size",Data::DATA("1024") });
	//my.Insert(da1, "t_video");
	//插入二进制数据
	da1.insert({ "name",Data::DATA("test01") });
	Data::DATA file;
	file.LoadFile("E:\\mysql_code\\test_mysql\\9.jpg");
	da1.insert({ "data",file });
	da1.insert({ "size",&file.size });
	my.InsertBinary(da1, "t_video"); 
	file.DropSpace();

	//修改id=1的数据
	Data::INDATA da2;
	da2.insert({ "name",Data::DATA("updatenmae001") });
	int affect_number = my.Update(da2, "t_video", " where id=1");
	cout << affect_number << endl;


	//修改二进制数据id=1
	Data::INDATA da3;
	Data::DATA file2;
	file2.LoadFile("9.jpg");
	da3.insert({ "data",file2 });
	affect_number = my.UpdateBinary(da3, "t_video", "where id=1");
	cout << affect_number << endl;
	file2.DropSpace();


	//事务插入多条记录

	Data::INDATA da4;
	da4.insert({ "name",Data::DATA("trans01") });
	da4.insert({ "size",Data::DATA("1024") });
	my.StartTransaction();
	my.Insert(da4, "t_video");
	my.Insert(da4, "t_video");
	my.Insert(da4, "t_video");
	my.RollBack();

	da4.insert({ "name",Data::DATA("trans02") });
	da4.insert({ "size",Data::DATA("1024") });
	my.Insert(da4, "t_video");
	da4.insert({ "name",Data::DATA("trans03") });
	da4.insert({ "size",Data::DATA("1024") });
	my.Insert(da4, "t_video");
	my.Commit();
	my.StopTransaction();


	sql = "select * from t_video";
	my.Query(sql.c_str());
	my.UseResult();
	//获取所有数据
	for (;;) {
		auto row = my.FetchRow();
		if (row.size() == 0) {
			break;
		}

		row[2].SaveFile(row[1].data);

		for (int i = 0; i < row.size(); i++) {
			if (row[i].data) {
				cout << row[i].data << " ";
			}
		}
		cout << endl;
	}
	//请理解结果集
	my.FreeResult();
	*/


	//开始测试字符集 问题：插入读取 GBK UTF8
	cout << "测试字符集：" << endl;
	//utf8 格式
	string sq = "create table if not exists t_utf8( \
		id int auto_increment,name varchar(1024) \
		character set utf8 collate utf8_bin, \
		primary key(id)) ";
	my.Query(sq.c_str());
	sq = "truncate t_utf8";
	my.Query(sq.c_str());
	sq = "set names utf8";
	my.Query(sq.c_str());




	Data::INDATA d;
	d.insert({ "name",u8"测试UTF中文" });
	my.Insert(d, "t_utf8");
	//GBK格式
	sq = "create table if not exists t_gbk( \
		id int auto_increment,name varchar(1024) \
		character set gbk collate gbk_bin, \
		primary key(id)) ";
	my.Query(sq.c_str());
	sq = "truncate t_gbk";
	my.Query(sq.c_str());
	sq = "set names GBK";
	my.Query(sq.c_str());
	Data::INDATA d9;
	d9.insert({ "name","测试GBK中文" });
	my.Insert(d9, "t_gbk");

	cout << "===Print utf8 string===" << endl;
	my.Query("set names utf8;");
	my.Query("select * from t_utf8");
	my.StoreResult();
	for (;;) {
		//获取数据
		auto row = my.FetchRow();
		if (row.size() == 0) {
			break;
		}
#ifdef _WIN32
		cout << "id:" << row[0].data << "name:" << row[1].UTF8_TO_GBK() << endl;
#else
		cout << "id:" << row[0].data << "name:" << row[1].data << endl;
#endif
	}
	my.FreeResult();

	//字符集转换
	cout << "===Print gbk string===" << endl;
	my.Query("set names gbk;");
	my.Query("select * from t_gbk");
	my.StoreResult();
	for (;;) {
		//获取数据
		auto row = my.FetchRow();
		if (row.size() == 0) {
			break;
		}
#ifdef _WIN32
		cout << "id:" << row[0].data << "name:" << row[1].data << endl;
#else
		cout << "id:" << row[0].data << "name:" << row[1].GBK_TOUTF8() << endl;
#endif
	}
	my.FreeResult();

	XROWS rows = my.GetResult("select * from t_video");
	for (int i = 0; i < rows.size(); i++) {
		auto row = rows[i];
		for (int i = 0; i < row.size(); i++) {
			if (row[i].data) {
				cout << "[NULL]" << " ";
				continue;
			}
			switch(row[i].type) {
			case MY_TYPE_BLOB:
				cout << "[BLOB]" << " ";
				break;
			case MY_TYPE_LONG:
			case MY_TYPE_STRING:
			default:
				cout << row[i].data << " ";
				break;
			}
			cout << row[i].data << " ";
		}
	}

	//订票模拟t_tickets(id int, sold int)
	sql = "create table if not exists `t_tickets`(\
			`id` int auto_increment,\
			`sold` int,\
			primary key(`id`)\
			); ";
	my.Query("truncate t_tickets");
	my.Query(sql.c_str());
	{
		Data::INDATA da1;
		da1.insert({ "sold",Data::DATA("0") });
		my.Insert(da1, "t_tickets");
	}
	my.StartTransaction();
	XROWS ro = my.GetResult("select * from t_tickets where sold=0 order by id");
	//my.GetResult("select * from t_tickets where sold=0 for update");
	cout << "Buy ticket id is: " << ro[0][0].data << endl;



//for update 可以根据条件来完成行锁锁定
//并且ID是有索引键的列
//如果ID不是索引键那么InnoDB将完成表锁。
	my.Commit();
	my.StopTransaction();

	//清理mysql
	my.Close();
	system("pause");
	cout << "MySQL 8.0 API" << endl;
	return 0;
}