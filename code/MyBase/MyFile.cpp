#include "MyFile.h"
#include <string>

#include<windows.h>

MyFile::MyFile()
{
	name = 0;
}


MyFile::~MyFile()
{
}

void MyFile::SetName(const char *name)
{
	int len = strlen(name);
	Cleaner::Delete<char*>(&(this->name));
	this->name = new char[len];
	strcpy(this->name, name);
}

bool MyFile::OpenRead()
{
	type = INPUT;
	if (!name) {
		return false;
	}

	file.open(name, std::ios_base::in | std::ios_base::binary);
	file.seekg(0, std::ios_base::beg);
	if (file.is_open()) {
		Size(name, size);
		return true;
	}
	return false;
}

bool MyFile::OpenWrite()
{
	type = OUTPUT;
	if (!name) {
		return false;
	}

	file.open(name, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
	file.seekp(0, std::ios_base::beg);
	if (file.is_open()) {
		return true;
	}
	return false;
}

bool MyFile::Close()
{
	if (file.is_open()) {
		file.close();
		return true;
	}
	return false;
}

bool MyFile::Write(const char *buf)
{
	std::cout << buf << std::endl;
	return true;
}

bool MyFile::Write(const char * buf, unsigned long long expected, unsigned long long & fact, unsigned long long offset)
{
	if(IsRead()){
		return false;
	}
	file.clear();
	file.seekp(offset, std::ios_base::beg);
	if (offset + expected > size) {
		file.write(buf, size - offset);
		fact = size - offset;
	}else{
		file.write(buf, expected);
		fact = expected;
	}
	return true;
}

bool MyFile::Read(char *buf, unsigned long long expected, unsigned long long & fact, unsigned long long offset)
{
	if(IsWrite()){
		return false;
	}

	file.clear();
	file.seekg(offset, std::ios_base::beg);
	file.read(buf, expected);
	fact = file.gcount();
	return true;
}

bool MyFile::Read(const char *buf)
{
	std::cout << "read file\n";
	
	return true;
}

bool MyFile::MakeDir(const char * name)
{
//	std::string mkdir = "mkdir";
//	mkdir += " ";
//	mkdir += name;
//	std::cout << mkdir << std::endl;

//	if (-1 == system(mkdir.c_str())) {
//		std::cout << "创建文件夹失败\n";
//		return false;
//	}
//	std::cout << "创建文件夹成功\n";

	if (0 == CreateDirectoryA(name, NULL)) {
		std::cout << "创建文件夹失败\n";
		return false;
	}
	std::cout << "创建文件夹成功\n";
	return true;
}

bool MyFile::Exist(const char *name)
{
	std::ifstream test;
	test.open(name);
	if (test.is_open()) {
		test.close();
		return true;
	}
	return false;
}

bool MyFile::Size(const char * name, unsigned long long & s)
{
	if (!Exist(name)) {
		return false;
	}
	std::ifstream fin;
	fin.open(name);
	fin.seekg(0, std::ios_base::end);
	s = fin.tellg();
	fin.close();
	return true;
}

bool MyFile::CreateEmptyFile(const char * name, unsigned long long size)
{
	std::ofstream fout;
	if (Exist(name)) {
		std::cout << "文件已存在" << std::endl;
		return false;
	}
	fout.open(name, std::ios_base::ate);
	if (!fout.is_open()) {
		return false;
	}
	if (size == 0) {
		fout.close();
		return true;
	}

	int alreadyW = 0;
	char buf[1000];
	memset(buf, 0, 1000);
	
	if (size <= 1000) {
		fout.write(buf, size);
		fout.close();
		return true;
	}

	while (alreadyW <= size - 1000) {
		fout.write(buf, 1000);
		alreadyW += 1000;
	}
	if (size - alreadyW < 1000) {
		fout.write(buf, size - alreadyW);
	}
	fout.close();
	return true;
}
