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
//	�����߳�
//	��������ն����л�ȡ���ִ��
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

	//�����߳�
	MySendRecvThread* sendRecv;
	MyRecvThread* recv;
	MyMissionManager* downloadManager;
	MyMissionManager* uploadManager;

	bool isFinish;

	MyMainWindow* mainWindow;	//listenerģʽ������֪ͨ���������ı�

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

	//�����ϴ�������������ʱ��ԭ��Э��û�������ã���ʱû��ʵ��
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

	//�����������
	#pragma region control
	//���������ʾҪ���͵���Ϣ
	void ShowAlreadySendMessage(std::string &, std::string &);

	//���������ʾ���յ�����Ϣ
	void ShowReceiveMessage(std::string &, std::string &);

	//�������ļ�����
	void Rename(std::string &, std::string &);

	//ճ���ļ�����
	void Paste();

	//����·��
	bool SetPath(std::string &s);

	//�����ļ�������
	bool ReplaceContent(std::vector<std::string> &fileInfo);

	//�����û���������
	bool LogIn(std::string &un, std::string &pw);

	void SetFinish() {
		isFinish = true;
	}

	bool IsFinish() {
		return isFinish == true;
	}

	void ShowShareInfo(bool b);

	//��������δ������ɵ��߳�
	void ResumeAllNotComplete();

	//������ʾ�ڴ����ϵ��û������û�ͷ���Լ��û���������
	void SetUserInfo(std::string &name, std::string &url, unsigned long long currentV, unsigned long long totalV);

	std::string GetPath(std::string &name);
	
	bool AddNormFile(std::string &uId, std::string &path, std::string &name, std::string &date, unsigned long long size, bool shared);
	
	bool AddDirector(std::string &uId, std::string &path, std::string & name, std::string & date, unsigned long long size, bool shared);
	
	void DeleteNetFile(std::string &name);
	#pragma endregion

	//���ϴ��������̵߳��ã�֪ͨ����ı�
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
