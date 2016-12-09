#include "MyControl.h"

MyControl* MyControl::instance = 0;

MyControl::MyControl()
{
	buffer = 0;
	net = 0;
	user = 0;
	isFinish = false;
}

MyControl::~MyControl()
{
	if (net->GetSocket()) {
		net->GetSocket()->SetFinish();
		Sleep(1500);
		net->GetSocket()->Close();
	}

	if (net->GetMessageSocket()) {
		net->GetMessageSocket()->Close();
	}

	net->GetUpLoadManager()->CloseAllThread();
	net->GetDownLoadManager()->CloseAllThread();
	std::cout << "close all transfer\n";
	Sleep(500);
	Cleaner::Release<MyUserInfo*>(&user);
	Cleaner::Release<MyNetFileManager*>(&netFileManager);
	Cleaner::Release<MyNetWork*>(&net);
}

void MyControl::Execute()
{
	MyCommand* temp = 0;
	while (!IsFinish()) {
		if (false == buffer->GetRecvCommand(&temp)) {
			continue;
		}
		temp->Execute(this);
		Cleaner::Delete<MyCommand*>(&temp);
	}
	std::cout << "control quit\n";
}

MyControl * MyControl::Instance()
{
	if (instance == 0) {
		instance = new MyControl();
	}
	return instance;
}

void MyControl::Release()
{
	Cleaner::Delete<MyControl*>(&instance);
}
