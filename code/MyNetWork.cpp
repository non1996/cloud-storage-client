#include "MyNetWork.h"

MyNetWork* MyNetWork::instance = 0;

MyNetWork::MyNetWork()
{
	recv = 0;
	sendRecv = 0;

	downloadManager = 0;
	uploadManager = 0;
}


MyNetWork::~MyNetWork()
{
	Cleaner::Release<MyRecvThread*>(&recv);
	Cleaner::Release<MySendRecvThread*>(&sendRecv);
	Cleaner::Release<MyMissionManager*>(&downloadManager);
	Cleaner::Release<MyMissionManager*>(&uploadManager);
}

bool MyNetWork::InitSocket()
{
	sendRecv = MySendRecvThread::Instance();
	bool r = false;
	r = sendRecv->Init("10.201.14.164", 10087);
	if (r == false) {
		Cleaner::Release<MySendRecvThread*>(&sendRecv);
		return false;
	}
	return true;
}

bool MyNetWork::StartSocket()
{
	if (!sendRecv) {
		return false;
	}

	bool r = false;
	r = sendRecv->Start();
	if (r == false) {
		sendRecv->Close();
		return false;
	}
	sendRecv->Resume();
	return true;
}

bool MyNetWork::CloseSocket()
{
	sendRecv->Close();
	return true;
}

bool MyNetWork::InitMessageSocket(std::string &token, std::string &username)
{
	recv = MyRecvThread::Instance();
	recv->SetUsername(username);
	recv->SetTokenOne(token);
	bool r = false;
	r = recv->Init("10.201.14.164", 10087);
	if (r == false) {
		Cleaner::Release<MyRecvThread*>(&recv);
		return false;
	}
	return true;
}

bool MyNetWork::StartMessageSocket()
{
	if (!recv) {
		return false;
	}

	bool r = false;
	r = recv->Start();
	if (r == false) {
		recv->Close();
		return false;
	}
	recv->Resume();
	return true;
}

bool MyNetWork::CloseMessageSocket()
{
	recv->Close();
	return true;
}

bool MyNetWork::InitManger()
{
	downloadManager = new MyMissionManager();
	if (downloadManager == 0) {
		return false;
	}

	uploadManager = new MyMissionManager();
	if (uploadManager == 0) {
		Cleaner::Delete<MyMissionManager*>(&downloadManager);
		return false;
	}

	downloadManager->SetMaxCurrency(3);
	uploadManager->SetMaxCurrency(1);

	return true;
}

bool MyNetWork::NewMissionD(std::string &uID, std::string &token_one, std::string &username, std::string &password)
{
	MyDownLoadMission* temp = 0;
	temp = new MyDownLoadMission();
	if (temp == 0) {
		return false;
	}
	if (temp->InitSocket("10.201.14.164", 10087) == false) {
		Cleaner::Release<MyDownLoadMission*>(&temp);
		return false;
	}
	temp->SetTokenOne(token_one);
	temp->SetUsername(username);
	temp->SetFileID(uID);
	temp->SetPassword(password);
	downloadManager->NewMission(temp);
	return true;
}

bool MyNetWork::NewMissionU(std::string &fileName, unsigned long long uID, std::string &localPath, std::string &netPath, std::string &token_one, std::string &username)
{
	MyUpLoadMission* temp = 0;
	temp = new MyUpLoadMission();
	if (temp == 0) {
		return false;
	}
	if (!temp->InitSocket("10.201.14.164", 10087)) {
		Cleaner::Release<MyUpLoadMission*>(&temp);
		return false;
	}
	temp->SetTokenOne(token_one);
	temp->SetUsername(username);
	temp->SetUID(uID);
	temp->SetFileName(fileName);
	temp->SetLocalPath(localPath);
	temp->SetNetPath(netPath);
	uploadManager->NewMission(temp);
	return true;
}

void MyNetWork::CloseManager()
{
	downloadManager->CloseAll();
	uploadManager->CloseAll();
}

MyNetWork * MyNetWork::Instance()
{
	if (instance == 0) {
		instance = new MyNetWork();
	}
	return instance;
}

void MyNetWork::Release()
{
	Cleaner::Delete<MyNetWork*>(&instance);
}
