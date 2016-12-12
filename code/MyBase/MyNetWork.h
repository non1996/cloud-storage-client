#pragma once

#ifndef MYNETWORK_H_
#define MYNETWORK_H_


#include "MySendRecvThread.h"
#include "MyRecvThread.h"
#include "MyMissionManager.h"
#include "MyDownLoadMission.h"
#include "MyUpLoadMission.h"

#include "Cleaner.h"


//------------------------------------------
//	外观类，维护接收命令线程
//				发送命令线程
//				下载管理器
//				上传管理器
//	提供初始化、控制、关闭这几个模块的接口
//	
//------------------------------------------
class MyNetWork
{
private:
	static MyNetWork* instance;

	MySendRecvThread* sendRecv;
	MyRecvThread* recv;					//被动接收从服务器发来的信息，登陆后创建链接

	MyMissionManager* downloadManager;
	MyMissionManager* uploadManager;

protected:
	MyNetWork();

public:	
	~MyNetWork();

	//----------------------------------------------------------------------
	//	管理命令线程
	//----------------------------------------------------------------------
	bool InitSocket();				//创建并初始化接收命令和发送命令线程
	bool StartSocket();				//链接到服务器并启动线程
	bool CloseSocket();				//关闭线程

	MySendRecvThread* GetSocket() {
		return sendRecv;
	}

	//----------------------------------------------------------------------
	//	管理消息线程
	//----------------------------------------------------------------------
	bool InitMessageSocket(std::string &token, std::string &username);
	bool StartMessageSocket();
	bool CloseMessageSocket();
	MyRecvThread* GetMessageSocket() {
		return recv;
	}

	//----------------------------------------------------------------------
	//	管理下载和上传管理器
	//	对外提供mission manager相应的接口
	//----------------------------------------------------------------------
	#pragma region Manager
	
	bool InitManger();				//初始化下载和上传管理器
	
	MyMissionManager* GetDownLoadManager() {
		return downloadManager;
	}

	MyMissionManager* GetUpLoadManager() {
		return uploadManager;
	}

	void SetMaxCurrencyD(const int mc) {
		downloadManager->SetMaxCurrency(mc);
	}
	void SetMaxCurrencyU(const int mc) {
		uploadManager->SetMaxCurrency(mc);
	}
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

	void CloseManager();
	#pragma endregion

	static MyNetWork* Instance();
	static void Release();
};

#endif // !MYNETWORK_H_


