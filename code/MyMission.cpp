#include "MyMission.h"
#include "MyEnCoder.h"

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

bool MyMission::RecvToWriter()
{
	if (file == 0) {
		return false;
	}
	MySocket::Packet p;
	unsigned long long totalLength = 0;
	unsigned long long recvLength = 0;
	unsigned long long plength = 0;
	unsigned long long elength = 0;
	unsigned long long actualWrite = 0;
	std::string fileBlock;

	if (false == GetSocket()->Receive(p)) {
		return false;
	}
	if (p.len < 8) {
		return false;
	}
	totalLength = MyEnCoder::BytesToUll(std::string(p.buf, 8));
	std::cout << "file " << GetFile()->GetName() << " size: " << totalLength << std::endl;

	for (;;) {
		if (recvLength == totalLength) {
			return true;
		}
		if (false == GetSocket()->Receive(p)) {
			return false;
		}
		std::string str_p(p.buf, p.len);
		plength = MyEnCoder::BytesToUll(str_p.substr(0, 8));
		elength = MyEnCoder::BytesToUll(str_p.substr(8, 8));
		fileBlock = MyEnCoder::Instance()->Decode(str_p.substr(16).c_str(), GetToken().c_str(), str_p.size() - 16);
		if (false == GetFile()->Write(fileBlock.c_str(), fileBlock.size(), actualWrite, recvLength)) {
			std::cout << "write file error: offset is: " << recvLength << std::endl;
		}
		recvLength += plength;
	}
	return true;
}


bool MyMission::SendFromReader()
{
	if (file == 0) {
		return false;
	}
	std::string len = MyEnCoder::UllToBytes(file->GetSize());
	if (false == GetSocket()->Send(len.c_str(), 8)) {
		return false;
	}

	char sendBuf[4096 * 1024];
	std::string sendCode;
	unsigned long long sendLength = file->GetSize();
	unsigned long long encodeBufLen = sizeof(sendBuf) / 3 - 16;
	unsigned long long actualLength = 0;
	unsigned long long alrSend = 0;	

	for (;;) {
		std::string sizeCode, sizeRead;
		if (sendLength <= 0) {
			return true;
		}
		if (sendLength >= encodeBufLen) {
			file->Read(sendBuf, encodeBufLen, actualLength, alrSend);
		}
		else {
			file->Read(sendBuf, sendLength, actualLength, alrSend);
		}
		sendCode = MyEnCoder::Instance()->Encode(sendBuf, GetToken().c_str(), actualLength);
		sizeCode = MyEnCoder::UllToBytes(sendCode.size());
		sizeRead = MyEnCoder::UllToBytes(actualLength);
		sizeRead += sizeCode + sendCode;
		if (false == GetSocket()->Send(sizeRead.c_str(), sizeRead.size())) {
			return false;
		}
		sendLength -= (unsigned long long)actualLength;
		alrSend += actualLength;
	}

	return true;
}

MyMission::MyMission()
{
	file = 0;
	currentSize = 0;
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

std::string MyMission::GetCertificationInfo()
{
	return EncodeUT(username + token_one, username.size());
}

bool MyMission::InfoRight()
{
	std::string rcv;
	if (GetSocket()->RecvBytes(rcv, GetToken().c_str()) == false) {
		SetConnect(false);
		std::cout << "info wrong-----------server broke the link\n";
		return false;
	}
	std::cout << "info right\n";
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

void MyMission::OutputSituation()
{
	std::cout	<< file->GetName() << "\t"
				<< file->GetSize() << "\t"
				<< GetProcess() << std::endl;
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
			std::cout << "connect failed --------transmit thread" << std::endl;
			Sleep(10000);
			continue;
		}
		std::cout << "connect to server --------transmit thread" << std::endl;
		if (!GetTokenFromServer()) {
			Sleep(10000);
			continue;
		}
		std::cout << "get token " << GetToken() << " --------transmit thread" << std::endl;
		if (!Certification()) {
			std::cout << "connect broke1 --------transmit thread" << std::endl;
			Sleep(10000);
			continue;
		}
		else {
			break;
		}
	}
	std::cout << "log in! enter receive loop --------recv thread" << std::endl;
}

void MyMission::Release()
{
	Cleaner::Delete<MyFile*>(&file);
}


