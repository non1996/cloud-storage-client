#pragma once

#ifndef MYCONTROLLER_H_
#define MYCONTROLLER_H_

#include "Cleaner.h"
#include "MyControl.h"
#include "MyCommandBuffer.h"

//-------------------------------------------
//	�����࣬�����û������������ŵ����ͻ���
//	(ʵ�ڲ�֪����ʲô��)
//	������ģ��ĳ�ʼ������Դ�ͷ�
//-------------------------------------------
class MyController
{
private:
	static MyController* instance;

	MyControl* control;
	MyCommandBuffer* buffer;
protected:
	MyController();

public:
	
	~MyController();

	bool Init() {
		buffer = MyCommandBuffer::Instance();
		control = MyControl::Instance();					
		if (!control->Init()) {
			Cleaner::Release<MyControl*>(&control);
			Cleaner::Release<MyCommandBuffer*>(&buffer);
			return false;
		}
		return true;
	}

	MyControl* GetControl() {
		return control;
	}

	MyCommandBuffer* GetBuffer() {
		return buffer;
	}

	//���������ļ�����
	void PushGetCommand(std::string &uId) {
//		buffer->PutRecv(MyCommandBuilder::MakeGetCommand(uId));
		buffer->PutRecvCommand(MyCommandBuilder::MakeGetCommand(std::string("50")));
	}

	//�����ϴ��ļ�����
	void PushPutCommand(std::string name, std::string localPath, std::string netPath) {
		buffer->PutSend(MyCommandBuilder::MakePutCommand(name, localPath, netPath));
	}
	
	//����ɾ���ļ�����
	void PushDeleteCommand(std::string &uId, std::string &name) {
		buffer->PutSend(MyCommandBuilder::MakeDeleteCommand(uId, name));
	}

	//���ͻ�ȡ�����ļ�����
	void PushForkCommand(std::string &uId, std::string &password, std::string &newPath) {
		buffer->PutSend(MyCommandBuilder::MakeForkCommand(uId, password, newPath));
	}

	//���Ͳ鿴Ŀ¼���ļ�����
	void PushLsCommand(std::string &showAll, std::string &name, std::string &pathName, std::vector<std::string> &args) {
//		buffer->PutSend(MyCommandBuilder::MakeLsCommand(showAll, name, pathName, args));
		buffer->PutSend(MyCommandBuilder::MakeLsCommand(std::string("0"), std::string(""), std::string("/"), args));
	}

	//�����ļ�������
	void PushMkdirCommand(std::string &name, std::string &pathName) {
		buffer->PutSend(MyCommandBuilder::MakeMkDirCommand(name, pathName));
	}

	//�����ļ�����
	void PushCopyCommand(std::string &uId, std::string &newPath) {
		buffer->PutSend(MyCommandBuilder::MakeCopyCommand(uId, newPath));
	}

	//�ƶ��ļ�����
	void PushMoveCommand(std::string &uId, std::string &newName, std::string &newPath) {
		buffer->PutSend(MyCommandBuilder::MakeMoveCommand(uId, newName, newPath));
	}

	//�����ļ�����
	void PushTouchCommand(std::string &name, std::string &pathName) {
		buffer->PutSend(MyCommandBuilder::MakeTouchCommand(name, pathName));
	}

	//�������ļ�
	void PushRenameCommand(std::string &uId, std::string &oldName, std::string &newName, std::string &path) {
		buffer->PutSend(MyCommandBuilder::MakeRenameCommand(uId, oldName, newName, path));
	}

	//�����ļ�
	void PushShareCommand(std::string &uId, std::string &priv) {
		buffer->PutSend(MyCommandBuilder::MakeShareCommand(uId, priv));
	}

	void PushLogInCommand(std::string &un, std::string &pw) {
		std::cout << un << " " << pw << std::endl;
		buffer->PutSend(MyCommandBuilder::MakeLogInCommand(un, pw));
	}

	static MyController* Instance();
	static void Release();
};



#endif // !MYCONTROLLER_H_
