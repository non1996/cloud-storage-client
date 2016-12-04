#include "MyUpLoadMission.h"
#include "MyEnCoder.h"


bool MyUpLoadMission::SendCommand()
{
	std::string cmd;
	std::string fileMD5;
	fileMD5 = MyEnCoder::FileMD5(fileName);
	cmd += "put+"
		+ std::string("1") + "+"
		+ fileMD5 + "+"
		+ fileName + "+"
		+ netPath;
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
	if (rsp == "200") {
		return SUCCESS;
	}
	if (rsp == "300") {
		return EXIST;
	}
	return FAILED;
}

MyUpLoadMission::MyUpLoadMission()
{
}


MyUpLoadMission::~MyUpLoadMission()
{
}

void MyUpLoadMission::Execute()
{
	MyMission::Execute();

	if (false == SendCommand()) {
		std::cout << "传输链接断开\n";
		return;
	}
	ResponceType r = RecvResponse();
	if (r == ResponceType::EXIST) {
		std::cout << "文件已存在,上传成功\n";
		return;
	}
	if (r == ResponceType::FAILED) {
		std::cout << "上传初始化失败";
		return;
	}

	if (false == InitFile((fileName + localPath).c_str(), 0)) {
		std::cout << "文件流初始化失败\n";
	}
	if(false == GetFile()->OpenRead()) {
		std::cout << "文件打开失败\n";
	}
	if (false == SendFromReader()) {
		std::cout << "上传失败\n";
	}
	else {
		std::cout << "上传成功\n";
	}
	GetSocket()->disconnect();
	GetFile()->Close();
}