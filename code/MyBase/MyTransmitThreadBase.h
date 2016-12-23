#pragma once
#include "Cleaner.h"
#include "MySocket.h"
#include "MyThread.h"

//------------------------------------
//	所有传输线程的基类
//	提供传输线程初始化，与服务器连接，从服务器获取秘钥的方法
//	子类自行定义获取验证信息和验证的函数
//------------------------------------
class MyTransmitThreadBase :
	public MyThread
{
private:
	MySocket* sock;

	std::string ServerIp;
	int ServerPort;

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

	bool ReInit();

	bool GetTokenFromServer();
	
	bool Connect();

	void DisConnect();

	#pragma region Certification
	//获取验证信息, 活动连接与非活动连接自行实现
	virtual std::string GetCertificationInfo() = 0;

	//验证是否通过, 活动连接与非活动连接自行实现
	virtual bool InfoRight() = 0;

	//与服务器进行验证，此方法为模板方法
	virtual bool Certification();

	#pragma endregion

};

