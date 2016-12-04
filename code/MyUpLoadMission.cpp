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
		std::cout << "�������ӶϿ�\n";
		return;
	}
	ResponceType r = RecvResponse();
	if (r == ResponceType::EXIST) {
		std::cout << "�ļ��Ѵ���,�ϴ��ɹ�\n";
		return;
	}
	if (r == ResponceType::FAILED) {
		std::cout << "�ϴ���ʼ��ʧ��";
		return;
	}

	if (false == InitFile((fileName + localPath).c_str(), 0)) {
		std::cout << "�ļ�����ʼ��ʧ��\n";
	}
	if(false == GetFile()->OpenRead()) {
		std::cout << "�ļ���ʧ��\n";
	}
	if (false == SendFromReader()) {
		std::cout << "�ϴ�ʧ��\n";
	}
	else {
		std::cout << "�ϴ��ɹ�\n";
	}
	GetSocket()->disconnect();
	GetFile()->Close();
}