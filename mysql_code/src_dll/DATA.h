#pragma once

#ifdef SRCDLL_EXPORTS
//动态链接库调用
#define API __declspec(dllexport)

#else
//执行程序调用
#define API __declspec(dllimport)

#endif 

#include<iostream>
#include<cstring>
#include<string>
#include<map>
#include<vector>
#include<fstream>

#ifdef _WIN32
#include<Windows.h>
#else
#include<iconv.h>
#endif // WIN_32



namespace Data {
	enum OPTION {
		MY_OPT_CONNECT_TIMEOUT,
		MY_OPT_COMPRESS,
		MY_OPT_NAMED_PIPE,
		MY_INIT_COMMAND,
		MY_READ_DEFAULT_FILE,
		MY_READ_DEFAULT_GROUP,
		MY_SET_CHARSET_DIR,
		MY_SET_CHARSET_NAME,
		MY_OPT_LOCAL_INFILE,
		MY_OPT_PROTOCOL,
		MY_SHARED_MEMORY_BASE_NAME,
		MY_OPT_READ_TIMEOUT,
		MY_OPT_WRITE_TIMEOUT,
		MY_OPT_USE_RESULT,
		MY_REPORT_DATA_TRUNCATION,
		MY_OPT_RECONNECT,
		MY_PLUGIN_DIR,
		MY_DEFAULT_AUTH,
		MY_OPT_BIND,
		MY_OPT_SSL_KEY,
		MY_OPT_SSL_CERT,
		MY_OPT_SSL_CA,
		MY_OPT_SSL_CAPATH,
		MY_OPT_SSL_CIPHER,
		MY_OPT_SSL_CRL,
		MY_OPT_SSL_CRLPATH,
		MY_OPT_CONNECT_ATTR_RESET,
		MY_OPT_CONNECT_ATTR_ADD,
		MY_OPT_CONNECT_ATTR_DELETE,
		MY_SERVER_PUBLIC_KEY,
		MY_ENABLE_CLEARTEXT_PLUGIN,
		MY_OPT_CAN_HANDLE_EXPIRED_PASSWORDS,
		MY_OPT_MAX_ALLOWED_PACKET,
		MY_OPT_NET_BUFFER_LENGTH,
		MY_OPT_TLS_VERSION,
		MY_OPT_SSL_MODE,
		MY_OPT_GET_SERVER_PUBLIC_KEY,
		MY_OPT_RETRY_COUNT,
		MY_OPT_OPTIONAL_RESULTSET_METADATA,
		MY_OPT_SSL_FIPS_MODE,
		MY_OPT_TLS_CIPHERSUITES,
		MY_OPT_COMPRESSION_ALGORITHMS,
		MY_OPT_ZSTD_COMPRESSION_LEVEL,
		MY_OPT_LOAD_DATA_LOCAL_DIR,
		MY_OPT_USER_PASSWORD,
		MY_OPT_SSL_SESSION_DATA
	};

	enum FIELD_TYPE {
		MY_TYPE_DECIMAL,
		MY_TYPE_TINY,
		MY_TYPE_SHORT,
		MY_TYPE_LONG,
		MY_TYPE_FLOAT,
		MY_TYPE_DOUBLE,
		MY_TYPE_NULL,
		MY_TYPE_TIMESTAMP,
		MY_TYPE_LONGLONG,
		MY_TYPE_INT24,
		MY_TYPE_DATE,
		MY_TYPE_TIME,
		MY_TYPE_DATETIME,
		MY_TYPE_YEAR,
		MY_TYPE_NEWDATE, 
		MY_TYPE_VARCHAR,
		MY_TYPE_BIT,
		MY_TYPE_TIMESTAMP2,
		MY_TYPE_DATETIME2,  
		MY_TYPE_TIME2,     
		MY_TYPE_TYPED_ARRAY, 
		MY_TYPE_INVALID = 243,
		MY_TYPE_BOOL = 244,
		MY_TYPE_JSON = 245,
		MY_TYPE_NEWDECIMAL = 246,
		MY_TYPE_ENUM = 247,
		MY_TYPE_SET = 248,
		MY_TYPE_TINY_BLOB = 249,
		MY_TYPE_MEDIUM_BLOB = 250,
		MY_TYPE_LONG_BLOB = 251,
		MY_TYPE_BLOB = 252,
		MY_TYPE_VAR_STRING = 253,
		MY_TYPE_STRING = 254,
		MY_TYPE_GEOMETRY = 255
	};

	struct API DATA {
		DATA(const char* data = 0);
		DATA(const int *d);
		bool LoadFile(const char* filename);
		bool SaveFile(const char* filename);
		void DropSpace();
		std::string UTF8_TO_GBK();
		std::string GBK_TO_UTF8();
		FIELD_TYPE type;
		const char* data = 0;
		int size = 0;
	};

	//插入和更新的结构
	typedef std::map<std::string, DATA> INDATA;
	//数据列表
	typedef std::vector<std::vector<DATA>>XROWS;

}

