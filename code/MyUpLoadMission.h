#pragma once

#ifndef MYUPLOADMISSION_H_
#define MYUPLOADMISSION_H_

#include "MyMission.h"
class MyUpLoadMission :
	public MyMission
{
private:
	enum ResponceType{
		SUCCESS,
		EXIST,
		FAILED
	};
private:
	std::string fileName;
	std::string localPath;
	std::string netPath;

	bool SendCommand();

	ResponceType RecvResponse();
public:
	MyUpLoadMission();
	virtual ~MyUpLoadMission();

	void SetFileName(std::string &fn) {
		fileName = fn;
	}

	void SetLocalPath(std::string &lp) {
		localPath = lp;
	}

	void SetNetPath(std::string & np) {
		netPath = np;
	}

	virtual void Execute();
};

#endif


