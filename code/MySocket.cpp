#include "MySocket.h"
#include "MyEnCoder.h"
#include <iostream>

MySocket::MySocket()
{
}


MySocket::~MySocket()
{
}

bool MySocket::init(const char* ipAddr, int port)
{
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}
	client = socket(AF_INET, SOCK_STREAM, 0);
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ipAddr);//���÷�����ip��ַ
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);

	return true;
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
	len = recv(client, recvBuf, MAX_BUF, 0);
	if (len <= 0) {
		return false;
	}
	p.buf = recvBuf;
	p.len = len;
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
	MySocket::Packet p;
	unsigned long long totalLength = 0;
	unsigned long long plength = 0;
	unsigned long long elength = 0;
	std::string temp;

	if (false == Receive(p)) {
		return false;
	}
	//��ȡҪ���յ����ĵ��ܳ���
	if (p.len < 8) {
		std::cout << "�յ�С��8�ֽڵĳ���\n";
		return false;
	}
	totalLength = MyEnCoder::BytesToUll(std::string(p.buf, p.len));
	std::cout << totalLength << std::endl;
	//	std::cout << "�����ĳ���: " << totalLength << std::endl;
	for (;;) {
		if (totalLength == 0) {
			std::cout << "return recv\n";
			return true;
		}
		if (false == Receive(p)) {
			return false;
		}
		//		std::cout << "packet length: " << p.len << std::endl;
		std::string str_p(p.buf, p.len);
		//��øð����ĳ���
		plength = MyEnCoder::BytesToUll(str_p.substr(0, 8));
		elength = MyEnCoder::BytesToUll(str_p.substr(8, 8));
		//����
		temp = MyEnCoder::Instance()->Decode(str_p.substr(16).c_str(), key, str_p.substr(16).size());
		totalLength -= plength;
		std::cout << "total length: "<< totalLength << std::endl;
		toRecv += temp;
	}
	return true;
}

void MySocket::Release()
{
	disconnect();
}
