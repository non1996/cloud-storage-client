#pragma once

#ifndef MYCONTROL_H_
#define MYCONTROL_H_

#include "Cleaner.h"
//#include "MyNetWork.h"

#include "MySendRecvThread.h"
#include "MyRecvThread.h"
#include "MyCommandBuffer.h"
#include "MyMissionManager.h"

#include "MyNetFileManager.h"
#include "MyUserInfo.h"

#define SERVER_IP "10.201.14.164"
#define SERVER_PORT 10087

//---------------------------------------------------
//	命令执行线程
//	从命令接收队列中获取命令并执行
//---------------------------------------------------
class MyControl
	:public MyThread
{
private:
	static MyControl* instance;

	MyCommandBuffer* buffer;
//	MyNetWork* net;
	
	MyNetFileManager* netFileManager;
	MyUserInfo* user;

	//传输线程的引用
	MySendRecvThread* sendRecv;
	MyRecvThread* recv;
	MyMissionManager* downloadManager;
	MyMissionManager* uploadManager;

	bool isFinish;

protected:
	MyControl();

public:
	
	~MyControl();

	bool Init();

	#pragma region Init
	//主交互线程初始化和启动
	bool InitSendRecvThread();
	bool StartSendRecvThread();
	bool CloseStartSendRecvThread();

	//被动侦听线程初始化和启动
	bool InitMessageSocket(std::string &token, std::string &username);
	bool StartMessageSocket();
	bool CloseMessageSocket();

	//传输管理器初始化
	bool InitTransmitManager();
	#pragma endregion

	#pragma region Interface for mission manager
	int GetMissionNumD() {
		return downloadManager->GetMissionNum();
	}
	int GetMissionNumU() {
		return uploadManager->GetMissionNum();
	}
	int GetCurrentTransD() {
		return downloadManager->GetCurrentTrans();
	}
	int GetCurrentTransU() {
		return uploadManager->GetCurrentTrans();
	}

	bool ResumeD(int pos) {
		return downloadManager->Resume(pos);
	}
	bool ResumeU(int pos) {
		return uploadManager->Resume(pos);
	}

	bool SuspendD(int pos) {
		return downloadManager->Suspend(pos);
	}
	bool SuspendD(std::vector<int> &poss) {
		return downloadManager->Suspend(poss);
	}
	bool SuspendAllD() {
		return downloadManager->SuspendAll();
	}
	bool SuspendU(int pos) {
		return uploadManager->Suspend(pos);
	}
	bool SuspendU(std::vector<int> &poss) {
		return uploadManager->Suspend(poss);
	}
	bool SuspendAllU() {
		return uploadManager->SuspendAll();
	}

	bool CancelD(int pos) {
		return downloadManager->Cancel(pos);
	}
	bool CancelAllD() {
		return downloadManager->CancelAll();
	}
	bool CancelU(int pos) {
		return uploadManager->Cancel(pos);
	}
	bool CancelAllU() {
		return uploadManager->CancelAll();
	}

	//创建新的下载或上传任务
	bool NewMissionD(std::string &uID, std::string &token_one, std::string &username, std::string &password);
	bool NewMissionU(std::string &fileName, unsigned long long uID, std::string &localPath, std::string &netPath, std::string &token_one, std::string &username);


	#pragma endregion

	std::string &GetSendRecvToken() {
		return sendRecv->GetToken();
	}

	std::string &GetUserNameM() {
		return user->GetUsername();
	}

	std::string &GetPassword() {
		return user->GetPassword();
	}

	MyNetFileManager* GetManager() {
		return netFileManager;
	}

	bool IsLogIn();

	bool LogIn(std::string &un, std::string &pw);

	void SetFinish() {
		isFinish = true;
	}

	bool IsFinish() {
		return isFinish == true;
	}

	void CloseAllTransmit();

	virtual void Execute();
	static MyControl* Instance();

	static void Release();
};

#endif
