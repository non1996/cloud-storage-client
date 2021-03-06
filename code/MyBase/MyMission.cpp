#include "MyMission.h"
#include "MyEnCoder.h"
#include "MyMissionManager.h"

std::string MyMission::EncodeUT(std::string & ut, unsigned long long usize)
{
	MyEnCoder* instance = MyEnCoder::Instance();

	std::string code;

	std::string sizeUserAndToken;
	std::string sizeCode;
	std::string sizeUser;

	code = instance->Encode(ut.c_str(), GetToken().c_str(), ut.size());

	sizeUserAndToken = MyEnCoder::UllToBytes((unsigned long long)ut.size());
	sizeCode = MyEnCoder::UllToBytes((unsigned long long)code.size());
	sizeUser = MyEnCoder::UllToBytes(usize);

	return sizeUserAndToken + sizeCode + sizeUser + code;
}

bool MyMission::_RecvToWriter()
{
	if (file == 0) {
		return false;
	}
	unsigned long long totalLength = 0;	//文件大小
	unsigned long long recvLength = 0;	//已经收到的大小
	unsigned long long plength = 0;		//文件块明文长度
	unsigned long long elength = 0;		//文件块密文长度
	unsigned long long actualWrite = 0;	//实际写入文件的长度
	std::string strTotalLen;
	std::string fileBlock;

	//首先接收该文件的大小，格式为大端序的8个字节
	if (false == GetSocket()->ReceiveUntil(8)) {
		return false;
	}
	GetSocket()->Read(strTotalLen, 8);
	totalLength = MyEnCoder::BytesToUll(strTotalLen);

	StartCount();

	for (;;) {
		if (recvLength == totalLength) {
			return true;
		}
		//对于每个文件块，首先接收16个字节的长度信息
		//前8个字节表示文件块明文长度
		//后8个字节表示文件块密文长度
		if (false == GetSocket()->ReceiveUntil(16)) {
			return false;
		}
		std::string strLenInfo;
		GetSocket()->Read(strLenInfo, 16);
		plength = MyEnCoder::BytesToUll(strLenInfo.substr(0, 8));
		elength = MyEnCoder::BytesToUll(strLenInfo.substr(8, 8));
		elength -= 16;
		
		std::string code;
		if (false == GetSocket()->ReceiveUntil(elength)) {
			return false;
		}
		GetSocket()->Read(code, elength);
		fileBlock = MyEnCoder::Instance()->Decode(code.c_str(), GetToken().c_str(), elength);
		//将文件块从偏移处写入文件
		if (false == GetFile()->Write(fileBlock.c_str(), fileBlock.size(), actualWrite, recvLength)) {

		}
		recvLength += plength;

		//计速，每秒通知界面改变一次
		Count(plength);
		if (IsOneSecond()) {
			ProgressChange((float)recvLength / totalLength, bytesPerSecond);
			ClearCount();
			StartCount();
		}
	}
	return true;
}

bool MyMission::SendFromReader()
{
	if (file == 0) {
		return false;
	}
	//首先发送文件大小
	std::string len = MyEnCoder::UllToBytes(file->GetSize());
	if (false == GetSocket()->Send(len.c_str(), 8)) {
		return false;
	}

	char *sendBuf = new char[4096 * 1024];
	if (sendBuf == 0) {
		return false;
	}

	std::string sendCode;
	unsigned long long sendLength = file->GetSize();
	unsigned long long encodeBufLen = 4096 * 1024 / 3 - 16;
	unsigned long long actualLength = 0;
	unsigned long long alrSend = 0;	

	StartCount();

	for (;;) {
		std::string sizeCode, sizeRead;
		//当剩余发送长度减为0时退出
		if (sendLength <= 0) {
			delete[] sendBuf;
			CompleteChange();
			return true;
		}

		//控制发送速率，每次发送长度不超过发送缓冲的1/3。
		if (sendLength >= encodeBufLen) {
			file->Read(sendBuf, encodeBufLen, actualLength, alrSend);
		}
		else {
			file->Read(sendBuf, sendLength, actualLength, alrSend);
		}
		
		//将文件块加密，以格式  ||明文长度 | 包长度 | 密文||  发送
		sendCode = MyEnCoder::Instance()->Encode(sendBuf, GetToken().c_str(), actualLength);
		sizeCode = MyEnCoder::UllToBytes(sendCode.size() + 16);
		sizeRead = MyEnCoder::UllToBytes(actualLength);
		sizeRead += sizeCode + sendCode;

		if (false == GetSocket()->Send(sizeRead.c_str(), sizeRead.size())) {
			delete[] sendBuf;
			return false;
		}
		sendLength -= (unsigned long long)actualLength;
		alrSend += actualLength;
		
		Count(actualLength);
		if (IsOneSecond()) {
			ProgressChange((float)alrSend / (float)(alrSend + sendLength), bytesPerSecond);	
			ClearCount();
			StartCount();
		}
	}
	delete[] sendBuf;
	return true;
}

void MyMission::StartCount()
{
	start = clock();
}

bool MyMission::IsOneSecond()
{
	end = clock();
	if (end - start >= 1000) {
		return true;
	}
	return false;
}

void MyMission::Count(unsigned long long num)
{
	bytesPerSecond += num;
}

void MyMission::ClearCount()
{
	bytesPerSecond = 0;
}

void MyMission::ProgressChange(float progress, unsigned int speed)
{
	if (manager == 0) {
		return;
	}
	manager->ProgressChange(this, progress, speed);
}

void MyMission::CompleteChange()
{
	if (manager == 0) {
		return;
	}
	manager->CompleteChange(this);
}

MyMission::MyMission()
{
	file = 0;
	currentSize = 0;
	manager = 0;
	isComplete = false;
}


MyMission::~MyMission()
{
}

bool MyMission::InitSocket(const char * ipAddr, int port)
{
	return Init(ipAddr, port);
}

bool MyMission::InitFile(const char * fileName, unsigned long long size)
{
	file = new MyFile();
	if (!file) {
		return false;
	}
	file->SetName(fileName);
	file->SetSize(size);
	return true;
}

bool MyMission::InitFile()
{
	file = new MyFile();
	if (file == 0) {
		return false;
	}
	return true;
}

std::string MyMission::GetLocalPath()
{
	return "";
}

void MyMission::SetManager(MyMissionManager* m)
{
	manager = m;
}

std::string MyMission::GetCertificationInfo()
{
	return EncodeUT(username + token_one, username.size());
}

bool MyMission::InfoRight()
{
	std::string rcv;
	if (GetSocket()->RecvBytes(rcv, GetToken().c_str()) == false) {
		return false;
	}
	return true;
}

void MyMission::SaveProcess()
{
	std::cout << "save process method has not implement\n";
}

void MyMission::LoadProcess()
{
	std::cout << "load process method has not implement\n";
}

bool MyMission::Cancel()
{
	return true;
}

void MyMission::Execute()
{
	if (!IsInit()) {
		return;
	}
	while (!IsFinish()) {
		if (!Connect()) {
			DisConnect();
			ReInit();
			continue;
		}
		if (!GetTokenFromServer()) {
			DisConnect();
			ReInit();
			continue;
		}
		if (!Certification()) {
			DisConnect();
			ReInit();
			continue;
		}
		else {
			break;
		}
	}
}

void MyMission::Release()
{
	Cleaner::Delete<MyFile*>(&file);
}


