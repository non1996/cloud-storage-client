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

	bool IsRead() {					//�ļ��Ƕ�����
		return type == INPUT;
	}
	bool IsWrite() {				//�ļ���д����
		return type == OUTPUT;
	}

	void SetName(const char*);	
	void SetSize(unsigned long long size) {  //�����ļ���С��
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
	//��һ������������д��Ļ��棬�ڶ���������ϣ��д����ֽ�����������������ʵ��д����ֽ��������ĸ�������������ļ���ͷ��ƫ����

	bool Read(char* buf, unsigned long long expected, unsigned long long &fact, unsigned long long offset);
	//��һ�����������ݶ����Ļ��棬�ڶ���������ϣ���������ֽ�����������������ʵ�ʶ������ֽ��������ĸ�������������ļ���ͷ��ƫ����
	
	
	static bool MakeDir(const char* name);
	static bool Exist(const char* name);
	static bool Size(const char* name, unsigned long long &s);
	//����һ���Ѵ����ļ��Ĵ�С
	static bool CreateEmptyFile(const char* name, unsigned long long size);
	//����һ�����ļ���Ϊ������׼��
};

#endif // !MYFILE_H_
