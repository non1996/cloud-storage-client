#pragma once

#ifndef MYSOCKET_H_
#define MYSOCKET_H_

#include <string>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib") 


//------------------------------------------------
//	��װ��windows socket���׽�����
//------------------------------------------------
class MySocket
{
public:
	class Packet {
	public:
		char* buf;
		int len;
	};
	const static unsigned int MAX_BUF = 4096;	//Ĭ�Ͻ��ջ���Ϊ4KB

private:
	WSADATA wsd;
	SOCKET client;
	SOCKADDR_IN addrSrv;

	char recvBuf[MAX_BUF];
	char* extraBuf;						//���⻺����������Ҫ����4096B�Ļ�����ʱ����

	unsigned int currentContentSize;	//��¼��ǰ���������ж����ֽ�����
	unsigned int exSize;

public:
	MySocket();
	~MySocket();

	//��ʼ�����÷�����ip��ַ�Ͷ˿�
	bool init(const char* ipAddr, int port);

	//���ö��⻺��
	void SetExtraBuf(char* e, unsigned int size);

	int ConnectSrv();
	void disconnect();

	//����
	bool Send(const char* buf, int len);
	bool Receive(char** buf, int &len);

	bool Receive(Packet &p);
	
	//ѭ�����յȴ���ֱ���������е��ַ����ﵽuntil��ֵ
	bool ReceiveUntil(unsigned int until);

	//�ӻ������ж�������Ϊlen�����ݣ�����ǰҪ�ȵ���ReceiveUntil����
	bool Read(std::string &recvr, unsigned int len);

	//�������ݼ��ܺ��͸�������
	bool SendBytes(std::string toSend, const char* key);
	
	//���շ�������������Ϣ������
	bool RecvBytes(std::string &toRecv, const char* key);

	void Release();
};

#endif
