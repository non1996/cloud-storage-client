#pragma once

#ifndef MYTHREAD_H_
#define MYTHREAD_H_

#include <Windows.h>
#include <iostream>

//---------------------------------------
//	��װ��windows�̵߳��߳���
//---------------------------------------
class MyThread {
private:
	enum State{
		RUNNING,
		WAITING,
		CLOSE
	};
	
	HANDLE HThread;

	State state;
public:
	MyThread();
	virtual ~MyThread();

	virtual bool Start() {
		HThread = CreateThread(NULL,
			0,
			ThdMain,
			this,
			CREATE_SUSPENDED,
			NULL);
		if (HThread < 0)
			return false;
		SetStateWaiting();
		return true;
	}

	bool _Close() {
		if (IsStateClose()) {
			return true;
		}
		CloseHandle(HThread);
		SetStateClose();
		return true;
	}

	bool Close() {		//ǿ����ֹ�߳�
		TerminateThread(HThread, 0);
		WaitForSingleObject(HThread, INFINITE);
		return true;
	}

	bool Resume() {
		if (!IsStateWaiting()) {
			return false;
		}
		ResumeThread(HThread);
		SetStateRunning();
		return true;
	}

	bool Suspend() {
		if (!IsStateRunning()) {
			return false;
		}
		SuspendThread(HThread);
		SetStateWaiting();
		return true;
	}

	//������дExecute������ʵ���Լ��Ĺ���
	virtual void Execute() = 0;

	static DWORD WINAPI ThdMain(LPVOID param) {
		MyThread* s = (MyThread*)param;
		s->Execute();
		return 0;
	}

	#pragma region Change State
	void SetStateRunning() {
		state = State::RUNNING;
	}

	void SetStateWaiting() {
		state = State::WAITING;
	}

	void SetStateClose() {
		state = State::CLOSE;
	}

	bool IsStateRunning() {
		return state == State::RUNNING;
	}

	bool IsStateWaiting() {
		return state == State::WAITING;
	}

	bool IsStateClose() {
		return state == State::CLOSE;
	}
	#pragma endregion
};

//-----------------------------------
//	���ź����ķ�װ
//-----------------------------------
class MySemaphore
{
private:
	HANDLE semaphore;
public:
	MySemaphore(int init, int max);	//�ź�����ʼֵ, �ź������ֵ
	~MySemaphore() { CloseHandle(semaphore); }

	void Wait();
	void Signal();
};

//-----------------------------------
//	�Ի������ķ�װ
//-----------------------------------
class MyMutex
{
private:
	HANDLE mutex;
public:
	MyMutex() {
		mutex = CreateMutex(NULL, FALSE, NULL);
	}
	~MyMutex() {
		CloseHandle(mutex);
	}

	void Wait();
	void Signal();
};
#endif // !MYTHREAD_H_


