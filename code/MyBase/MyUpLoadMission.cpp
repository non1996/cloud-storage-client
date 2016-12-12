#include "MyUpLoadMission.h"
#include "MyEnCoder.h"
#include "MyCommand.h"
#include <sstream>

bool MyUpLoadMission::SendTouchCommand()
{
	MyTouchCommand temp(fileName, netPath);
	std::string resp;
	if (false == GetSocket()->SendBytes(temp.ToString(), GetToken().c_str())) {
		return false;
	}
	if (false == GetSocket()->RecvBytes(resp, GetToken().c_str())) {
		return false;
	}
	if (resp != "200") {
		std::cout << "touch 错误" << std::endl;
		return false;
	}
	if (false == GetSocket()->RecvBytes(resp, GetToken().c_str())) {
		return false;
	}
	temp.GetServerResponse(resp.c_str(), resp.size());
	uID = temp.GetUID();
	return true;
}

bool MyUpLoadMission::SendCommand()
{
	std::string cmd;
	std::string fileMD5;

	std::stringstream ss_size, ss_uID;
	unsigned long long size = 0;
	MyFile::Size((localPath + fileName).c_str(), size);
	ss_size << size;
	ss_uID << uID;
	std::cout << localPath << fileName << ss_size.str() << std::endl;

	fileMD5 = MyEnCoder::PrivateFileMD5((localPath + fileName));
	if (fileMD5.size() == 0) {
		return false;
	}
	std::cout << "file md5 is:" << fileMD5 << std::endl;
	cmd += "put+"					//PUT <SEP> UID <SEP> MD5
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
		std::cout << "传输链接断开\n";
		return;
	}
	ResponceType r = RecvResponse();
	if (r == ResponceType::START) {
		std::cout << "开始传输\n";
		if (false == InitFile((localPath + fileName).c_str(), 0)) {
			std::cout << "文件流初始化失败\n";
		}else if (false == GetFile()->OpenRead()) {
			std::cout << "文件打开失败\n";
		}else if (false == SendFromReader()) {
			std::cout << "上传失败\n";
		}else {
			std::cout << "上传成功\n";
			std::string res;
			GetSocket()->RecvBytes(res, GetToken().c_str());
			std::cout << "结果码:" << MyEnCoder::BytesToUll(res) << std::endl;
		}
		SetFinish();
		GetSocket()->disconnect();
		GetFile()->Close();
		return;
	}else if (r == ResponceType::SUCCESS) {
		std::cout << "文件已存在,上传成功\n";
		CompleteChange();
		GetSocket()->disconnect();
		return;
	}else {
		std::cout << "上传失败，错误码为:" << r << std::endl;
		GetSocket()->disconnect();
		return;
	}
}