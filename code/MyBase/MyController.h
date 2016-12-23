#pragma once

#ifndef MYCONTROLLER_H_
#define MYCONTROLLER_H_

#include "Cleaner.h"
#include "MyControl.h"
#include "MyCommandBuffer.h"

//-------------------------------------------
//	�����࣬���û���������������������������뷢�Ͷ���
//	(ʵ�ڲ�֪����ʲô��)
//	������ģ��ĳ�ʼ������Դ�ͷ�
//-------------------------------------------
class MyController
{
private:
	static MyController* instance;

	MyControl* control;
	MyCommandBuffer* buffer;

	MyMainWindow* mainWindow;
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

	//����һ�������ڵ�����
	void SetMainWindowReferance(MyMainWindow*);

	MyControl* GetControl() {
		return control;
	}

	MyCommandBuffer* GetBuffer() {
		return buffer;
	}

	//������������뷢�Ͷ���
	#pragma region put command to send queue
	//���������ļ�����
	void PushGetCommand(std::string &uId) {
		buffer->PutRecvCommand(MyCommandBuilder::MakeGetCommand(uId));
	}

	//�����ϴ��ļ�����
	void PushPutCommand(std::string name, std::string localPath, std::string netPath) {
		buffer->PutSendCommand(MyCommandBuilder::MakePutCommand(name, localPath, netPath));
	}

	//����ɾ���ļ�����
	void PushDeleteCommand(std::string &uId, std::string &name) {
		buffer->PutSendCommand(MyCommandBuilder::MakeDeleteCommand(uId, name));
	}

	//���ͻ�ȡ�����ļ�����
	void PushForkCommand(std::string &uId, std::string &password, std::string &newPath) {
		buffer->PutSendCommand(MyCommandBuilder::MakeForkCommand(uId, password, newPath));
	}

	//���Ͳ鿴Ŀ¼���ļ�����
	void PushLsCommand(std::string &showAll, std::string &name, std::string &pathName, std::vector<std::string> &args) {
		buffer->PutSendCommand(MyCommandBuilder::MakeLsCommand(showAll, name, pathName, args));
	}

	//�����ļ�������
	void PushMkdirCommand(std::string &name, std::string &pathName) {
		buffer->PutSendCommand(MyCommandBuilder::MakeMkDirCommand(name, pathName));
	}

	//�����ļ�����
	void PushCopyCommand(std::string &uId, std::string &newPath) {
		buffer->PutSendCommand(MyCommandBuilder::MakeCopyCommand(uId, newPath));
	}

	//�ƶ��ļ�����
	void PushMoveCommand(std::string &uId, std::string &newName, std::string &newPath) {
		buffer->PutSendCommand(MyCommandBuilder::MakeMoveCommand(uId, newName, newPath));
	}

	//�����ļ�����
	void PushTouchCommand(std::string &name, std::string &pathName) {
		buffer->PutSendCommand(MyCommandBuilder::MakeTouchCommand(name, pathName));
	}

	//�������ļ�
	void PushRenameCommand(std::string &uId, std::string &oldName, std::string &newName, std::string &path) {
		buffer->PutSendCommand(MyCommandBuilder::MakeRenameCommand(uId, oldName, newName, path));
	}

	//�����ļ�
	void PushShareCommand(std::string &uId, std::string &priv, std::string &pass) {
		buffer->PutSendCommand(MyCommandBuilder::MakeShareCommand(uId, priv, pass));
	}

	void PushLogInCommand(std::string &un, std::string &pw) {
		buffer->PutSendCommand(MyCommandBuilder::MakeLogInCommand(un, pw));
	}

	void PushSendCommand(std::string &cID, std::string &content) {
		buffer->PutSendCommand(MyCommandBuilder::MakeSendCommand(cID, content));
	}
	#pragma endregion
	
	void Close();

	static MyController* Instance();
	static void Release();
};



#endif // !MYCONTROLLER_H_
