#pragma once

#ifndef MYSENDRECVTHREAD_H_
#define MYSENDRECVTHREAD_H_

#include "Cleaner.h"
#include "MyTransmitThreadBase.h"
#include "MyCommandBuffer.h"

class MySendRecvThread
	:public MyTransmitThreadBase
{
private:
	static MySendRecvThread* instance;

	//---------------------------------------------------------
	//	a referance of command buffer.
	//	Thread get command from send buffer and send command to server,
	//	and while receiving a command from server, the command is put into
	//	receive buffer. The control thread then catches this command and
	//	performs the corresponding operation 
	//---------------------------------------------------------
	MyCommandBuffer* buffer;		
	MyCommand* cmd = 0;	
private:
	MySendRecvThread();

public:
	~MySendRecvThread();

	bool Init(const char* ipAddr, int port);

	virtual std::string GetCertificationInfo();

	virtual bool InfoRight();

	bool Communicate();

	virtual void Execute();

	static MySendRecvThread* Instance();
	
	static void Release();

	//these print method use for test
	void TextOutExit() {
		std::cout << "send and receive thread exit\n";
	}
};

#endif // !MySendRecvThread
