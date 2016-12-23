#pragma once

#ifndef MYNETFILEBASE_H_
#define MYNETFILEBASE_H_

#include "Cleaner.h"
#include <string>

//-------------------------------------------------------
//	网盘文件基类，保存网盘文件的基本信息
//-------------------------------------------------------
class MyNetFileBase
{
private:
	std::string uId;			//文件在服务器上的编号
	std::string name;			//文件名
	std::string path;			//文件所在的网盘路径
	std::string date;			//文件的创建日期
	unsigned long long size;	//文件大小（文件夹大小均为0）
	bool shared;				//文件是否共享
public:
	MyNetFileBase();
	MyNetFileBase(std::string &name, std::string &path, std::string &date, unsigned long long size);
	MyNetFileBase(std::string &uId, std::string &path, std::string &name, std::string &date, unsigned long long size, bool share);
	virtual ~MyNetFileBase();

	virtual std::string toString();

	void SetUid(std::string &uId) {
		this->uId = uId;
	}

	void SetName(std::string &name) {
		this->name = name;
	}

	void SetPath(std::string &path) {
		this->path = path;
	}

	void SetDate(std::string &date) {
		this->date = date;
	}

	void SetSize(unsigned long long size) {
		this->size = size;
	}

	void SetShared(bool shared) {
		this->shared = shared;
	}

	std::string & GetUid() {
		return uId;
	}

	std::string & GetName() {
		return name;
	}

	std::string & GetPath() {
		return path;
	}

	std::string &GetDate() {
		return date;
	}

	virtual std::string GetType() = 0;

	unsigned long long GetSize() {
		return size;
	}

	virtual MyNetFileBase* Copy() = 0;
};

//----------------------------------------------------
//	网盘文件类
//----------------------------------------------------
class MyNetFileNorm 
	:public MyNetFileBase{
private:
	
public:
	MyNetFileNorm() 
		:MyNetFileBase(){

	}
	
	MyNetFileNorm(std::string &uId, std::string &path, std::string &name, std::string &date, unsigned long long size, bool shared)
		:MyNetFileBase(uId, path, name, date, size, shared) {

	}
	
	virtual std::string GetType() {
		return "file";
	}

	virtual MyNetFileBase* Copy() {
		return new MyNetFileNorm(*this);
	}
};

//----------------------------------------------------
//	网盘文件夹类
//----------------------------------------------------
class MyNetFileDir
	:public MyNetFileBase{
private:

public:
	MyNetFileDir()
		:MyNetFileBase() {

	}

	MyNetFileDir(std::string &uId, std::string &path, std::string &name, std::string &date, unsigned long long size, bool shared)
		:MyNetFileBase(uId, path, name, date, size, shared) {

	}

	virtual std::string GetType() {
		return "dir";
	}

	virtual MyNetFileBase* Copy() {
		return new MyNetFileDir(*this);
	}
};
#endif // !MYNETFILEBASE_H_


