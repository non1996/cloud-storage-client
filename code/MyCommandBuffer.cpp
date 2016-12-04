#include "MyCommandBuffer.h"

MyCommandBuffer* MyCommandBuffer::instance = 0;

MyCommandBuffer::MyCommandBuffer(){
	recvLock = false;
	sendLock = false;
}


MyCommandBuffer::~MyCommandBuffer(){
/*	while (!recvQueue.empty()) {
		MyCommand* temp = recvQueue.front();
		Cleaner::Delete<MyCommand*>(&temp);
		recvQueue.pop();
	}
	while (!sendQueue.empty()) {
		MyCommand* temp = sendQueue.front();
		Cleaner::Delete<MyCommand*>(&temp);
		sendQueue.pop();
	}*/
}

void MyCommandBuffer::ClearSendQueue()
{
	if (IsSendQueEmpty()) {
		return;
	}

	MyCommand* temp = 0;
	while (!IsSendQueEmpty()) {
		temp = sendQueue.front();
		sendQueue.pop();
		delete temp;
		temp = 0;
	}
}

void MyCommandBuffer::ClearRecvQueue()
{
	if (IsRecvQueEmpty()) {
		return;
	}

	MyCommand* temp = 0;
	while (!IsRecvQueEmpty()) {
		temp = recvQueue.front();
		recvQueue.pop();
		delete temp;
		temp = 0;
	}
}

void MyCommandBuffer::PutSend(MyCommand* c){
	sendQueue.push(c);
}

void MyCommandBuffer::GetRecv(MyCommand** c){
	if (recvQueue.empty()) {
		*c = 0;
		return;
	}
	*c = recvQueue.front();
	recvQueue.pop();
}

void MyCommandBuffer::PutRecv(MyCommand* c){
	recvQueue.push(c);
}

bool MyCommandBuffer::GetSendCommand(MyCommand ** cmd)
{
	if (IsSendQueEmpty()) {
		return false;
	}
	if (!TestAndLockS()) {
		return false;
	}	
	GetSend(cmd);
	UnlockSend();
	return true;
}

bool MyCommandBuffer::PutSendCommand(MyCommand * cmd)
{
	if (!TestAndLockS()) {
		return false;
	}
	PutSend(cmd);
	UnlockSend();
	return true;
}

bool MyCommandBuffer::GetRecvCommand(MyCommand ** cmd)
{
	if (IsRecvQueEmpty()) {
		return false;
	}
	if (!TestAndLockR()) {
		return false;
	}
	GetRecv(cmd);
	UnlockRecv();
	return true;
}

bool MyCommandBuffer::PutRecvCommand(MyCommand * cmd)
{
	if (!TestAndLockR()) {
		return false;
	}
	PutRecv(cmd);
	UnlockRecv();
	return true;
}

void MyCommandBuffer::GetSend(MyCommand ** c)
{
	if (IsSendQueEmpty()) {
		*c = 0;
		return;
	}
	*c = sendQueue.front();
	sendQueue.pop();
}

MyCommandBuffer * MyCommandBuffer::Instance()
{
	if (instance == 0) {
		instance = new MyCommandBuffer;
	}
	return instance;
}

void MyCommandBuffer::Release()
{
	Cleaner::Delete<MyCommandBuffer*>(&instance);
}


