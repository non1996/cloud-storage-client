#include "MyNetFileBase.h"
#include <sstream>


MyNetFileBase::MyNetFileBase()
{
	size = 0;
}

MyNetFileBase::MyNetFileBase(std::string & name, std::string & path, std::string & date, unsigned long long size)
{
	SetName(name);
	SetPath(path);
	SetDate(date);
	SetSize(size);
}

MyNetFileBase::MyNetFileBase(std::string &uId, std::string &path, std::string &name, std::string &date, unsigned long long size, bool share)
{
	SetUid(uId);
	SetName(name);
	SetPath(path);
	SetDate(date);
	SetSize(size);
	SetShared(shared);
}


MyNetFileBase::~MyNetFileBase()
{
}

std::string MyNetFileBase::toString()
{
	std::stringstream ss;
	ss	<< "�ļ���:" << name
		<< "\n�ļ�·��: " << path
		<< "\n��������: " << date
		<< "\n��С:" << size;
	return ss.str();
}

std::string MyNetFileNorm::toString()
{
	std::stringstream ss;
	ss << "�ļ���Ϣ: \n" << MyNetFileBase::toString();
	return ss.str();
}

std::string MyNetFileDir::toString()
{
	std::stringstream ss;
	ss << "�ļ�����Ϣ: \n" << MyNetFileBase::toString();
	return ss.str();
}
