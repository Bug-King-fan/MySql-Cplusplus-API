#include"DATA.h"
#include<iostream>
#include<fstream>
using namespace std;

namespace Data {

#ifndef _WIN32
	static size_t Convert(char* from_cha, char* to_cha, char* in, size_t inlen, char* out, size_t outlen) {
		//转换上下文
		iconv_t cd;
		iconv_open(to_cha, from_cha);
		if (cd == 0) { //打开失败
			return -1;
		}

		memset(out, 0, outlen);
		char** prin = &in;
		char** ou = &out;
		//返回转化字节的数量，但转GBK经常不正确 返回值》=0代表成功
		size_t re = iconv(cd, prin, &inlen, ou, &outlen);

		if (cd != 0) {
			iconv_close(cd);
		}
		return re;
	}
#endif 


	DATA::DATA(const char * data) {
		this->type = MY_TYPE_STRING;
		if (!data) {
			return;
		}
		this->data = data;
		this->size = strlen(data);
	}
	DATA::DATA(const int *d){
		this->type = MY_TYPE_LONG;
		this->data = (char*)d;
		this->size = sizeof(int);
	}
	bool DATA::LoadFile(const char* filename){
		if (!filename) {
			return false;
		}
		fstream in(filename, ios::in | ios::binary);
		if (!in.is_open()) {
			cout << "open " << filename << " error!" << endl;
			return false;
		}
		//文件大小分配空间  
		in.seekg(0, ios::end);
		size = in.tellg();
		in.seekg(0, ios::beg);
		if (size <= 0) {//空文件
			return true;
		}
		data = new char[size];
		int red = 0;
		while (!in.eof()) {
			in.read((char*)data + red, size - red);
			if (in.gcount() > 0) {
				red = in.gcount();
			}
			else {
				break;
			}
		}
		in.close();
		this->type = MY_TYPE_BLOB;
		return true;
	}


	bool DATA::SaveFile(const char* filename){
		if (!data || size <= 0) {
			return false;
		}
		fstream out(filename, ios::out | ios::binary);
		if (!out.is_open()) {
			cout << "savefile error!" << filename << endl;
			return false;
		}
		out.write(data, size);
		out.close();
		return true;
	}
	
	
	void DATA::DropSpace(){
		delete[]data;
		data = NULL;
	}

	std::string DATA::UTF8_TO_GBK(){
		string utf8 = "";
		//1. utf8 -> unicode win:utf16
		// 统计转化后字节数 MultiByteToWideChar(转换格式，转换方式(默认0)，输入的字符串，字符串大小(-1自动计算 找到\0结束)，输出，输出的大小空间)
#ifdef _WIN32
		int len = MultiByteToWideChar(CP_UTF8, 0, data, -1, 0, 0);
		if (len <= 0) {
			return utf8;
		}
		wstring udata;
		udata.resize(len);
		MultiByteToWideChar(CP_UTF8, 0, data, -1, (wchar_t*)udata.data(), len);
		//2. unicode->gbk
		int len2 = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, 0, 0
			, 0 //失败默认替代字符
			, 0//是否使用默认替代
		);
		if (len <= 0) {
			return utf8;
		}
		utf8.resize(len2);
		WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, (char*)utf8.data(), len2, 0, 0);
#else 
		utf8.resize(1024);
		int inlen = strlen(data);
		Convert((char*)"utf-8", (char*)"gbk", (char*)data, inlen, (char*)utf8.data(), utf8.size());
		int outlen = strlen(utf8.data());
		utf8.resize(outlen);

#endif

		return utf8;
	}

	std::string DATA::GBK_TO_UTF8(){
		string gbk = "";
		//GBK->unicode
#ifdef		_WIN32
		int len = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
		if (len <= 0) {
			return gbk;
		}
		wstring udata;
		udata.resize(len);
		MultiByteToWideChar(CP_ACP, 0, data, -1, (wchar_t*)udata.data(), len);

		//UNICODE->UTF8
		int len2 = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, 0, 0
			, 0 //失败默认替代字符
			, 0//是否使用默认替代
		);
		if (len <= 0) {
			return gbk;
		}
		gbk.resize(len2);
		WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, (char*)gbk.data(), len2, 0, 0);
#else
		utf8.resize(1024);
		int inlen = strlen(data);
		Convert((char*)"gbk", (char*)"utf8", (char*)data, inlen, (char*)utf8.data(), utf8.size());
		int outlen = strlen(utf8.data());
		utf8.resize(outlen);
#endif 

		return gbk;
	}

	
}
