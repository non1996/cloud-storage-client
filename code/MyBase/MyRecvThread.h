#pragma once

#ifndef MYRECVTHREAD_H_
#define MYRECVTHREAD_H_

#include "MyTransmitThreadBase.h"
#include "MyCommandBuffer.h"

//------------------------------------------------
//	被动接收线程
//	接收从服务器发来的消息，同时作为保活连接
//------------------------------------------------
class MyRecvThread :
	public MyTransmitThreadBase
{
private:
	static MyRecvThread* instance;

	MyCommandBuffer* buffer;

	std::string token_one;				//主传输线程验证时用的秘钥
	std::string username;				//用户名
private:
	MyRecvThread();

public:
	virtual ~MyRecvThread();

	bool Init(const char* ipAddr, int port);

	void SetUsername(std::string &username) {
		this->username = username;
	}

	void SetTokenOne(std::string &token) {
		token_one = token;
	}

	//用新收到的秘钥加密用户名和主线程验证的秘钥，用于该线程连接时的验证
	std::string EncodeUT(std::string &ut, unsigned long long usize);

	virtual std::string GetCertificationInfo();

	virtual bool InfoRight();

	bool Communicate();

	virtual void Execute();

	static MyRecvThread* Instance();
	
	static void Release();
};

#endif // !MYRECVTHREAD_H_
