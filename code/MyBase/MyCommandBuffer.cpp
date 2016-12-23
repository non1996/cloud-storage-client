#include "MyCommandBuffer.h"
#include "MyThread.h"

MyCommandBuffer* MyCommandBuffer::instance = 0;

MyCommandBuffer::MyCommandBuffer(){
	recvMutex = new MyMutex();
	sendMutex = new MyMutex();

	recvEmpty = new MySemaphore(maxBufLen, maxBufLen);
	sendEmpty = new MySemaphore(maxBufLen, maxBufLen);

	recvFull = new MySemaphore(0, maxBufLen);
	sendFull = new MySemaphore(0, maxBufLen);

	recvLock = false;
	sendLock = false;
}


MyCommandBuffer::~MyCommandBuffer(){
	while (!recvQueue.empty()) {
		MyCommand* temp = recvQueue.front();
		Cleaner::Delete<MyCommand*>(&temp);
		recvQueue.pop();
	}
	while (!sendQueue.empty()) {
		MyCommand* temp = sendQueue.front();
		Cleaner::Delete<MyCommand*>(&temp);
		sendQueue.pop();
	}
	Cleaner::Delete<MyMutex*>(&recvMutex);
	Cleaner::Delete<MyMutex*>(&sendMutex);
	Cleaner::Delete<MySemaphore*>(&recvEmpty);
	Cleaner::Delete<MySemaphore*>(&sendEmpty);
	Cleaner::Delete<MySemaphore*>(&recvFull);
	Cleaner::Delete<MySemaphore*>(&sendFull);
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
	sendFull->Wait();
	sendMutex->Wait();
	GetSend(cmd);
	sendMutex->Signal();
	sendEmpty->Signal();
	return true;
}

bool MyCommandBuffer::PutSendCommand(MyCommand * cmd)
{
	sendEmpty->Wait();
	sendMutex->Wait();
	PutSend(cmd);
	sendMutex->Signal();
	sendFull->Signal();
	return true;
}

bool MyCommandBuffer::GetRecvCommand(MyCommand ** cmd)
{
	recvFull->Wait();
	recvMutex->Wait();
	GetRecv(cmd);
	recvMutex->Signal();
	recvEmpty->Signal();
	return true;
}

bool MyCommandBuffer::PutRecvCommand(MyCommand * cmd)
{
	recvEmpty->Wait();
	recvMutex->Wait();
	PutRecv(cmd);
	recvMutex->Signal();
	recvFull->Signal();
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


