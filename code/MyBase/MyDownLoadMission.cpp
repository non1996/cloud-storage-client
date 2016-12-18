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
	if (false == RecvFileNum(num)) {		//��ȡ�ļ�����(�����ļ�����Ŀ)
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
		//����ļ���
		if (false == GetSocket()->RecvBytes(name, GetToken().c_str())) {
			GetSocket()->disconnect();
			return;
		}
		//�������
		if (false == GetSocket()->RecvBytes(t_type, GetToken().c_str())) {	//8���ֽ�
			GetSocket()->disconnect();
			break;
		}
		type = MyEnCoder::BytesToUll(t_type);
		if (type == 1) {				//������ļ��У��򴴽���������һ��ѭ��
			if (true == MyFile::MakeDir((downloadPath + name).c_str())) {
			}
			continue;
		}
		else if(type == 0){			//, ������ļ��������
			GetFile()->SetName((downloadPath + name).c_str());
			MyFile::CreateEmptyFile((downloadPath + name).c_str(), 0);	//����һ�����ļ�
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
