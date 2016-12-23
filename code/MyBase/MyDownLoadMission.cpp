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
	//	���ø���ĺ�������������Ӽ���֤
	MyMission::Execute();
	
	//�Զ���socket��Ĭ�Ϸ��ͻ�������СΪ4K�ֽڣ�Ϊ���ٶ���չ��4M
	GetSocket()->SetExtraBuf(new char[4096 * 1024 + 1], 4096 * 1024);

	//��������������������ȡ�ļ�
	if (false == SendCommand()) {
		GetSocket()->disconnect();
		return;
	}
	int num = 0;
	//��ȡҪ���ص��ļ�����
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
		//����ļ���
		if (false == GetSocket()->RecvBytes(name, GetToken().c_str())) {
			GetSocket()->disconnect();
			return;
		}
		//�������
		if (false == GetSocket()->RecvBytes(t_type, GetToken().c_str())) {	//8���ֽ�
			GetSocket()->disconnect();
			return;
		}
		type = MyEnCoder::BytesToUll(t_type);
		//������ļ��У��򴴽��ļ��в�������һ��ѭ��
		if (type == 1) {				
			MyFile::MakeDir((downloadPath + name).c_str());
			continue;
		}
		//������ļ��������
		else if(type == 0){			
			GetFile()->SetName((downloadPath + name).c_str());
			//����һ�����ļ�
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
