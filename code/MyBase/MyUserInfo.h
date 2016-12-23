#pragma once

#include "Cleaner.h"
#include <string>
#include <fstream>


//-------------------------------------------------
//	用户信息类
//-------------------------------------------------
class MyUserInfo
{
private:
	static std::string usersInfoFile;

private:
	static MyUserInfo* instance;
	
	std::string username;
	std::string password;
	
	//记录当前用户的进度
	std::fstream processFile;

	bool isLogIn;
protected:
	MyUserInfo();

public:
	~MyUserInfo();

	void SetUsername(std::string &username) {
		this->username = username;
	}

	void SetPassword(std::string &password) {
		this->password = password;
	}

	std::string &GetUsername() {
		return username;
	}

	std::string &GetPassword() {
		return password;
	}

	bool IsLogIn() {
		return isLogIn;
	}

	//用户登陆以获取用户信息
	bool LogIn(std::string &un, std::string &pw);					//到时候修改

	//以下函数暂未实现
	bool HasLogInBefore(std::string &un, std::string &fileName);		

	void LogOut();

	//保存下载和上传的进度
	void SaveProcess(std::string &fileName, std::string &path, unsigned long long size, unsigned long long offset);

	//读取上传和下载的进度
	void ReadProcess(std::string &fileName, std::string &path, unsigned long long &size, unsigned long long &offset);

	static MyUserInfo* Instance();
	
	static void Release();
};

