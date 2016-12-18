#include "MySocket.h"
#include "MyEnCoder.h"
#include <iostream>
#include <fstream>

bool MySocket::ReceiveUntil(unsigned int until)
{
	int len;
	for (;;) {
		if (currentContentSize >= until) {	//�����յ��㹻����������ʱ��֪���Զ�ȡ
			break;
		}
		if (extraBuf == 0) {	//�ж��Ƿ�����ӽ��ջ���
			//��buffer�����е����ݺ������
			len = recv(client, recvBuf + currentContentSize, MAX_BUF - currentContentSize, 0);
		}
		else{
			len = recv(client, extraBuf + currentContentSize, exSize - currentContentSize, 0);
		}
		if (len <= 0) {
			return false;
		}
		currentContentSize += (unsigned int)len;
	}
	return true;
}

MySocket::MySocket()
{
	extraBuf = 0;
	exSize = 0;
	currentContentSize = 0;
}


MySocket::~MySocket()
{
	delete[] extraBuf;
}

bool MySocket::init(const char* ipAddr, int port)
{
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0){
		return false;
	}
	client = socket(AF_INET, SOCK_STREAM, 0);
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ipAddr);//���÷�����ip��ַ
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	return true;
}

void MySocket::SetExtraBuf(char * e, unsigned int size)
{
	extraBuf = e;
	exSize = size;
}

int MySocket::connect_to_srv()
{
	return connect(client, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
}

void MySocket::disconnect()
{
	closesocket(client);
	WSACleanup();
}

bool MySocket::Send(const char * buf, int len)
{
	int re;
	re = send(client, buf, len, 0);
	
	if (re == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool MySocket::Receive(char** buf, int &len)
{
	int re;
	re = recv(client, recvBuf, MAX_BUF, 0);
	if (re <= 0) {
		return false;
	}
//	memcpy(buf, recvBuf, re);
	(*buf) = recvBuf;
	return true;
}

bool MySocket::Receive(Packet & p)
{
	int len;
	if (extraBuf == 0) {
		len = recv(client, recvBuf, MAX_BUF, 0);
		if (len <= 0) {
			return false;
		}
		p.buf = recvBuf;
		p.len = len;
		return true;
	}
	//��������˾��ö��⻺��
	len = recv(client, extraBuf, exSize, 0);
	if (len <= 0) {
		return false;
	}
	p.buf = extraBuf;
	p.len = len;
	return true;
}

bool MySocket::Read(std::string &recvr, unsigned int len)
{
	if (extraBuf == 0) {
		recvr.assign(recvBuf, len);
		currentContentSize -= len;
		memcpy(recvBuf, recvBuf + len, currentContentSize);
		return true;
	}
	recvr.assign(extraBuf, len);
	currentContentSize -= len;
	memcpy(extraBuf, extraBuf + len, currentContentSize);
	return true;
}

bool MySocket::SendBytes(std::string toSend, const char * key)
{
	unsigned long long totalLength = toSend.size();
	unsigned long long alrSend = 0;		//�Ѿ����͵����ݳ���
	std::string str_plength, str_tlength;
	std::string encode;

	if (false == Send(MyEnCoder::UllToBytes((unsigned long long)toSend.size()).c_str(), 8)) {
		return false;
	}

	str_plength = MyEnCoder::UllToBytes((unsigned long long)toSend.size());
	encode = MyEnCoder::Instance()->Encode(toSend.c_str(), key, toSend.size());
	str_tlength = MyEnCoder::UllToBytes((unsigned long long)(encode.size() + 16));

	std::string temp;
	temp += str_plength + str_tlength + encode;
	if (false == Send(temp.c_str(), temp.size())) {
		return false;
	}

	//�ȷ���Ҫ�������ݵ��ܳ���
	return true;
}

bool MySocket::RecvBytes(std::string & toRecv, const char * key)
{
	std::string str_totalLength;
	unsigned long long totalLength = 0;
	unsigned long long plength = 0;
	unsigned long long elength = 0;
	std::string plainText;

	if (false == ReceiveUntil(8)) {
		return false;
	}
	//��ȡҪ���յ����ĵ��ܳ���
	Read(str_totalLength, 8);
	totalLength = MyEnCoder::BytesToUll(str_totalLength);
	
	for (;;) {
		if (totalLength == 0) {
			return true;
		}
		if (false == ReceiveUntil(16)) {
			return false;
		}
		std::string strLenInfo;
		Read(strLenInfo, 16);
		//��øð����ĳ���
		plength = MyEnCoder::BytesToUll(strLenInfo.substr(0, 8));
		//�����ĳ���+16
		elength = MyEnCoder::BytesToUll(strLenInfo.substr(8, 8));
		elength -= 16;
		if (false == ReceiveUntil(elength)) {
			return false;
		}
		std::string code;
		Read(code, elength);
		//����
		plainText = MyEnCoder::Instance()->Decode(code.c_str(), key, elength);
		totalLength -= plength;
		toRecv += plainText;
	}
	return true;
}

void MySocket::Release()
{
	disconnect();
	delete[] extraBuf;
	extraBuf = 0;
}
