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
//	This class maintains two queue: receive commands, and send commands. 
//	1.Controler put command into queue, sender get command and send it to the server
//	2.Receiver receive command from server, and put it into queue, 
//	  Controler then get it and execute.
//	This class is a singleton 
//----------------------------------------------
class MyCommandBuffer{

private:
	static const int maxBufLen = 10;

private:
	static MyCommandBuffer* instance;

	#pragma region Command Queue
	std::queue<MyCommand*> recvQueue;
	MyMutex* recvMutex;
	MySemaphore* recvEmpty, *recvFull;

	std::queue<MyCommand*> sendQueue;
	MyMutex* sendMutex;
	MySemaphore* sendEmpty, *sendFull;
	#pragma endregion

	bool recvLock, sendLock;//·ÏÆú

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

	//invoked by others
	bool GetSendCommand(MyCommand** cmd);	

	bool PutSendCommand(MyCommand* cmd);

	bool GetRecvCommand(MyCommand** cmd);	

	bool PutRecvCommand(MyCommand* cmd);

	#pragma endregion
	
	#pragma region LockControl		//ÒÑ·ÏÆú
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