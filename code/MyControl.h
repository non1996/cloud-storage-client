#pragma once

#ifndef MYCONTROL_H_
#define MYCONTROL_H_

#include "Cleaner.h"
#include "MyNetWork.h"
#include "MyNetFileManager.h"
#include "MyUserInfo.h"

//---------------------------------------------------
//	����ִ���߳�
//	��������ն����л�ȡ���ִ��
//---------------------------------------------------
class MyControl
	:public MyThread
{
private:
	static MyControl* instance;

	MyCommandBuffer* buffer;
	MyNetWork* net;
	MyNetFileManager* netFileManager;
	MyUserInfo* user;

	bool isFinish;

protected:
	MyControl();

public:
	
	~MyControl();

	MySendRecvThread* GetSendRecv() {
		return GetNetWork()->GetSocket();
	}

	MyNetWork* GetNetWork() {
		return net;
	}

	MyNetFileManager* GetManager() {
		return netFileManager;
	}

	MyUserInfo* GetUser() {
		return user;
	}

	bool Init() {
		buffer = MyCommandBuffer::Instance();
		net = MyNetWork::Instance();
		netFileManager = MyNetFileManager::Instance();
		user = MyUserInfo::Instance();

		if (!net->InitSocket() || !net->InitManger()) {
			Cleaner::Release<MyNetWork*>(&net);
			return false;
		}
		if (!net->StartSocket()) {
			Cleaner::Release<MyNetWork*>(&net);
			return false;
		}

		Start();
		Resume();

		return true;
	}

	void SetFinish() {
		isFinish = true;
	}

	bool IsFinish() {
		return isFinish == true;
	}

	void NetWorkStop() {					//�ر�����ģ��
		net->CloseSocket();					//�ر������̣߳����Ͽ�����
		net->CloseManager();				//�������д����̴߳����굱ǰ�����Ͽ����ӣ�
											//����û�йر��߳�
		SetFinish();
	}

	virtual void Execute();
	static MyControl* Instance();

	static void Release();
};

#endif
