#include "MyDownLoadMission.h"
#include "MyEnCoder.h"

std::string MyDownLoadMission::downloadPath = "myDownload\\";

bool MyDownLoadMission::SendCommand()
{
	std::string cmd;
	cmd += "get+"
		+ GetFileID() + "+"
		+ std::string("6696");
	if (false == GetSocket()->SendBytes(cmd, GetToken().c_str())) {
		return false;
	}
	std::string resp;
	if (false == GetSocket()->RecvBytes(resp, GetToken().c_str())) {
		return false;
	}
	if (resp == "VALID") {
		return true;
	}
	return false;
}

bool MyDownLoadMission::RecvFileNum(int &num)
{
	std::string recv;
	if (false == GetSocket()->RecvBytes(recv, GetToken().c_str())) {
		num = -1;
		return false;
	}
	if (recv.size() != 8) {
		return false;
	}
	num = MyEnCoder::BytesToUll(recv);
	return true;
}

bool MyDownLoadMission::RecvFileName(std::string &name)
{
	if (false == GetSocket()->RecvBytes(name, GetToken().c_str())) {
		return false;
	}
	return true;
}

bool MyDownLoadMission::RecvFileType(std::string & type)
{
	if (false == GetSocket()->RecvBytes(type, GetToken().c_str())) {
		return false;
	}
	return true;
}

MyDownLoadMission::MyDownLoadMission()
{
}

MyDownLoadMission::~MyDownLoadMission()
{
}

void MyDownLoadMission::Execute()
{
	//	connect to server
	GetSocket()->SetExtraBuf(new char[4096 * 1024 + 1], 4096 * 1024);

	MyMission::Execute();

	if (false == SendCommand()) {
		GetSocket()->disconnect();
		return;
	}
	int num = 0;
	if (false == RecvFileNum(num)) {		//获取文件数量(包括文件夹数目)
		GetSocket()->disconnect();
		return;
	}

	if (!InitFile()) {
		return;
	}
	for (int i = 0; i < num; ++i) {
		std::string name;
		std::string t_type;
		unsigned long long type;
		//获得文件名
		if (false == GetSocket()->RecvBytes(name, GetToken().c_str())) {
			GetSocket()->disconnect();
			return;
		}
		//获得类型
		if (false == GetSocket()->RecvBytes(t_type, GetToken().c_str())) {	//8个字节
			GetSocket()->disconnect();
			break;
		}
		type = MyEnCoder::BytesToUll(t_type);
		if (type == 1) {				//如果是文件夹，则创建并进入下一个循环
			if (true == MyFile::MakeDir((downloadPath + name).c_str())) {
			}
			continue;
		}
		else if(type == 0){			//, 如果是文件，则接收
			GetFile()->SetName((downloadPath + name).c_str());
			MyFile::CreateEmptyFile((downloadPath + name).c_str(), 0);	//创建一个空文件
			GetFile()->OpenWrite();
			if (false == _RecvToWriter()) {
			}
			else {
			}
			GetFile()->Close();
		}
		else {
		}
	}
	CompleteChange();
	Sleep(1000);
	SetFinish();
	GetSocket()->disconnect();
	GetFile()->Close();
}
