#include "MyRecvThread.h"
#include "MyEnCoder.h"
MyRecvThread* MyRecvThread::instance = 0;

MyRecvThread::MyRecvThread(): MyTransmitThreadBase()
{
	buffer = 0;
}


MyRecvThread::~MyRecvThread()
{
}

bool MyRecvThread::Init(const char * ipAddr, int port)
{
	if (true == MyTransmitThreadBase::Init(ipAddr, port)) {
		buffer = MyCommandBuffer::Instance();
		return true;
	}
	return false;
}

std::string MyRecvThread::EncodeUT(std::string & ut, unsigned long long usize)
{
	MyEnCoder* instance = MyEnCoder::Instance();
	std::string code;

	std::string sizeUserAndToken;
	std::string sizeCode;
	std::string sizeUser;

	code = instance->Encode(ut.c_str(), GetToken().c_str(), ut.size());

	sizeUserAndToken = MyEnCoder::UllToBytes((unsigned long long)ut.size());
	sizeCode = MyEnCoder::UllToBytes((unsigned long long)code.size());
	sizeUser = MyEnCoder::UllToBytes(usize);
	return sizeUserAndToken + sizeCode + sizeUser + code;
}

std::string MyRecvThread::GetCertificationInfo()
{
	return EncodeUT(username + token_one, username.size());
}

bool MyRecvThread::InfoRight()
{
	std::string rcv;
	if (GetSocket()->RecvBytes(rcv, GetToken().c_str()) == false) {
		SetConnect(false);
		return false;
	}
	return true;
}

bool MyRecvThread::Communicate()
{
	MyCommand* temp = 0;
	std::string recv;
	while (!IsFinish()) {
		recv.clear();
		if (false == GetSocket()->RecvBytes(recv, GetToken().c_str())) {
			return false;
		}
		if (recv == "[check]") {			//保活信息
			continue;
		}
		temp = MyCommandBuilder::MakeMessageCommand();
		temp->GetServerResponse(recv.c_str(), recv.size());
		buffer->PutRecvCommand(temp);
	}
	return true;
}

void MyRecvThread::Execute()
{
	if (!IsInit()) {
		return;
	}
	while (!IsFinish()) {
		if (!Connect()) {
			Sleep(10000);
			continue;
		}
		if (!GetTokenFromServer()) {
			Sleep(10000);
			continue;
		}
		if (!Certification()) {
			Sleep(10000);
			continue;
		}
		else {
			break;
		}
	}
	if (!Communicate()) {
	}
	DisConnect();
}

MyRecvThread * MyRecvThread::Instance()
{
	if (instance == 0) {
		instance = new MyRecvThread();
	}
	return instance;
}

void MyRecvThread::Release()
{
	Cleaner::Delete<MyRecvThread*>(&instance);
}