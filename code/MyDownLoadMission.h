#pragma once

#ifndef MYDOWNLOADMISSION_H_
#define MYDOWNLOADMISSION_H_

#include "MyMission.h"
class MyDownLoadMission :
	public MyMission
{
private:
	bool SendCommand();

	bool RecvFileNum(int &num);

	bool RecvFileName(std::string &name);

	bool RecvFileType(std::string &type);

public:
	MyDownLoadMission();
	virtual ~MyDownLoadMission();

	virtual void Execute();
};

#endif // !MYDOWNLOADMISSION_H_
