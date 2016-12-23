#pragma once

#ifndef MYCONTROL_H_
#define MYCONTROL_H_

#include "Cleaner.h"

#include "MySendRecvThread.h"
#include "MyRecvThread.h"
#include "MyCommandBuffer.h"
#include "MyMissionManager.h"
#include "MyNetFileManager.h"
#include "MyUserInfo.h"

#define SERVER_IP "10.201.14.176"
#define SERVER_PORT 10087

//---------------------------------------------------
//	控制线程
//	从命令接收队列中获取命令并执行
//---------------------------------------------------
class MyMainWindow;
class MyControl
	:public MyThread
{
private:
	std::string serverIP;
	int serverPort;

private:
	static MyControl* instance;

	MyCommandBuffer* buffer;
	
	MyNetFileManager* netFileManager;
	MyUserInfo* user;

	//传输线程
	MySendRecvThread* sendRecv;
	MyRecvThread* recv;
	MyMissionManager* downloadManager;
	MyMissionManager* uploadManager;

	bool isFinish;

	MyMainWindow* mainWindow;	//listener模式，用于通知界面做出改变

protected:
	MyControl();

public:
	
	~MyControl();

	bool IsConnect();

	bool IsLogIn();

	bool Init();

	void SetMainWindowReferance(MyMainWindow*);

	bool ReadNetInfo();

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

	//管理上传下载任务，由于时间原因，协议没有商量好，暂时没有实现
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

	#pragma region Get
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

	std::string &GetUploadPath(int n) {
		return uploadManager->GetPath(n);
	}

	std::string &GetDownloadPath(int n) {
		return downloadManager->GetPath(n);
	}
	#pragma endregion

	//由命令类调用
	#pragma region control
	//在聊天框显示要发送的消息
	void ShowAlreadySendMessage(std::string &, std::string &);

	//在聊天框显示接收到的消息
	void ShowReceiveMessage(std::string &, std::string &);

	//重命名文件操作
	void Rename(std::string &, std::string &);

	//粘贴文件操作
	void Paste();

	//设置路径
	bool SetPath(std::string &s);

	//重置文件管理器
	bool ReplaceContent(std::vector<std::string> &fileInfo);

	//保存用户名和密码
	bool LogIn(std::string &un, std::string &pw);

	void SetFinish() {
		isFinish = true;
	}

	bool IsFinish() {
		return isFinish == true;
	}

	void ShowShareInfo(bool b);

	//重启所有未传输完成的线程
	void ResumeAllNotComplete();

	//设置显示在窗口上的用户名和用户头像以及用户网盘容量
	void SetUserInfo(std::string &name, std::string &url, unsigned long long currentV, unsigned long long totalV);

	std::string GetPath(std::string &name);
	
	bool AddNormFile(std::string &uId, std::string &path, std::string &name, std::string &date, unsigned long long size, bool shared);
	
	bool AddDirector(std::string &uId, std::string &path, std::string & name, std::string & date, unsigned long long size, bool shared);
	
	void DeleteNetFile(std::string &name);
	#pragma endregion

	//由上传或下载线程调用，通知界面改变
	#pragma region listen
	void GetProgress(MyMissionManager* m, int i, float pro, unsigned int speed);
	void Complete(MyMissionManager* m, int i);

	#pragma endregion

	void Restart();

	void CloseAllTransmit();

	virtual void Execute();

	static MyControl* Instance();

	static void Release();
};

#endif
