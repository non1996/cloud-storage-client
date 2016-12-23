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
//	�ļ����غ��ϴ��̵߳Ļ���
//-------------------------------------------------
class MyMission :
	public MyTransmitThreadBase
{
private:
	MyFile* file;

	unsigned long long currentSize;					

	std::string fID;					//�����ļ���id
	std::string username;				//�û���Ϣ����Կ������֤
	std::string password;
	std::string token_one;
	
	bool isComplete;					//��Ǵ����Ƿ����

	clock_t start, end;					//���㴫������
	unsigned long long bytesPerSecond;

	MyMissionManager* manager;			//״̬�ı�ʱ����֪ͨ

protected:
	MyFile* GetFile() {
		return file;
	}

	//�ӷ����������ļ��鲢�����ļ���
	bool _RecvToWriter();

	//���ļ���ȡ�ļ��鲢���͸�������
	bool SendFromReader();

	//�����ļ��������ʣ�֪ͨ����ı���ʾ��Ϣ
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

	//���÷�������ַ�Ͷ˿�
	bool InitSocket(const char* ipAddr, int port);
	//�����ļ������ļ���С
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

	//��֤�����뱻�������߳����ƣ�����׸��
	std::string EncodeUT(std::string &ut, unsigned long long usize);

	virtual std::string GetCertificationInfo();

	virtual bool InfoRight();

	bool Complete() {
		return currentSize == file->GetSize();
	}

	//�ݲ�֧�ֱ����������߳�
	void SaveProcess();

	void LoadProcess();

	virtual bool Cancel();

	virtual void Execute();

	void Release();
};

#endif // !MYMISSION_H_
