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
		GetSocket()->RecvBytes(name, GetToken().c_str());//����ļ���
		GetSocket()->RecvBytes(type, GetToken().c_str());//�������
		if (type == "0") {				//������ļ��У��򴴽���������һ��ѭ��
			MyFile::MakeDir(name.c_str());
			continue;
		}
		else if(type == "1"){			//������ļ��������
			GetFile()->SetName(name.c_str());
			MyFile::CreateEmptyFile(name.c_str(), 0);
			GetFile()->OpenWrite();
			if (false == RecvToWriter()) {
				std::cout << "�ļ� " << GetFile()->GetName() << "����ʧ��\n";
			}
			else {
				std::cout << "�ļ� " << GetFile()->GetName() << "����ɹ�\n";
			}
			GetFile()->Close();
		}
	}
	std::cout << "trans finish\n";
	GetSocket()->disconnect();
	GetFile()->Close();
}
