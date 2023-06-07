#pragma once
#include"DATA.h"
#include "DATA.h"

struct MYSQL;//对结构体进行声明 使调用可以通过编译
//不能直接引用mysql.h 要进行封装！！！
struct MYSQL_RES;

namespace Frank {
	class API src_dll {
	public:
		//MYSQL初始化
		bool Init();

		//清理占用的所有资源
		void Close();

		//数据库连接(不考虑线程安全) flag可设置支持多条语句
		bool Connect(const char* host, const char* user, const char* pass, const char* db, unsigned short port = 3306, unsigned long flag = 0);

		//执行sql语句  if sqllen=0 用strlen获取长度
		bool Query(const char* sql, unsigned long sqllen = 0);

		//Mysql参数设置 connect之前调用 option设置保护 防止调用
		bool SetConnectTimeOut(int sec);
		//设置自动重连 mysql默认不自动重连
		bool SetReconnect(bool isre = true);

		//获取结果集
		bool StoreResult();
		bool UseResult();

		//删除结果集占用空间
		bool FreeResult();

		//获取一行
		std::vector<Data::DATA> FetchRow();

		//插入数据
		bool Insert(Data::INDATA kv, std::string table);
		bool InsertBinary(Data::INDATA kv, std::string table);

		//更新 返回更新数量 失败返回-1
		int Update(Data::INDATA kv, std::string table, std::string where);
		int UpdateBinary(Data::INDATA kv, std::string table, std::string where);

		//事务
		bool StartTransaction();
		bool StopTransaction();
		bool Commit();
		bool RollBack();
		
		//简易接口
		Data::XROWS GetResult(const char* sql);
		 

	protected:
		MYSQL* mysql = 0;
		MYSQL_RES* result = 0;
		bool Option(Data::OPTION opt, const void* arg);
		//生成插入数据的SQL语句
		std::string GetInsertSql(Data::INDATA kv, std::string table);
		std::string GetUpdateSql(Data::INDATA kv, std::string table, std::string where);



	};
}
//属性-》配置属性-》常规-》输出目录: ..\bin 
//属性-》连接器-》高级-》导入库-》..\lib\$(TargetName).lib
//SRCDLL_EXPORTS 配置宏 属性-》C++-》预处理 
