#pragma once

#ifndef MYTHREAD_H_
#define MYTHREAD_H_

#include <Windows.h>
#include <iostream>

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
		std::cout << "this thread is closed\n";
		SetStateClose();
		return true;
	}

	bool Close() {		//强行终止线程
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

#endif // !MYTHREAD_H_


