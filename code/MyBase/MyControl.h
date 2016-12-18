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

#define SERVER_IP "10.201.14.164"
#define SERVER_PORT 10087

//---------------------------------------------------
//	����ִ���߳�
//	��������ն����л�ȡ���ִ��
//---------------------------------------------------

class MyMainWindow;
class MyControl
	:public MyThread
{
private:
	static MyControl* instance;

	MyCommandBuffer* buffer;
//	MyNetWork* net;
	
	MyNetFileManager* netFileManager;
	MyUserInfo* user;

	//�����̵߳�����
	MySendRecvThread* sendRecv;
	MyRecvThread* recv;
	MyMissionManager* downloadManager;
	MyMissionManager* uploadManager;

	bool isFinish;

	MyMainWindow* mainWindow;	//����֪ͨ���������ı�

protected:
	MyControl();

public:
	
	~MyControl();

	bool Init();

	void SetMainWindowReferance(MyMainWindow*);

	#pragma region Init
	//�������̳߳�ʼ��������
	bool InitSendRecvThread();
	bool StartSendRecvThread();
	bool CloseStartSendRecvThread();

	//���������̳߳�ʼ��������
	bool InitMessageSocket(std::string &token, std::string &username);
	bool StartMessageSocket();
	bool CloseMessageSocket();

	//�����������ʼ��
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

	//�����µ����ػ��ϴ�����
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

	std::string &GetUploadPath(int n) {
		return uploadManager->GetPath(n);
	}

	std::string &GetDownloadPath(int n) {
		return downloadManager->GetPath(n);
	}

	void ShowAlreadySendMessage(std::string &, std::string &);

	void ShowReceiveMessage(std::string &, std::string &);

	void Rename(std::string &, std::string &);

	void Paste();

	bool SetPath(std::string &s);

	bool ReplaceContent(std::vector<std::string> &fileInfo);

	bool IsConnect();

	bool IsLogIn();

	bool LogIn(std::string &un, std::string &pw);

	void SetFinish() {
		isFinish = true;
	}

	bool IsFinish() {
		return isFinish == true;
	}

//	void showInfo(std::string);	//test

	std::string GetPath(std::string &name);
	bool AddNormFile(std::string &uId, std::string &path, std::string &name, std::string &date, unsigned long long size, bool shared);
	bool AddDirector(std::string &uId, std::string &path, std::string & name, std::string & date, unsigned long long size, bool shared);
	void DeleteNetFile(std::string &name);

	#pragma region listen
	void GetProgress(MyMissionManager* m, int i, float pro, unsigned int speed);
	void Complete(MyMissionManager* m, int i);

	#pragma endregion

	void CloseAllTransmit();

	virtual void Execute();
	static MyControl* Instance();

	static void Release();
};

#endif
