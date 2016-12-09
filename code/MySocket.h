#pragma once

#ifndef MYSOCKET_H_
#define MYSOCKET_H_

#include <string>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib") 

class MySocket
{
public:
	class Packet {
	public:
		char* buf;
		int len;
	};
	const static unsigned int MAX_BUF = 4096;
private:

	WSADATA wsd;
	SOCKET client;
	SOCKADDR_IN addrSrv;//·þÎñÆ÷µØÖ·
	char recvBuf[MAX_BUF];


	unsigned int currentContentSize;
	
	char* extraBuf;
	unsigned int exSize;

public:
	MySocket();
	~MySocket();

	bool init(const char* ipAddr, int port);

	void SetExtraBuf(char* e, unsigned int size);

	int connect_to_srv();
	void disconnect();

	bool Send(const char* buf, int len);
	bool Receive(char** buf, int &len);
	bool Receive(Packet &p);
	bool Read(std::string &recvr, unsigned int len);
	
	bool ReceiveUntil(unsigned int until);

	//use to suit server
	bool SendBytes(std::string toSend, const char* key);
	bool RecvBytes(std::string &toRecv, const char* key);
	bool _RecvBytes(std::string &toRecv, const char* key);

	void Release();
};

#endif
