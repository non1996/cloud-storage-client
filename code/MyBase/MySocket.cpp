#include "MySocket.h"
#include "MyEnCoder.h"
#include <iostream>
#include <fstream>

bool MySocket::ReceiveUntil(unsigned int until)
{
	int len;
	for (;;) {
		if (currentContentSize >= until) {	//当接收到足够数量的数据时告知可以读取
			break;
		}
		if (extraBuf == 0) {	//判断是否有外加接收缓冲
			//在buffer中已有的内容后面接收
			len = recv(client, recvBuf + currentContentSize, MAX_BUF - currentContentSize, 0);
		}
		else{
			len = recv(client, extraBuf + currentContentSize, exSize - currentContentSize, 0);
		}
		if (len < 0) {
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
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}
	client = socket(AF_INET, SOCK_STREAM, 0);
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ipAddr);//设置服务器ip地址
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	return true;
}

void MySocket::SetExtraBuf(char * e, unsigned int size)
{
	extraBuf = e;
	exSize = size;
	std::cout << "创建额外缓冲,大小为:" << size << std::endl;
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
//	std::cout << "发送长度(socket): "<< len << std::endl;
//	std::cout << std::endl;
//	for (int i = 0; i < 32; ++i) {
//		printf("%d ", (unsigned char)buf[len - 32 + i]);
//	}
//	std::cout << std::endl;
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
	//如果设置了就用额外缓冲
	std::cout << "使用额外缓冲\n";
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
//		std::cout << "缓冲区剩余长度：" << currentContentSize << std::endl;
		return true;
	}
	//如果设置了就用额外缓冲
//	std::cout << "额外\n";
	recvr.assign(extraBuf, len);
	currentContentSize -= len;
	memcpy(extraBuf, extraBuf + len, currentContentSize);
	return true;
}

bool MySocket::SendBytes(std::string toSend, const char * key)
{
	unsigned long long totalLength = toSend.size();
	unsigned long long alrSend = 0;		//已经发送的数据长度
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

	//先发送要传输数据的总长度
	return true;
}
/*
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
	//获取要接收的明文的总长度
	if (p.len < 8) {
		std::cout << "收到小于8字节的长度\n";
		return false;
	}
	totalLength = MyEnCoder::BytesToUll(std::string(p.buf, p.len));
	std::cout << "总明文长度: " << totalLength << std::endl;
	for (;;) {
		if (totalLength <= 0) {
			std::cout << "return recv\n";
			return true;
		}
		if (false == Receive(p)) {
			return false;
		}
		//		std::cout << "packet length: " << p.len << std::endl;
		std::string str_p(p.buf, p.len);
		//获得该包明文长度
		plength = MyEnCoder::BytesToUll(str_p.substr(0, 8));
		//包密文长度+16
		elength = MyEnCoder::BytesToUll(str_p.substr(8, 8));
		elength -= 16;
		std::cout << "包长:" << str_p.size() << std::endl;
		std::cout << "包明文长度为：" << plength << std::endl;
		std::cout << "包密文长度:" << elength << std::endl;
		//解密
		temp = MyEnCoder::Instance()->Decode(str_p.substr(16).c_str(), key, elength);
		totalLength -= plength;
		std::cout << "total length left: "<< totalLength << std::endl;
		toRecv += temp;
	}
	return true;
}*/

bool MySocket::RecvBytes(std::string & toRecv, const char * key)
{
	std::string str_totalLength;
	unsigned long long totalLength = 0;
	unsigned long long plength = 0;
	unsigned long long elength = 0;
	std::string plainText;

	if (false == ReceiveUntil(8)) {
		std::cout << "未收到8字节\n";
		return false;
	}
	//获取要接收的明文的总长度
	Read(str_totalLength, 8);
	totalLength = MyEnCoder::BytesToUll(str_totalLength);
//	std::cout << "总明文长度: " << totalLength << std::endl;
	
	for (;;) {
		if (totalLength == 0) {
//			std::cout << "接收结束\n";
			return true;
		}
		if (false == ReceiveUntil(16)) {
			std::cout << "接收不到16个字节的长度字段\n";
			return false;
		}
		std::string strLenInfo;
		Read(strLenInfo, 16);
		//获得该包明文长度
		plength = MyEnCoder::BytesToUll(strLenInfo.substr(0, 8));
		//包密文长度+16
		elength = MyEnCoder::BytesToUll(strLenInfo.substr(8, 8));
		elength -= 16;
//		std::cout << "包明文长度为：" << plength << std::endl;
//		std::cout << "包密文长度:" << elength << std::endl;
		if (false == ReceiveUntil(elength)) {
			std::cout << "接收不到足够长的密文\n";
			return false;
		}
		std::string code;
		Read(code, elength);
		//解密
		plainText = MyEnCoder::Instance()->Decode(code.c_str(), key, elength);
		totalLength -= plength;
//		std::cout << "total length left: " << totalLength << std::endl;
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
