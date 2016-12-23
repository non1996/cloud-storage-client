#pragma once

#ifndef MYCONTROLLER_H_
#define MYCONTROLLER_H_

#include "Cleaner.h"
#include "MyControl.h"
#include "MyCommandBuffer.h"

//-------------------------------------------
//	控制类，将用户的鼠标操作翻译成命令，并将命令放入发送队列
//	(实在不知道起什么名)
//	管理别的模块的初始化和资源释放
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

	//保存一个主窗口的引用
	void SetMainWindowReferance(MyMainWindow*);

	MyControl* GetControl() {
		return control;
	}

	MyCommandBuffer* GetBuffer() {
		return buffer;
	}

	//生成命令，并放入发送队列
	#pragma region put command to send queue
	//发送下载文件请求
	void PushGetCommand(std::string &uId) {
		buffer->PutRecvCommand(MyCommandBuilder::MakeGetCommand(uId));
	}

	//发送上传文件请求
	void PushPutCommand(std::string name, std::string localPath, std::string netPath) {
		buffer->PutSendCommand(MyCommandBuilder::MakePutCommand(name, localPath, netPath));
	}

	//发送删除文件请求
	void PushDeleteCommand(std::string &uId, std::string &name) {
		buffer->PutSendCommand(MyCommandBuilder::MakeDeleteCommand(uId, name));
	}

	//发送获取别人文件请求
	void PushForkCommand(std::string &uId, std::string &password, std::string &newPath) {
		buffer->PutSendCommand(MyCommandBuilder::MakeForkCommand(uId, password, newPath));
	}

	//发送查看目录下文件请求
	void PushLsCommand(std::string &showAll, std::string &name, std::string &pathName, std::vector<std::string> &args) {
		buffer->PutSendCommand(MyCommandBuilder::MakeLsCommand(showAll, name, pathName, args));
	}

	//创建文件夹请求
	void PushMkdirCommand(std::string &name, std::string &pathName) {
		buffer->PutSendCommand(MyCommandBuilder::MakeMkDirCommand(name, pathName));
	}

	//复制文件请求
	void PushCopyCommand(std::string &uId, std::string &newPath) {
		buffer->PutSendCommand(MyCommandBuilder::MakeCopyCommand(uId, newPath));
	}

	//移动文件请求
	void PushMoveCommand(std::string &uId, std::string &newName, std::string &newPath) {
		buffer->PutSendCommand(MyCommandBuilder::MakeMoveCommand(uId, newName, newPath));
	}

	//创建文件请求
	void PushTouchCommand(std::string &name, std::string &pathName) {
		buffer->PutSendCommand(MyCommandBuilder::MakeTouchCommand(name, pathName));
	}

	//重命名文件
	void PushRenameCommand(std::string &uId, std::string &oldName, std::string &newName, std::string &path) {
		buffer->PutSendCommand(MyCommandBuilder::MakeRenameCommand(uId, oldName, newName, path));
	}

	//共享文件
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
