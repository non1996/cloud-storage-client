#include "MyUserInfo.h"

MyUserInfo* MyUserInfo::instance;
std::string MyUserInfo::usersInfoFile = "userInfo.txt";


MyUserInfo::MyUserInfo()
{
	isLogIn = false;
}


MyUserInfo::~MyUserInfo()
{
}

bool MyUserInfo::LogIn(std::string &un, std::string &pw)
{
	SetUsername(un);
	SetPassword(pw);
	isLogIn = true;
	return true;
}

bool MyUserInfo::HasLogInBefore(std::string & un, std::string & fileName)
{
	return false;
}

void MyUserInfo::LogOut()
{
	isLogIn = false;
}

void MyUserInfo::SaveProcess(std::string & fileName, std::string & path, unsigned long long size, unsigned long long offset)
{
}

void MyUserInfo::ReadProcess(std::string & fileName, std::string & path, unsigned long long & size, unsigned long long & offset)
{
}

MyUserInfo * MyUserInfo::Instance()
{
	if (instance == 0) {
		instance = new MyUserInfo();
	}
	return instance;
}

void MyUserInfo::Release()
{
	Cleaner::Delete<MyUserInfo*>(&instance);
}
