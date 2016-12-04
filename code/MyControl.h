#pragma once

#ifndef MYCONTROL_H_
#define MYCONTROL_H_

#include "Cleaner.h"
#include "MyNetWork.h"
#include "MyNetFileManager.h"
#include "MyUserInfo.h"

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

	void NetWorkStop() {					//关闭网络模块
		net->CloseSocket();					//关闭命令线程，并断开连接
		net->CloseManager();				//仅让所有传输线程传输完当前任务后断开连接，
											//但还没有关闭线程
		SetFinish();
	}

	virtual void Execute();
	static MyControl* Instance();

	static void Release();
};

#endif
