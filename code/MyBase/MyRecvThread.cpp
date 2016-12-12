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
	std::cout << ut << std::endl;
	std::string code;

	std::string sizeUserAndToken;
	std::string sizeCode;
	std::string sizeUser;

	code = instance->Encode(ut.c_str(), GetToken().c_str(), ut.size());

	sizeUserAndToken = MyEnCoder::UllToBytes((unsigned long long)ut.size());
	sizeCode = MyEnCoder::UllToBytes((unsigned long long)code.size());
	sizeUser = MyEnCoder::UllToBytes(usize);
	std::cout << "encode finish\n";
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
		std::cout << "info wrong-----------server broke the link\n";
		return false;
	}
	std::cout << "info right\n";
	return true;
}

bool MyRecvThread::Communicate()
{
	MyCommand* temp = 0;
	std::string recv;
	while (!IsFinish()) {
		recv.clear();
//		std::cout << "receiver thread waits for server's message\n";
		if (false == GetSocket()->RecvBytes(recv, GetToken().c_str())) {
			return false;
		}
		if (recv == "[check]") {			//保活信息
//			std::cout << recv << std::endl;
			continue;
		}
		temp = MyCommandBuilder::MakeMessageCommand();
//		std::cout << "生成消息指令\n";
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
	std::cout << "start! ---------recv thread" << std::endl;
	while (!IsFinish()) {
		if (!Connect()) {
			std::cout << "connect failed --------recv thread" << std::endl;
			Sleep(10000);
			continue;
		}
		std::cout << "connect to server --------recv thread" << std::endl;
		if (!GetTokenFromServer()) {
			Sleep(10000);
			continue;
		}
		std::cout << "get token " << GetToken() << " --------recv thread" << std::endl;
		if (!Certification()) {
			std::cout << "connect broke1 --------recv thread" << std::endl;
			Sleep(10000);
			continue;
		}
		else {
			break;
		}
	}
	std::cout << "log in! enter receive loop --------recv thread" << std::endl;
	if (!Communicate()) {
		std::cout << "server disconnect\n";
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