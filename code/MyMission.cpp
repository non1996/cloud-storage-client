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
	std::cout << "文件" << GetFile()->GetName() << "的大小为: " << totalLength << std::endl;

	for (;;) {
		if (recvLength == totalLength) {
			return true;
		}
		if (false == GetSocket()->Receive(p)) {
			return false;
		}
		std::string str_p(p.buf, p.len);
		plength = MyEnCoder::BytesToUll(str_p.substr(0, 8));
		std::cout << "文件块明文长度:" << plength;
		elength = MyEnCoder::BytesToUll(str_p.substr(8, 8));
		std::cout << "文件块密文长度(已减16):" << elength;
		fileBlock = MyEnCoder::Instance()->Decode(str_p.substr(16).c_str(), GetToken().c_str(), elength);
		if (false == GetFile()->Write(fileBlock.c_str(), fileBlock.size(), actualWrite, recvLength)) {
			std::cout << "write file error: offset is: " << recvLength << std::endl;
		}
		recvLength += plength;
	}
	return true;
}

bool MyMission::_RecvToWriter()
{
	if (file == 0) {
		return false;
	}
	unsigned long long totalLength = 0;
	unsigned long long recvLength = 0;
	unsigned long long plength = 0;
	unsigned long long elength = 0;
	unsigned long long actualWrite = 0;
	std::string strTotalLen;
	std::string fileBlock;

	if (false == GetSocket()->ReceiveUntil(8)) {
		return false;
	}
	GetSocket()->Read(strTotalLen, 8);
	totalLength = MyEnCoder::BytesToUll(strTotalLen);
	std::cout << "文件" << GetFile()->GetName() << "的大小为: " << totalLength << std::endl;

	for (;;) {
		if (recvLength == totalLength) {
			return true;
		}
		if (false == GetSocket()->ReceiveUntil(16)) {
			return false;
		}
		std::string strLenInfo;
		GetSocket()->Read(strLenInfo, 16);
		plength = MyEnCoder::BytesToUll(strLenInfo.substr(0, 8));
		std::cout << "文件块明文长度:" << plength;
		elength = MyEnCoder::BytesToUll(strLenInfo.substr(8, 8));
		elength -= 16;
		std::cout << "文件块密文长度(已减16):" << elength;
		
		std::string code;
		if (false == GetSocket()->ReceiveUntil(elength)) {
			return false;
		}
		GetSocket()->Read(code, elength);
		fileBlock = MyEnCoder::Instance()->Decode(code.c_str(), GetToken().c_str(), elength);
		if (false == GetFile()->Write(fileBlock.c_str(), fileBlock.size(), actualWrite, recvLength)) {
			std::cout << "write file error: offset is: " << recvLength << std::endl;
		}
		recvLength += plength;
		std::cout << "文件总大小:" << totalLength << ",已下载大小:" << recvLength << std::endl;
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

	char *sendBuf = new char[4096 * 1024];
	if (sendBuf == 0) {
		std::cout << "发送缓冲创建失败\n";
		return false;
	}

	std::string sendCode;
	unsigned long long sendLength = file->GetSize();
	std::cout << "文件长度为" << sendLength << std::endl;
	unsigned long long encodeBufLen = 4096 * 1024 / 3 - 16;
	unsigned long long actualLength = 0;
	unsigned long long alrSend = 0;	

	for (;;) {
		std::string sizeCode, sizeRead;
		if (sendLength <= 0) {
			std::cout << "发送完成\n";
			delete[] sendBuf;
			return true;
		}
		if (sendLength >= encodeBufLen) {
			file->Read(sendBuf, encodeBufLen, actualLength, alrSend);
		}
		else {
			file->Read(sendBuf, sendLength, actualLength, alrSend);
		}
		std::cout << "实际读出的文件大小: " << actualLength << std::endl;
		for (int i = 0; i < 16; ++i) {
			printf("%d ", (unsigned char)sendBuf[actualLength - 16 + i]);
		}
		std::cout << "\n为文件块加密\n";
		sendCode = MyEnCoder::Instance()->Encode(sendBuf, GetToken().c_str(), actualLength);
		sizeCode = MyEnCoder::UllToBytes(sendCode.size() + 16);
		sizeRead = MyEnCoder::UllToBytes(actualLength);
		sizeRead += sizeCode + sendCode;

		std::cout << std::endl;
		for (int i = 0; i < 32; ++i) {
			printf("%d ", (unsigned char)sendCode[actualLength - 32 + i]);
		}
		std::cout << std::endl;

		if (false == GetSocket()->Send(sizeRead.c_str(), sizeRead.size())) {
			std::cout << "发送失败\n";
			delete[] sendBuf;
			return false;
		}
		sendLength -= (unsigned long long)actualLength;
		alrSend += actualLength;
	}
	delete[] sendBuf;
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

bool MyMission::InitFile()
{
	file = new MyFile();
	if (file == 0) {
		return false;
	}
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
	std::cout << "log in! enter receive loop --------transmit thread" << std::endl;
}

void MyMission::Release()
{
	Cleaner::Delete<MyFile*>(&file);
}


