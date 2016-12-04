#pragma once
#include "Cleaner.h"
#include "MySocket.h"
#include "MyThread.h"

class MyTransmitThreadBase :
	public MyThread
{
private:
	MySocket* sock;

	std::string token;

	bool isLogIn;
	bool isInit;
	bool isConnect;
	bool isFinish;
public:
	MyTransmitThreadBase();
	virtual ~MyTransmitThreadBase();
	#pragma region Set
	void SetInit(bool st) {
		isInit = st;
	}

	void SetLogIn(bool st) {
		isLogIn = st;
	}
	
	void SetConnect(bool st) {
		isConnect = st;
	}

	void SetFinish() {
		isFinish = true;
	}

	void SetToken(std::string &t) {
		token = t;
	}
#pragma endregion

	#pragma region Get
	bool IsLogIn() {
		return isLogIn;
	}
	bool IsInit() {
		return isInit;
	}
	bool IsConnect() {
		return isConnect;
	}
	bool IsFinish() {
		return isFinish;
	}
	MySocket* GetSocket();
	std::string &GetToken() {
		return token;
	}
#pragma endregion

	bool Init(const char* ipAddr, int port);

	bool GetTokenFromServer();
	
	bool Connect();

	void DisConnect();

	#pragma region Certification
	//��ȡ��֤��Ϣ, �������ǻ��������ʵ��
	virtual std::string GetCertificationInfo() = 0;

	//��֤�Ƿ�ͨ��, �������ǻ��������ʵ��
	virtual bool InfoRight() = 0;

	virtual bool Certification();
	#pragma endregion

};

