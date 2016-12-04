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
public:
	MySocket();
	~MySocket();

	bool init(const char* ipAddr, int port);
	int connect_to_srv();
	void disconnect();
//	void send_data(std::string & s);
//	int receive_data(std::string & r);

	bool Send(const char* buf, int len);
	bool Receive(char** buf, int &len);
	bool Receive(Packet &p);

	//use to suit server
	bool SendBytes(std::string toSend, const char* key);
	bool RecvBytes(std::string &toRecv, const char* key);

	void Release();
};

#endif
