#include "MyDownLoadMission.h"
#include "MyEnCoder.h"

std::string MyDownLoadMission::downloadPath = "myDownload\\";

bool MyDownLoadMission::SendCommand()
{
	std::string cmd;
	cmd += "get+"
		+ GetFileID() + "+"
		+ std::string("6696");
	std::cout << "������������:" << cmd << std::endl;
	if (false == GetSocket()->SendBytes(cmd, GetToken().c_str())) {
		std::cout << "����Ͽ�\n";
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
	std::cout << "��ȡ�ļ�����\n";
	std::string recv;
	if (false == GetSocket()->_RecvBytes(recv, GetToken().c_str())) {
		num = -1;
		return false;
	}
	if (recv.size() != 8) {
		std::cout << "�ļ�������8���ֽ�:" << recv.size() << "�յ���Ϊ:" << recv << std::endl;
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
		std::cout << "����getָ��ʧ��" << std::endl;
		GetSocket()->disconnect();
		return;
	}
	int num = 0;
	if (false == RecvFileNum(num)) {		//��ȡ�ļ�����(�����ļ�����Ŀ)
		std::cout << "��ȡ�ļ���Ŀʧ��\n";	//ת8���ֽ�
		GetSocket()->disconnect();
		return;
	}
	std::cout << "�ļ���ĿΪ:" << num << std::endl;

	if (!InitFile()) {
		std::cout << "�ļ���ʼ��ʧ��" << std::endl;
	}

	for (int i = 0; i < num; ++i) {
		std::string name;
		std::string t_type;
		unsigned long long type;
		//����ļ���
		if (false == GetSocket()->_RecvBytes(name, GetToken().c_str())) {
			std::cout << "��ȡ�ļ���ʧ��" << std::endl;
			GetSocket()->disconnect();
			return;
		}
		std::cout << "�ļ�����" << name << std::endl;
		//�������
		if (false == GetSocket()->_RecvBytes(t_type, GetToken().c_str())) {	//8���ֽ�
			std::cout << "��ȡ�ļ�����ʧ��" << std::endl;
			GetSocket()->disconnect();
			return;
		}
		type = MyEnCoder::BytesToUll(t_type);
		if (type == 1) {				//������ļ��У��򴴽���������һ��ѭ��
			std::cout << "�����ļ���\n";
			if (true == MyFile::MakeDir((downloadPath + name).c_str())) {
				std::cout << "�ļ��д����ɹ�\n";
			}
			continue;
		}
		else if(type == 0){			//, ������ļ��������
			GetFile()->SetName((downloadPath + name).c_str());
			MyFile::CreateEmptyFile((downloadPath + name).c_str(), 0);	//����һ�����ļ�
			GetFile()->OpenWrite();
			if (false == _RecvToWriter()) {
				std::cout << "�ļ� " << GetFile()->GetName() << "����ʧ��\n";
			}
			else {
				std::cout << "�ļ� " << GetFile()->GetName() << "����ɹ�\n";
			}
			GetFile()->Close();
		}
		else {
			std::cout << "��Ч���ļ�����:" << type << std::endl;
		}
	}
	std::cout << "trans finish\n";
	GetSocket()->disconnect();
	GetFile()->Close();
}
