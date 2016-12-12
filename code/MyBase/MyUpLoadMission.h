#pragma once

#ifndef MYUPLOADMISSION_H_
#define MYUPLOADMISSION_H_

#include "MyMission.h"
class MyUpLoadMission :
	public MyMission
{
private:
	enum ResponceType{
		SUCCESS,		//200
		START,			//201
		FAILED,			//203
		CMDNOTVALID,	//300
		FILENOTTOUCH,	//301
		MD5NOTMATCH,	//403
		SERVERERROR		//500
	};
private:
	std::string fileName;
	std::string localPath;
	std::string netPath;
	unsigned long long uID;
	char *sendBuf;

	bool SendTouchCommand();	//create a new file in server

	bool SendCommand();

	ResponceType RecvResponse();
public:
	MyUpLoadMission();
	virtual ~MyUpLoadMission();

	void SetUID(unsigned long long uID) {
		this->uID = uID;
	}

	void SetFileName(std::string &fn) {
		fileName = fn;
	}

	void SetLocalPath(std::string &lp) {
		localPath = lp;
	}

	void SetNetPath(std::string & np) {
		netPath = np;
	}

	std::string &GetLocalPath() {
		return localPath;
	}

	virtual void Execute();
};

#endif


