#pragma once

#ifndef MYSENDRECVTHREAD_H_
#define MYSENDRECVTHREAD_H_

#include "Cleaner.h"
#include "MyTransmitThreadBase.h"
#include "MyCommandBuffer.h"

//------------------------------------------------
//	主传输线程
//	从发送队列中取出数据，并发送给服务器
//------------------------------------------------
class MySendRecvThread
	:public MyTransmitThreadBase
{
private:
	static MySendRecvThread* instance;	//单例模式

	MyCommandBuffer* buffer;			//维护对命令队列的引用
	MyCommand* cmd = 0;	

	bool isQuit;
private:
	MySendRecvThread();

public:
	~MySendRecvThread();

	bool Init(const char* ipAddr, int port);

	virtual std::string GetCertificationInfo();	//定义了父类的验证函数

	virtual bool InfoRight();				

	bool GetUserInfo();

	bool Communicate();

	virtual void Execute();

	static MySendRecvThread* Instance();
	
	static void Release();

};

#endif // !MySendRecvThread
