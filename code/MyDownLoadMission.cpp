#include "MyDownLoadMission.h"
#include "MyEnCoder.h"

std::string MyDownLoadMission::downloadPath = "myDownload\\";

bool MyDownLoadMission::SendCommand()
{
	std::string cmd;
	cmd += "get+"
		+ GetFileID() + "+"
		+ std::string("6696");
	std::cout << "发送下载命令:" << cmd << std::endl;
	if (false == GetSocket()->SendBytes(cmd, GetToken().c_str())) {
		std::cout << "网络断开\n";
		return false;
	}
	std::string resp;
	if (false == GetSocket()->_RecvBytes(resp, GetToken().c_str())) {
		return false;
	}
	if (resp == "VALID") {
		std::cout << resp << std::endl;
		return true;
	}
	std::cout << resp << std::endl;
	return false;
}

bool MyDownLoadMission::RecvFileNum(int &num)
{
	std::cout << "获取文件数量\n";
	std::string recv;
	if (false == GetSocket()->_RecvBytes(recv, GetToken().c_str())) {
		num = -1;
		return false;
	}
	if (recv.size() != 8) {
		std::cout << "文件数不够8个字节:" << recv.size() << "收到的为:" << recv << std::endl;
		return false;
	}
	num = MyEnCoder::BytesToUll(recv);
	return true;
}

bool MyDownLoadMission::RecvFileName(std::string &name)
{
	if (false == GetSocket()->_RecvBytes(name, GetToken().c_str())) {
		return false;
	}
	return true;
}

bool MyDownLoadMission::RecvFileType(std::string & type)
{
	if (false == GetSocket()->_RecvBytes(type, GetToken().c_str())) {
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
	GetSocket()->SetExtraBuf(new char[4096 * 1024], 4096 * 1024);

	MyMission::Execute();

	if (false == SendCommand()) {
		std::cout << "发送get指令失败" << std::endl;
		GetSocket()->disconnect();
		return;
	}
	int num = 0;
	if (false == RecvFileNum(num)) {		//获取文件数量(包括文件夹数目)
		std::cout << "获取文件数目失败\n";	//转8个字节
		GetSocket()->disconnect();
		return;
	}
	std::cout << "文件数目为:" << num << std::endl;

	if (!InitFile()) {
		std::cout << "文件初始化失败" << std::endl;
	}

	for (int i = 0; i < num; ++i) {
		std::string name;
		std::string t_type;
		unsigned long long type;
		//获得文件名
		if (false == GetSocket()->_RecvBytes(name, GetToken().c_str())) {
			std::cout << "获取文件名失败" << std::endl;
			GetSocket()->disconnect();
			return;
		}
		std::cout << "文件名：" << name << std::endl;
		//获得类型
		if (false == GetSocket()->_RecvBytes(t_type, GetToken().c_str())) {	//8个字节
			std::cout << "获取文件类型失败" << std::endl;
			GetSocket()->disconnect();
			return;
		}
		type = MyEnCoder::BytesToUll(t_type);
		if (type == 1) {				//如果是文件夹，则创建并进入下一个循环
			std::cout << "创建文件夹\n";
			if (true == MyFile::MakeDir((downloadPath + name).c_str())) {
				std::cout << "文件夹创建成功\n";
			}
			continue;
		}
		else if(type == 0){			//, 如果是文件，则接收
			GetFile()->SetName((downloadPath + name).c_str());
			MyFile::CreateEmptyFile((downloadPath + name).c_str(), 0);	//创建一个空文件
			GetFile()->OpenWrite();
			if (false == _RecvToWriter()) {
				std::cout << "文件 " << GetFile()->GetName() << "传输失败\n";
			}
			else {
				std::cout << "文件 " << GetFile()->GetName() << "传输成功\n";
			}
			GetFile()->Close();
		}
		else {
			std::cout << "无效的文件类型:" << type << std::endl;
		}
	}
	std::cout << "trans finish\n";
	GetSocket()->disconnect();
	GetFile()->Close();
}
