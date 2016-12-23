#pragma once

#ifndef MYRECVTHREAD_H_
#define MYRECVTHREAD_H_

#include "MyTransmitThreadBase.h"
#include "MyCommandBuffer.h"

//------------------------------------------------
//	���������߳�
//	���մӷ�������������Ϣ��ͬʱ��Ϊ��������
//------------------------------------------------
class MyRecvThread :
	public MyTransmitThreadBase
{
private:
	static MyRecvThread* instance;

	MyCommandBuffer* buffer;

	std::string token_one;				//�������߳���֤ʱ�õ���Կ
	std::string username;				//�û���
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

	//�����յ�����Կ�����û��������߳���֤����Կ�����ڸ��߳�����ʱ����֤
	std::string EncodeUT(std::string &ut, unsigned long long usize);

	virtual std::string GetCertificationInfo();

	virtual bool InfoRight();

	bool Communicate();

	virtual void Execute();

	static MyRecvThread* Instance();
	
	static void Release();
};

#endif // !MYRECVTHREAD_H_
