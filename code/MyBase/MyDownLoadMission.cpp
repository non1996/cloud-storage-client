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
	//	调用父类的函数与服务器连接及验证
	MyMission::Execute();
	
	//自定义socket类默认发送缓冲区大小为4K字节，为提速而扩展到4M
	GetSocket()->SetExtraBuf(new char[4096 * 1024 + 1], 4096 * 1024);

	//向服务器发送命令申请获取文件
	if (false == SendCommand()) {
		GetSocket()->disconnect();
		return;
	}
	int num = 0;
	//获取要下载的文件数量
	if (false == RecvFileNum(num)) {		
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
			return;
		}
		type = MyEnCoder::BytesToUll(t_type);
		//如果是文件夹，则创建文件夹并进入下一个循环
		if (type == 1) {				
			MyFile::MakeDir((downloadPath + name).c_str());
			continue;
		}
		//如果是文件，则接收
		else if(type == 0){			
			GetFile()->SetName((downloadPath + name).c_str());
			//创建一个空文件
			MyFile::CreateEmptyFile((downloadPath + name).c_str(), 0);	
			GetFile()->OpenWrite();
			if (false == _RecvToWriter()) {
				return;
			}
			GetFile()->Close();
		}
		else {
			return;
		}
	}
	CompleteChange();
	Sleep(1000);
	SetComplete(true);
	SetFinish();
	GetSocket()->disconnect();
	GetFile()->Close();
}
