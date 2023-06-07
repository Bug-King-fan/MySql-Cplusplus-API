#include<iostream>
#include<string>
#include<cstring>
#include<cstring>
#ifdef _WIN32
#include<Windows.h>
#else
#include<iconv.h>
#endif // WIN_32

#ifndef _WIN32
static size_t Convert(char* from_cha, char* to_cha, char* in, size_t inlen, char* out, size_t outlen){
	//转换上下文
	iconv_t cd;
	iconv_open(to_cha, from_cha);
	if (cd == 0) { //打开失败
		return -1;
	}

	memset(out, 0, outlen);
	char** prin = &in;
	char** ou = &out;
	cout << in << endl;
	cout << inlen << endl;
	cout << outlen << endl;
	//返回转化字节的数量，但转GBK经常不正确 返回值》=0代表成功
	size_t re = iconv(cd, prin, &inlen, ou, &outlen);

	if (cd != 0) {
		iconv_close(cd);
	}
	cout << (int)re << endl;
	return re;
}
#endif 


using namespace std;

string UTF8_TO_GBK(const char* data) {
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
									,0 //失败默认替代字符
									,0//是否使用默认替代
	);
	if (len <= 0) {
		return utf8;
	}
	utf8.resize(len2);
	WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, (char*)utf8.data(), len2, 0, 0);
#else 
	utf8.resize(1024);
	int inlen = strlen(data);
	cout << inlen << endl;
	Convert((char*)"utf-8", (char*)"gbk", (char*)data, inlen, (char*)utf8.data(), utf8.size());
	int outlen = strlen(utf8.data());
	cout << outlen << endl;
	utf8.resize(outlen);

#endif

	return utf8;
}

string GBK_TO_UTF8(const char* data) {
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
	cout << inlen << endl;
	Convert((char*)"gbk", (char*)"utf8", (char*)data, inlen, (char*)utf8.data(), utf8.size());
	int outlen = strlen(utf8.data());
	cout << outlen << endl;
	utf8.resize(outlen);
#endif 

	return gbk;
}


int main() {
	//utf8-gbk
	cout << UTF8_TO_GBK(u8"测试UTF8-GBK") << endl;
	cout << UTF8_TO_GBK("测试UTF8-GBK") << endl;
	//gbk-utf8
	string s = GBK_TO_UTF8("测试GBK-UTF8-GBK");
	cout << s << endl;
	cout << UTF8_TO_GBK(s.c_str()) << endl;
	return 0;
}