#pragma once

#ifndef MYCOMMANDBUFFER_H_
#define MYCOMMANDBUFFER_H_

#include "Cleaner.h"
#include "MyCommand.h"
#include <queue>
#include <string>

class MySemaphore;
class MyMutex;

//----------------------------------------------
//	命令队列类
//	维护两个队列：发送命令队列和接收命令队列
//	大致工作过程：
//	1、用户执行操作后会生成一个命令，该命令会被放到发送队列中
//	2、传输线程从发送队列中获取命令，将其转变为字符串并加密后发送给服务器
//	3、得到服务器响应后，传输线程将响应值存入命令并放入接收队列
//	4、控制线程从接收队列获取指令，调用命令的execute函数，并将自己的引用传入
//	5、命令调用控制类的函数来执行操作
//----------------------------------------------
class MyCommandBuffer{

private:
	static const int maxBufLen = 10;

private:
	static MyCommandBuffer* instance;	//单例模式

	#pragma region Command Queue
	std::queue<MyCommand*> recvQueue;	//接收队列
	MyMutex* recvMutex;					//接收互斥锁
	MySemaphore* recvEmpty, *recvFull;	//接收队列信号量

	std::queue<MyCommand*> sendQueue;	//发送队列
	MyMutex* sendMutex;					//发送互斥锁
	MySemaphore* sendEmpty, *sendFull;	//发送队列信号量
	#pragma endregion

	bool recvLock, sendLock;//废弃

protected:
	MyCommandBuffer();

public:
	~MyCommandBuffer();

	int GetSendCommandNum() {
		return sendQueue.size();
	}

	int GetRecvCommandNum() {
		return recvQueue.size();
	}

	bool IsRecvQueEmpty() {
		return recvQueue.empty();
	}

	bool IsSendQueEmpty() {
		return sendQueue.empty();
	}

	void ClearSendQueue();
	
	void ClearRecvQueue();

	#pragma region PutAndGet	
	void PutSend(MyCommand* c);

	void GetSend(MyCommand** c);

	void GetRecv(MyCommand** c);

	void PutRecv(MyCommand* c);

	//由其它线程调用
	bool GetSendCommand(MyCommand** cmd);	

	bool PutSendCommand(MyCommand* cmd);

	bool GetRecvCommand(MyCommand** cmd);	

	bool PutRecvCommand(MyCommand* cmd);

	#pragma endregion
	
	#pragma region LockControl		//已废弃
	bool TestAndLockR() {	
		if (IsRecvLock()) {
			return false;
		}
		LockRecv();
		return true;
	}
	
	bool TestAndLockS() {
		if (IsSendLock()) {
			return false;
		}
		LockSend();
		return true;
	}

	bool IsRecvLock() {
		return recvLock == true;
	}
	
	bool IsSendLock() {
		return sendLock == true;
	}
	
	void LockRecv() {
		recvLock = true;
	}
	
	void LockSend() {
		sendLock = true;
	}
	
	void UnlockSend() {
		sendLock = false;
	}
	
	void UnlockRecv() {
		recvLock = false;
	}

	#pragma endregion

	#pragma region Singleton
	static MyCommandBuffer* Instance();
	
	static void Release();
	
	#pragma endregion
};

#endif // !MYCOMMANDBUFFER_H_