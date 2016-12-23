#pragma once

#include "Cleaner.h"
#include <string>
#include <fstream>


//-------------------------------------------------
//	�û���Ϣ��
//-------------------------------------------------
class MyUserInfo
{
private:
	static std::string usersInfoFile;

private:
	static MyUserInfo* instance;
	
	std::string username;
	std::string password;
	
	//��¼��ǰ�û��Ľ���
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

	//�û���½�Ի�ȡ�û���Ϣ
	bool LogIn(std::string &un, std::string &pw);					//��ʱ���޸�

	//���º�����δʵ��
	bool HasLogInBefore(std::string &un, std::string &fileName);		

	void LogOut();

	//�������غ��ϴ��Ľ���
	void SaveProcess(std::string &fileName, std::string &path, unsigned long long size, unsigned long long offset);

	//��ȡ�ϴ������صĽ���
	void ReadProcess(std::string &fileName, std::string &path, unsigned long long &size, unsigned long long &offset);

	static MyUserInfo* Instance();
	
	static void Release();
};

