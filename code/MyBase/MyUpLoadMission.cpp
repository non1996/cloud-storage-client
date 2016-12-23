#include "MyUpLoadMission.h"
#include "MyEnCoder.h"
#include "MyCommand.h"
#include <sstream>

bool MyUpLoadMission::SendCommand()
{
	std::string cmd;
	std::string fileMD5;

	std::stringstream ss_size, ss_uID;
	unsigned long long size = 0;
	MyFile::Size((localPath + fileName).c_str(), size);
	ss_size << size;
	ss_uID << uID;

	fileMD5 = MyEnCoder::PrivateFileMD5((localPath + fileName));
	if (fileMD5.size() == 0) {
		return false;
	}
	//发送格式 [PUT][+][UID][+][MD5]
	cmd += "put+"					
		+ ss_uID.str() + "+"
		+ ss_size.str() + "+"
		+ fileMD5;
	if (false == GetSocket()->SendBytes(cmd, GetToken().c_str())) {
		return false;
	}
	return true;
}

MyUpLoadMission::ResponceType MyUpLoadMission::RecvResponse()
{
	std::string rsp;
	if (false == GetSocket()->RecvBytes(rsp, GetToken().c_str())) {
		return FAILED;
	}
	if (rsp.size() != 8) {
		return FAILED;
	}
	unsigned long long rspNum = MyEnCoder::BytesToUll(rsp);
	if (rspNum == 200) {
		return SUCCESS;
	}
	if (rspNum == 201) {
		return START;
	}
	if (rspNum == 203) {
		return FAILED;
	}
	if (rspNum == 300) {
		return CMDNOTVALID;
	}
	if (rspNum == 301) {
		return FILENOTTOUCH;
	}
	if (rspNum == 403) {
		return MD5NOTMATCH;
	}
	if (rspNum == 500) {
		return SERVERERROR;
	}
	return FAILED;
}

MyUpLoadMission::MyUpLoadMission()
{
}


MyUpLoadMission::~MyUpLoadMission()
{
	delete[] sendBuf;
}

void MyUpLoadMission::Execute()
{
	MyMission::Execute();

	if (false == SendCommand()) {
		return;
	}
	ResponceType r = RecvResponse();
	//服务器允许传输且服务器上不存在相同文件，则开始传输
	if (r == ResponceType::START) {
		if (false == InitFile((localPath + fileName).c_str(), 0)) {
			return;
		}else if (false == GetFile()->OpenRead()) {
			return;
		}else if (false == SendFromReader()) {

		}else {
			std::string res;
			GetSocket()->RecvBytes(res, GetToken().c_str());
		}
		SetFinish();
		CompleteChange();
		SetComplete(true);
		GetSocket()->disconnect();
		GetFile()->Close();
		return;
	}
	//如果服务器上已有该文件，则秒传成功
	else if (r == ResponceType::SUCCESS) {
		SetFinish();
		CompleteChange();
		SetComplete(true);
		GetSocket()->disconnect();
		return;
	}else {
		GetSocket()->disconnect();
		return;
	}
}
