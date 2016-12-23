#pragma once

#ifndef MYDOWNLOADMISSION_H_
#define MYDOWNLOADMISSION_H_

#include "MyMission.h"

//--------------------------
//	œ¬‘ÿœﬂ≥Ã
//--------------------------
class MyDownLoadMission :
	public MyMission
{
private:
	static std::string downloadPath;
private:
	bool SendCommand();

	bool RecvFileNum(int &num);

	bool RecvFileName(std::string &name);

	bool RecvFileType(std::string &type);
	virtual std::string GetLocalPath() {
		return downloadPath;
	}
public:
	MyDownLoadMission();
	virtual ~MyDownLoadMission();

	virtual void Execute();
};



#endif // !MYDOWNLOADMISSION_H_
