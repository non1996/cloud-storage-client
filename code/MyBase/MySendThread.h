#pragma once

#ifndef MYSENDTHREAD_H_
#define MYSENDTHREAD_H_

#include "MySocket.h"
#include "MyThread.h"
#include "MyCommandBuffer.h"

class MySendThread :
	public MyThread
{
private:
	static MySendThread* instance;

	MySocket* sender;

	//-----------------------------------------------------
	//	maintain a referance of command buffer, so that 
	//	it can get command which should be send. 
	//-----------------------------------------------------
	MyCommandBuffer* buffer;

	bool isInit;
	bool isConnect;
	bool isFinish;

protected:
	MySendThread();

public:
	~MySendThread();

	bool Init(const char* ipAddr, int port);

#pragma region Set
	void SetSender(MySocket* sender) {
		this->sender = sender;
	}

	void SetBuffer(MyCommandBuffer* buffer) {
		this->buffer = buffer;
	}

	void SetBuffer() {
		this->buffer = MyCommandBuffer::Instance();
	}

	bool IsConnect() {
		return isConnect;
	}

	void SetConnect() {
		isConnect = false;
	}

	bool IsInit() {
		return isInit;
	}

	void SetInit() {
		isInit = true;
	}

	bool IsFinish() {
		return isFinish;
	}

	void SetFinish() {
		isFinish = true;
	}
#pragma endregion

	void Disconnect() {
		sender->disconnect();
	}

	bool GetCommand(MyCommand** temp);

	virtual void Execute();

	static MySendThread* Instance();

	static void Release();
};

#endif // !MYSENDTHREAD_H_
