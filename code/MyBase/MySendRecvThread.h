#pragma once

#ifndef MYSENDRECVTHREAD_H_
#define MYSENDRECVTHREAD_H_

#include "Cleaner.h"
#include "MyTransmitThreadBase.h"
#include "MyCommandBuffer.h"

//------------------------------------------------
//	�������߳�
//	�ӷ��Ͷ�����ȡ�����ݣ������͸�������
//------------------------------------------------
class MySendRecvThread
	:public MyTransmitThreadBase
{
private:
	static MySendRecvThread* instance;	//����ģʽ

	MyCommandBuffer* buffer;			//ά����������е�����
	MyCommand* cmd = 0;	

	bool isQuit;
private:
	MySendRecvThread();

public:
	~MySendRecvThread();

	bool Init(const char* ipAddr, int port);

	virtual std::string GetCertificationInfo();	//�����˸������֤����

	virtual bool InfoRight();				

	bool GetUserInfo();

	bool Communicate();

	virtual void Execute();

	static MySendRecvThread* Instance();
	
	static void Release();

};

#endif // !MySendRecvThread
