#pragma once

#ifndef MYMISSIONMANAGER_H_
#define MYMISSIONMANAGER_H_

#include "MyMission.h"
#include <vector>

//----------------------------------------------------
//	管理传输任务
//	维护一个任务队列
//	提供创建任务，挂起任务，恢复任务，取消任务等服务
//----------------------------------------------------
class MyMissionManager
{
private:
	std::vector<MyMission*> missionList;
	int maxCurrency;			//能同时执行的任务数
	int currentTrans;			//当前正在执行的任务数

public:
	MyMissionManager();

	~MyMissionManager();

	//添加一个新任务,如果同时运行的任务数达到最大值，
	//则任务添加后挂起，否则任务添加后直接执行
	bool NewMission(MyMission* m);

#pragma region GetAndSet
	void SetMaxCurrency(const int mc) {
		maxCurrency = mc;
	}

	void SetCurrentTrans(const int ct) {
		currentTrans = ct;
	}

	int GetMaxCurrency() {
		return maxCurrency;
	}

	int GetCurrentTrans() {
		return currentTrans;
	}

	int GetMissionNum() {
		return missionList.size();
	}

	bool IsFull() {
		return currentTrans == maxCurrency;
	}

#pragma endregion

#pragma region MissionManage
	//继续运行传输线程，传入参数为线程下标
	//如果下标越界，返回false
	//否则执行线程的resume函数，并返回其返回值
	bool Resume(int pos);

	//挂起传输线程（即暂停），传入参数为线程下标
	//如果下标越界，返回false
	//否则执行线程的挂起函数，并返回其返回值
	bool Suspend(int pos);
	bool Suspend(std::vector<int> &poss);
	bool SuspendAll();

	bool Cancel(int pos);
	bool CancelAll();

	//程序退出时终止所有未传输完的线程
	void CloseAllThread();

#pragma endregion

	void Release();
};

#endif // !MYMISSIONMANAGER_H_
