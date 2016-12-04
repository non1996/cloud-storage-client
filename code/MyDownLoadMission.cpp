#include "MyDownLoadMission.h"



bool MyDownLoadMission::SendCommand()
{
	std::string cmd;
	cmd += "get+"
		+ GetFileID() + "+"
		+ GetPassword();
	if (false == GetSocket()->SendBytes(cmd, GetToken().c_str())) {
		return false;
	}
	return true;
}

bool MyDownLoadMission::RecvFileNum(int &num)
{
	std::string recv;
	if (false == GetSocket()->RecvBytes(recv, GetToken().c_str())) {
		num = -1;
		return false;
	}
	num = std::stoi(recv);
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
	MyMission::Execute();

	if (false == SendCommand()) {
		return;
	}
	int num = 0;
	if (false == RecvFileNum(num)) {
		return;
	}

	for (int i = 0; i < num; ++i) {
		std::string name;
		std::string type;
		GetSocket()->RecvBytes(name, GetToken().c_str());//获得文件名
		GetSocket()->RecvBytes(type, GetToken().c_str());//获得类型
		if (type == "0") {				//如果是文件夹，则创建并进入下一个循环
			MyFile::MakeDir(name.c_str());
			continue;
		}
		else if(type == "1"){			//如果是文件，则接收
			GetFile()->SetName(name.c_str());
			MyFile::CreateEmptyFile(name.c_str(), 0);
			GetFile()->OpenWrite();
			if (false == RecvToWriter()) {
				std::cout << "文件 " << GetFile()->GetName() << "传输失败\n";
			}
			else {
				std::cout << "文件 " << GetFile()->GetName() << "传输成功\n";
			}
			GetFile()->Close();
		}
	}
	std::cout << "trans finish\n";
	GetSocket()->disconnect();
	GetFile()->Close();
}
