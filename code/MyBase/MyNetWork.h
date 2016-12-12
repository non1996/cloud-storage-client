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
//	����࣬ά�����������߳�
//				���������߳�
//				���ع�����
//				�ϴ�������
//	�ṩ��ʼ�������ơ��ر��⼸��ģ��Ľӿ�
//	
//------------------------------------------
class MyNetWork
{
private:
	static MyNetWork* instance;

	MySendRecvThread* sendRecv;
	MyRecvThread* recv;					//�������մӷ�������������Ϣ����½�󴴽�����

	MyMissionManager* downloadManager;
	MyMissionManager* uploadManager;

protected:
	MyNetWork();

public:	
	~MyNetWork();

	//----------------------------------------------------------------------
	//	���������߳�
	//----------------------------------------------------------------------
	bool InitSocket();				//��������ʼ����������ͷ��������߳�
	bool StartSocket();				//���ӵ��������������߳�
	bool CloseSocket();				//�ر��߳�

	MySendRecvThread* GetSocket() {
		return sendRecv;
	}

	//----------------------------------------------------------------------
	//	������Ϣ�߳�
	//----------------------------------------------------------------------
	bool InitMessageSocket(std::string &token, std::string &username);
	bool StartMessageSocket();
	bool CloseMessageSocket();
	MyRecvThread* GetMessageSocket() {
		return recv;
	}

	//----------------------------------------------------------------------
	//	�������غ��ϴ�������
	//	�����ṩmission manager��Ӧ�Ľӿ�
	//----------------------------------------------------------------------
	#pragma region Manager
	
	bool InitManger();				//��ʼ�����غ��ϴ�������
	
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

	//�����µ����ػ��ϴ�����
	bool NewMissionD(std::string &uID, std::string &token_one, std::string &username, std::string &password);
	bool NewMissionU(std::string &fileName, unsigned long long uID, std::string &localPath, std::string &netPath, std::string &token_one, std::string &username);

	void CloseManager();
	#pragma endregion

	static MyNetWork* Instance();
	static void Release();
};

#endif // !MYNETWORK_H_


