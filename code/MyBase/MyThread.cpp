#include "MyThread.h"



MyThread::MyThread()
{
	state = State::CLOSE;
}


MyThread::~MyThread()
{
}

MySemaphore::MySemaphore(int init, int max)
{
	semaphore = CreateSemaphore(NULL, init, max, NULL);
}

void MySemaphore::Wait()
{
	WaitForSingleObject(semaphore, INFINITE);
}

void MySemaphore::Signal()
{
	ReleaseSemaphore(semaphore, 1, NULL);
}

void MyMutex::Wait()
{
	WaitForSingleObject(mutex, INFINITE);
}

void MyMutex::Signal()
{
	ReleaseMutex(mutex);
}
