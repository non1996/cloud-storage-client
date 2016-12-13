#pragma once

#ifndef MYMISSION_H_
#define MYMISSION_H_

#include "Cleaner.h"
#include "MyTransmitThreadBase.h"
#include "MyFile.h"

#include <ctime>
#include <iostream>

class MyMissionManager;

//-------------------------------------------------
//	base class of file transfer mission
//-------------------------------------------------
class MyMission :
	public MyTransmitThreadBase
{
private:
	MyFile* file;

	unsigned long long currentSize;					

	std::string fID;		//file ID on server
	std::string username;
	std::string password;
	std::string token_one;

	clock_t start, end;
	unsigned long long bytesPerSecond;

	MyMissionManager* manager; //listener

protected:
	MyFile* GetFile() {
		return file;
	}

	std::string EncodeUT(std::string &ut, unsigned long long usize);

	//bool RecvToWriter();
	bool _RecvToWriter();

	bool SendFromReader();

	#pragma region calculate speed and tell control
	void StartCount();
	bool IsOneSecond();
	void Count(unsigned long long num);
	void ClearCount();

//	void SpeedChange(unsigned int speed);
	void ProgressChange(float progress, unsigned int speed);
	void CompleteChange();
	#pragma endregion

public:
	MyMission();
	virtual ~MyMission();

	//设置服务器地址和端口
	bool InitSocket(const char* ipAddr, int port);
	//设置文件名和文件大小
	bool InitFile(const char* fileName, unsigned long long size);
	bool InitFile();

	void SetUsername(std::string &username) {
		this->username = username;
	}

	void SetPassword(std::string &pw) {
		password = pw;
	}

	std::string &GetPassword() {
		return password;
	}

	virtual std::string &GetLocalPath() = 0;

	void SetTokenOne(std::string &token) {
		token_one = token;
	}

	void SetManager(MyMissionManager* manager);

	unsigned long long GetFileSize() {
		return file->GetSize();
	}

	unsigned long long GetCurrentSize() {
		return currentSize;
	}

	void SetFileSize(unsigned long long size) {
		file->SetSize(size);
	}

	void IncCurrentSize(unsigned long long add) {
		currentSize += add;
	}

	float GetProcess() {
		if (file == 0) {
			return 0.0f;
		}
		if (file->GetSize() == 0) {
			return 0.0f;
		}
		return (float)currentSize / (float)file->GetSize();
	}

	void SetFileID(std::string &id) {
		fID = id;
	}

	std::string &GetFileID() {
		return fID;
	}

	virtual std::string GetCertificationInfo();

	virtual bool InfoRight();

	bool Complete() {
		return currentSize == file->GetSize();
	}

	void SaveProcess();

	void LoadProcess();

	void OutputSituation();

	virtual bool Cancel();

	virtual void Execute();

	void Release();
};

#endif // !MYMISSION_H_

