#pragma once

#ifndef MYFILE_H_
#define MYFILE_H_

#include "Cleaner.h"
#include <fstream>
#include <iostream>


//-------------------------------------
//	��װ��std::fstream���ļ���
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

	bool IsRead() {					//�ļ��Ƕ�����
		return type == INPUT;
	}

	bool IsWrite() {				//�ļ���д����
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

	//��ֻ����ʽ��һ���ļ��������ļ�ָ�����õ��ļ���ͷ
	bool OpenRead();

	//��ֻд��ʽ��һ���ļ��������ļ�ָ�����õ��ļ���ͷ
	bool OpenWrite();

	//�ر��ļ�
	bool Close();

	//���ļ�д��һ���ַ�����
	//��һ������������д��Ļ��棬
	//�ڶ���������ϣ��д����ֽ�����
	//������������ʵ��д����ֽ�����
	//���ĸ�������������ļ���ͷ��ƫ����
	bool Write(const char* buf, unsigned long long expected, unsigned long long &fact, unsigned long long offset);
	
	//���ļ��ж�һ���ַ���
	//��һ�����������ݶ����Ļ��棬
	//�ڶ���������ϣ���������ֽ�����
	//������������ʵ�ʶ������ֽ�����
	//���ĸ�������������ļ���ͷ��ƫ����
	bool Read(char* buf, unsigned long long expected, unsigned long long &fact, unsigned long long offset);
		
	//����һ���ļ���
	static bool MakeDir(const char* name);
	
	//�ж��ļ��Ƿ����
	static bool Exist(const char* name);

	//����һ���Ѵ����ļ��Ĵ�С
	static bool Size(const char* name, unsigned long long &s);
	
	//����һ�����ļ���Ϊ������׼��
	static bool CreateEmptyFile(const char* name, unsigned long long size);
};
#endif // !MYFILE_H_
