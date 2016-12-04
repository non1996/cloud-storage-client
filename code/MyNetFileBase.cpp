#include "MyNetFileBase.h"



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

MyNetFileBase::MyNetFileBase(std::string & uId, std::string & name, std::string & path, std::string & date, unsigned long long size)
{
	SetUid(uId);
	SetName(name);
	SetPath(path);
	SetDate(date);
	SetSize(size);
}


MyNetFileBase::~MyNetFileBase()
{
}
