#pragma once

#ifndef MYFILE_H_
#define MYFILE_H_

#include "Cleaner.h"
#include <fstream>
#include <iostream>


//-------------------------------------
//	封装了std::fstream的文件类
//-------------------------------------
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

	void SetSize(unsigned long long size) {
		this->size = size;
	}

	const char* GetName() {
		return name;
	}

	unsigned long long GetSize() {
		return size;
	}

	//以只读方式打开一个文件，并将文件指针设置到文件开头
	bool OpenRead();

	//以只写方式打开一个文件，并将文件指针设置到文件开头
	bool OpenWrite();

	//关闭文件
	bool Close();

	//向文件写入一个字符串，
	//第一个参数是数据写入的缓存，
	//第二个参数是希望写入的字节数，
	//第三个参数是实际写入的字节数，
	//第四个参数是相对于文件开头的偏移量
	bool Write(const char* buf, unsigned long long expected, unsigned long long &fact, unsigned long long offset);
	
	//从文件中读一个字符串
	//第一个参数是数据读出的缓存，
	//第二个参数是希望读出的字节数，
	//第三个参数是实际读出的字节数，
	//第四个参数是相对于文件开头的偏移量
	bool Read(char* buf, unsigned long long expected, unsigned long long &fact, unsigned long long offset);
		
	//创建一个文件夹
	static bool MakeDir(const char* name);
	
	//判断文件是否存在
	static bool Exist(const char* name);

	//计算一个已存在文件的大小
	static bool Size(const char* name, unsigned long long &s);
	
	//创建一个空文件，为下载做准备
	static bool CreateEmptyFile(const char* name, unsigned long long size);
};
#endif // !MYFILE_H_
