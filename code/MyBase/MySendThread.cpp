#include "MySendThread.h"

MySendThread* MySendThread::instance = 0;

MySendThread::MySendThread(): MyThread()
{
	sender = 0;
	buffer = 0;
	isInit = false;
	isFinish = false;
}

MySendThread::~MySendThread()
{
	sender->Release();
	Cleaner::Delete<MySocket*>(&sender);
	buffer = 0;

	isInit = false;
	isConnect = false;
	isFinish = false;
}

bool MySendThread::Init(const char * ipAddr, int port)
{
	sender = new MySocket();
	buffer = MyCommandBuffer::Instance();

	if (sender == 0) {
		return false;
	}
	if (buffer == 0) {
		return false;
	}
	if (!sender->init(ipAddr, port)) {
		return false;
	}
	
	SetInit();
	
	return true;
}

 bool MySendThread::GetCommand(MyCommand** temp)
{
	if (buffer == 0) {
		return false;
	}
	if (buffer->IsSendLock()) {
		std::cout << "buffer lock\n";
		return false;
	}
	if (buffer->IsSendQueEmpty()) {
		return false;
	}

	buffer->LockSend();
	buffer->GetSend(temp);
	buffer->UnlockSend();
	return true;
}

void MySendThread::Execute()
{
	if (!IsInit()) {
		return;
	}

	Sleep(1000);//等待receiver先链接
	while (0 > sender->connect_to_srv()) {
		Sleep(1000);
	}
	std::cout << "sender连接到服务器\n";///
	SetConnect();

	MyCommand* temp = 0;
	while (!IsFinish()/* || !buffer->IsSendQueEmpty()*/) {
		if (GetCommand(&temp) == false) {
			continue;
		}
		if(sender->Send(temp->ToString().c_str(), temp->ToString().size()))
		Cleaner::Delete<MyCommand*>(&temp);
	}
	std::cout << "sender finish\n";///
}

MySendThread * MySendThread::Instance()
{
	if (instance == 0) {
		instance = new MySendThread();
	}
	return instance;
}

void MySendThread::Release()
{
	Cleaner::Delete<MySendThread*>(&instance);
}


