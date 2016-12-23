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
//	文件下载和上传线程的基类
//-------------------------------------------------
class MyMission :
	public MyTransmitThreadBase
{
private:
	MyFile* file;

	unsigned long long currentSize;					

	std::string fID;					//保存文件的id
	std::string username;				//用户信息和秘钥用于验证
	std::string password;
	std::string token_one;
	
	bool isComplete;					//标记传输是否完成

	clock_t start, end;					//计算传输速率
	unsigned long long bytesPerSecond;

	MyMissionManager* manager;			//状态改变时向上通知

protected:
	MyFile* GetFile() {
		return file;
	}

	//从服务器接收文件块并存入文件中
	bool _RecvToWriter();

	//从文件读取文件块并发送给服务器
	bool SendFromReader();

	//计算文件传输速率，通知界面改变显示信息
	#pragma region calculate speed and tell control
	void StartCount();
	bool IsOneSecond();
	void Count(unsigned long long num);
	void ClearCount();

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

	bool IsComplete() {
		return isComplete;
	}

	#pragma region Get
	float GetProcess() {
		if (file == 0) {
			return 0.0f;
		}
		if (file->GetSize() == 0) {
			return 0.0f;
		}
		return (float)currentSize / (float)file->GetSize();
	}

	std::string &GetPassword() {
		return password;
	}

	virtual std::string GetLocalPath() = 0;

	unsigned long long GetFileSize() {
		return file->GetSize();
	}

	unsigned long long GetCurrentSize() {
		return currentSize;
	}

	std::string &GetFileID() {
		return fID;
	}
	#pragma endregion

	#pragma region Set
	void SetUsername(std::string &username) {
		this->username = username;
	}

	void SetPassword(std::string &pw) {
		password = pw;
	}

	void SetComplete(bool c) {
		isComplete = c;
	}

	void SetTokenOne(std::string &token) {
		token_one = token;
	}

	void SetManager(MyMissionManager* manager);

	void SetFileSize(unsigned long long size) {
		file->SetSize(size);
	}

	void IncCurrentSize(unsigned long long add) {
		currentSize += add;
	}

	void SetFileID(std::string &id) {
		fID = id;
	}
	#pragma endregion

	//验证过程与被动监听线程类似，不做赘述
	std::string EncodeUT(std::string &ut, unsigned long long usize);

	virtual std::string GetCertificationInfo();

	virtual bool InfoRight();

	bool Complete() {
		return currentSize == file->GetSize();
	}

	//暂不支持保存与重启线程
	void SaveProcess();

	void LoadProcess();

	virtual bool Cancel();

	virtual void Execute();

	void Release();
};

#endif // !MYMISSION_H_
