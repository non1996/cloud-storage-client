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
//	���������
//	ά���������У�����������кͽ����������
//	���¹������̣�
//	1���û�ִ�в����������һ�����������ᱻ�ŵ����Ͷ�����
//	2�������̴߳ӷ��Ͷ����л�ȡ�������ת��Ϊ�ַ��������ܺ��͸�������
//	3���õ���������Ӧ�󣬴����߳̽���Ӧֵ�������������ն���
//	4�������̴߳ӽ��ն��л�ȡָ����������execute�����������Լ������ô���
//	5��������ÿ�����ĺ�����ִ�в���
//----------------------------------------------
class MyCommandBuffer{

private:
	static const int maxBufLen = 10;

private:
	static MyCommandBuffer* instance;	//����ģʽ

	#pragma region Command Queue
	std::queue<MyCommand*> recvQueue;	//���ն���
	MyMutex* recvMutex;					//���ջ�����
	MySemaphore* recvEmpty, *recvFull;	//���ն����ź���

	std::queue<MyCommand*> sendQueue;	//���Ͷ���
	MyMutex* sendMutex;					//���ͻ�����
	MySemaphore* sendEmpty, *sendFull;	//���Ͷ����ź���
	#pragma endregion

	bool recvLock, sendLock;//����

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

	//�������̵߳���
	bool GetSendCommand(MyCommand** cmd);	

	bool PutSendCommand(MyCommand* cmd);

	bool GetRecvCommand(MyCommand** cmd);	

	bool PutRecvCommand(MyCommand* cmd);

	#pragma endregion
	
	#pragma region LockControl		//�ѷ���
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