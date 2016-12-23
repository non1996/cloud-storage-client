#pragma once

#ifndef MYSOCKET_H_
#define MYSOCKET_H_

#include <string>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib") 


//------------------------------------------------
//	封装了windows socket的套接字类
//------------------------------------------------
class MySocket
{
public:
	class Packet {
	public:
		char* buf;
		int len;
	};
	const static unsigned int MAX_BUF = 4096;	//默认接收缓冲为4KB

private:
	WSADATA wsd;
	SOCKET client;
	SOCKADDR_IN addrSrv;

	char recvBuf[MAX_BUF];
	char* extraBuf;						//额外缓冲区，当需要大于4096B的缓冲区时设置

	unsigned int currentContentSize;	//记录当前缓冲区中有多少字节数据
	unsigned int exSize;

public:
	MySocket();
	~MySocket();

	//初始化设置服务器ip地址和端口
	bool init(const char* ipAddr, int port);

	//设置额外缓冲
	void SetExtraBuf(char* e, unsigned int size);

	int ConnectSrv();
	void disconnect();

	//废弃
	bool Send(const char* buf, int len);
	bool Receive(char** buf, int &len);

	bool Receive(Packet &p);
	
	//循环接收等待，直到缓冲区中的字符数达到until的值
	bool ReceiveUntil(unsigned int until);

	//从缓冲区中读出长度为len的数据，调用前要先调用ReceiveUntil函数
	bool Read(std::string &recvr, unsigned int len);

	//发送数据加密后发送给服务器
	bool SendBytes(std::string toSend, const char* key);
	
	//接收服务器发来的信息并解密
	bool RecvBytes(std::string &toRecv, const char* key);

	void Release();
};

#endif
