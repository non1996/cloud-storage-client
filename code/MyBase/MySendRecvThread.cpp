#include "MySendRecvThread.h"
#include "MyEnCoder.h"

MySendRecvThread* MySendRecvThread::instance = 0;

MySendRecvThread::MySendRecvThread(): MyTransmitThreadBase()
{
	buffer = 0;
}


MySendRecvThread::~MySendRecvThread()
{
//	buffer = 0;
}

bool MySendRecvThread::Init(const char * ipAddr, int port)
{
	if (true == MyTransmitThreadBase::Init(ipAddr, port)) {
		buffer = MyCommandBuffer::Instance();
		return true;
	}
	return false;
}

std::string MySendRecvThread::GetCertificationInfo()
{
	std::string temp;

	buffer->GetSendCommand(&cmd);	//等待用户输入用户名和密码
	temp = cmd->ToString();
	if (temp.size() <= 8) {
		return "";
	}

	std::string userAndPassMD5;
	std::string code;
	std::string sizeUserAndPassMD5;			//用户名和密码MD5长度和
	std::string sizeCode;					//加密后的密文长度
	std::string sizeUser;					//用户明文长度
	
	sizeUserAndPassMD5 = temp.substr(0, 8);
	sizeUser = temp.substr(8, 8);
	userAndPassMD5 = temp.substr(16);
	code = MyEnCoder::Instance()->Encode(userAndPassMD5.c_str(), GetToken().c_str(), userAndPassMD5.size());
	sizeCode = MyEnCoder::UllToBytes((unsigned long long)code.size());

	return sizeUserAndPassMD5 + sizeCode + sizeUser + code;
}

bool MySendRecvThread::InfoRight()
{
	std::string rcv;
	if (false == GetSocket()->RecvBytes(rcv, GetToken().c_str())) {
		SetConnect(false);
		return false;
	}
	cmd->GetServerResponse(rcv.c_str(), rcv.size());
	buffer->PutRecvCommand(cmd);
	std::cout << "put commmand" << std::endl;
	return true;
}

bool MySendRecvThread::Communicate()
{
	while (!IsFinish()) {
		std::string readBuf;

		buffer->GetSendCommand(&cmd);
		std::cout << "send ----" << cmd->ToString() << std::endl;
		if (GetSocket()->SendBytes(cmd->ToString(), GetToken().c_str()) == false) {
			SetLogIn(false);
			SetConnect(false);
			return false;
		}
		if (GetSocket()->RecvBytes(readBuf, GetToken().c_str()) == false) {
			SetLogIn(false);
			SetConnect(false);
			return false;
		}
		std::cout << "recv finish\n";
		cmd->GetServerResponse(readBuf.c_str(), readBuf.size());
		buffer->PutRecvCommand(cmd);
		std::cout << "put finish\n";
	}
	return true;
}

void MySendRecvThread::Execute()
{
	if (!IsInit()) {
		return;
	}

	while (!IsFinish()) {
		if (!Connect()) {
			std::cout << "connect failed --------sendRecv thread" << std::endl;
			continue;
		}
		std::cout << "connect to server --------sendRecv thread" << std::endl;
		if (!GetTokenFromServer()) {
			std::cout << "connect broke --------sendRecv thread" << std::endl;
			continue;
		}
		std::cout << "get token " << GetToken() << " --------sendRecv thread" << std::endl;
		if (!Certification()) {
			std::cout << "connect broke --------sendRecv thread" << std::endl;
			continue;
		}
		std::cout << "log in! enter send receive loop --------sendRecv thread" << std::endl;
		if (!Communicate()) {
			std::cout << "connect broke --------sendRecv thread" << std::endl;
			continue;
		}
	}
	DisConnect();
}

MySendRecvThread * MySendRecvThread::Instance()
{
	if (instance == 0) {
		instance = new MySendRecvThread();
	}
	return instance;
}

void MySendRecvThread::Release()
{
	Cleaner::Delete<MySendRecvThread*>(&instance);
}
