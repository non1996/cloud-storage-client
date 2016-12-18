#pragma once

#ifndef MYFILE_H_
#define MYFILE_H_

#include "Cleaner.h"
#include <fstream>
#include <iostream>

class MyFile
{
	enum {
		INPUT,
		OUTPUT
	};
	
	unsigned long long size;
	char* name;

	std::fstream file;

	int type;
public:
	MyFile();
	~MyFile();

	bool IsRead() {					//文件是读类型
		return type == INPUT;
	}
	bool IsWrite() {				//文件是写类型
		return type == OUTPUT;
	}

	void SetName(const char*);	
	void SetSize(unsigned long long size) {  //设置文件大小，
		this->size = size;
	}
	const char* GetName() {
		return name;
	}
	unsigned long long GetSize() {
		return size;
	}

	bool OpenRead();
	bool OpenWrite();
	bool Close();

	bool Write(const char* buf, unsigned long long expected, unsigned long long &fact, unsigned long long offset);
	//第一个参数是数据写入的缓存，第二个参数是希望写入的字节数，第三个参数是实际写入的字节数，第四个参数是相对于文件开头的偏移量

	bool Read(char* buf, unsigned long long expected, unsigned long long &fact, unsigned long long offset);
	//第一个参数是数据读出的缓存，第二个参数是希望读出的字节数，第三个参数是实际读出的字节数，第四个参数是相对于文件开头的偏移量
	
	
	static bool MakeDir(const char* name);
	static bool Exist(const char* name);
	static bool Size(const char* name, unsigned long long &s);
	//计算一个已存在文件的大小
	static bool CreateEmptyFile(const char* name, unsigned long long size);
	//创建一个空文件，为下载做准备
};

#endif // !MYFILE_H_
