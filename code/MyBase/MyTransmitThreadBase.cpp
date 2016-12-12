#include "MyTransmitThreadBase.h"
#include "MyEncoder.h"
MyTransmitThreadBase::MyTransmitThreadBase()
{
	sock = 0;

	isLogIn = false;
	isInit = false;
	isConnect = false;
	isFinish = false;
}

MyTransmitThreadBase::~MyTransmitThreadBase()
{
	Cleaner::Delete<MySocket*>(&sock);
}

MySocket * MyTransmitThreadBase::GetSocket()
{
	return sock;
}

bool MyTransmitThreadBase::Init(const char * ipAddr, int port)
{
	sock = new MySocket();

	if (sock == 0) {
		return false;
	}
	if (!sock->init(ipAddr, port)) {
		Cleaner::Delete<MySocket*>(&sock);
		return false;
	}
	SetInit(true);
	return true;
}

bool MyTransmitThreadBase::GetTokenFromServer()
{
	MySocket::Packet p;
	if (false == sock->Receive(p)) {
		SetConnect(false);
		return false;
	}
	token = std::string(p.buf, p.len);
	return true;
}

bool MyTransmitThreadBase::Connect()
{
	while (0 > sock->connect_to_srv()) {
		Sleep(1000);
		if (IsFinish() == true) {
			return false;
		}
	}
	SetConnect(true);
	return true;
}

void MyTransmitThreadBase::DisConnect()
{
	sock->disconnect();
}

bool MyTransmitThreadBase::Certification()
{
	std::string code;
	std::string recv;

	code = GetCertificationInfo();
	if (!sock->Send(code.c_str(), code.size())) {
		SetConnect(false);
		return false;
	}
	if (!InfoRight()) {
		return false;
	}
	SetLogIn(true);
	return true;
}
