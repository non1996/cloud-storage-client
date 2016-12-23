#pragma once

#ifndef MYCOMMAND_H_ 
#define MYCOMMAND_H_

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

//----------------------------------------
//	命令基类，使用command模式
//	子类通过重写execute函数来实现操作，便于添加新的命令
//----------------------------------------
class MyControl;
class MyCommand {
protected:
	bool isOk;
public:
	MyCommand();
	virtual ~MyCommand();

	virtual void Execute(MyControl*) = 0;

	//等待来自服务器的反馈
	virtual bool GetServerResponse(const char* info, int len) = 0;

	//将命令转换为发送的格式
	virtual std::string ToString() = 0;
};

//------------------------------------------------
//	下载命令
//	参数：文件id
//	命令发送格式： [get][+][uId]
//------------------------------------------------
class MyGetCommand : public MyCommand {
private:
	std::string uId;

public:
	MyGetCommand(std::string uID) {
		uId = uID;
	}
	virtual void Execute(MyControl*);

	virtual bool GetServerResponse(const char* info, int len) { return true; }

	virtual std::string ToString() {
		return "";
	}
};

//------------------------------------------------
//	创建新文件（夹）命令
//	参数：文件名称，文件路径，文件类型（文件或文件夹）
//	命令发送格式： [touch][+][fileName][+][pathName][+][type]
//------------------------------------------------
class MyTouchCommand : public MyCommand {
private:
	unsigned long long uId;
	std::string fileName;
	std::string pathName;
	std::string type;
	std::string date;

public:
	MyTouchCommand() {}
	MyTouchCommand(std::string name, std::string pathName) {
		fileName = name;
		this->pathName = pathName;
		SetType("0");	//0表示文件，1表示文件夹
	}

	void SetType(std::string type) {
		this->type = type;
	}

	unsigned long long GetUID() {
		return uId;
	}

	std::string &GetName() {
		return fileName;
	}

	std::string &GetPath() {
		return pathName;
	}

	std::string &GetDate() {
		return date;
	}

	virtual bool GetServerResponse(const char* info, int len);

	virtual void Execute(MyControl*);

	virtual std::string ToString() {
		std::stringstream ss;
		ss << "touch" << "+"
			<< fileName << "+"
			<< pathName << "+"
			<< type;
		return ss.str();
	}
};

//------------------------------------------------
//	上传命令
//	调用touch指令先创建一个空文件
//	再在子传输线程内发送上传指令
//------------------------------------------------
class MyPutCommand : public MyCommand {
private:
	std::string fileName;
	std::string localPath;
	std::string netPath;
	MyTouchCommand touch;

public:
	MyPutCommand(std::string name, std::string localPath, std::string netPath) {
		fileName = name;
		this->localPath = localPath;
		this->netPath = netPath;
		touch = MyTouchCommand(fileName, netPath);
		isOk = false;
	}

	unsigned long long GetUID() {
		return touch.GetUID();
	}

	virtual void Execute(MyControl*);

	virtual bool GetServerResponse(const char* info, int len) { 
		if (touch.GetServerResponse(info, len) == true) {
			isOk = true;
			return true;
		}
		return false;
	}

	virtual std::string ToString() {				
		//before put i should touch first
		return touch.ToString();
	}
};

//------------------------------------------------
//	删除命令
//	参数：文件id
//	发送格式：[rm][+][uid]
//------------------------------------------------
class MyDeleteCommand : public MyCommand {
private:
	std::string uId;
	std::string name;
public:
	MyDeleteCommand(std::string uId, std::string name) {
		this->name = name;
		this->uId = uId;
	}

	virtual void Execute(MyControl*);

	virtual bool GetServerResponse(const char* info, int len);

	virtual std::string ToString() {
		std::stringstream ss;
		ss << "rm" << "+"
			<< uId;
		return ss.str();
	}
};

//------------------------------------------------
//	获取共享文件命令
//	参数：文件id，提取码，存放路径
//	发送格式：[fork][+][id][+][password][+][new path]
//------------------------------------------------			
class MyForkCommand : public MyCommand {
private:
	std::string uId;
	std::string password;
	std::string newPath;
public:
	MyForkCommand(std::string uId, std::string password, std::string newPath) {
		this->uId = uId;
		this->password = password;
		this->newPath = newPath;
	}

	virtual void Execute(MyControl*);

	virtual bool GetServerResponse(const char* info, int len);

	virtual std::string ToString() {
		std::stringstream ss;
		ss << "fork" << "+"
			<< uId << "+"
			<< password << "+"
			<< newPath;
		return ss.str();
	}
};

//------------------------------------------------
//	查看目录下文件命令
//	参数：是否递归显示文件，路径名，额外信息（查询时使用）
//	发送格式：[ls][+][showAll][+][path][+][arguments]
//------------------------------------------------	
class MyLsCommand : public MyCommand {
private:
	std::string showAll;				//为“1”返回目录下所有文件（夹）
										//为“0”返回该目录下文件（夹）
	std::string dirName;
	std::string pathName;
	std::vector<std::string> args;		//参数，供查找文件时用
	std::string resOrContent;			//服务器返回的信息
public:
	MyLsCommand(std::string showAll, std::string dir, std::string path, std::vector<std::string> &args) {
		this->showAll = showAll;
		this->dirName = dir;
		this->pathName = path;
		this->args = args;
	}

	//splid a string with the paramar character 
	void split(std::string &str, char flag, std::vector<std::string> &v) {
		unsigned int begin = 0;
		for (unsigned int i = 0; i < str.size(); ++i) {
			if (str.at(i) == flag && i != begin) {
				v.push_back(str.substr(begin, i - begin));
				begin = i + 1;
			}
			else if (str.at(i) == flag && i == begin) {
				begin = i + 1;
			}
		}
		if (begin < str.size()) {
			v.push_back(str.substr(begin, str.size() - begin));
		}
	}

	virtual bool GetServerResponse(const char* info, int len);

	virtual void Execute(MyControl*);

	virtual std::string ToString() {
		std::stringstream ss;
		ss  << "ls" << "+"
			<< showAll << "+"
			<< pathName + dirName;
		for (unsigned int i = 0; i < args.size(); ++i) {
			ss << "+" << args[i];
		}
		return ss.str();
	}
};

//------------------------------------------------
//	创建文件夹命令, 格式和touch相同，为方便使用
//------------------------------------------------	
class MyMkDirCommand : public MyCommand {
private:
	MyTouchCommand t;
public:
	MyMkDirCommand(std::string name, std::string pathName) {
		t = MyTouchCommand(name, pathName);
		t.SetType("1");										//1表示文件夹
	}

	virtual bool GetServerResponse(const char* info, int len) {
		return t.GetServerResponse(info, len);
	}

	virtual void Execute(MyControl*);

	virtual std::string ToString() {
		return t.ToString();
	}
};

//------------------------------------------------
//	复制命令
//	参数：文件id，存放路径
//	发送格式：[cp][+][id][+][new path]
//------------------------------------------------	
class MyCopyCommand : public MyCommand {
private:
	std::string uId;
	std::string newPath;

public:
	MyCopyCommand(std::string &uId, std::string &newPath) {
		this->uId = uId;
		this->newPath = newPath;
	}

	virtual void Execute(MyControl*);

	virtual bool GetServerResponse(const char* info, int len);

	virtual std::string ToString() {
		std::stringstream ss;
		ss << "cp" << "+"
			<< uId << "+"
			<< newPath;
		return ss.str();
	}
};

//------------------------------------------------
//	移动文件（夹）命令
//	参数：文件id，新名字，新路径
//	发送格式：[mv][+][id][+][new name][+][new path]
//------------------------------------------------	
class MyMoveCommand :public MyCommand {
private:
	std::string uId;
	std::string newName;
	std::string newPath;


public:
	MyMoveCommand(std::string uId, std::string newName, std::string newPath) {
		this->uId = uId;
		this->newName = newName;
		this->newPath = newPath;
	}

	std::string &GetNewName() {
		return newName;
	}

	virtual void Execute(MyControl*);

	virtual bool GetServerResponse(const char* info, int len);

	virtual std::string ToString() {
		std::stringstream ss;
		ss << "mv" << "+"
			<< uId << "+"
			<< newName << "+"
			<< newPath;
		return ss.str();
	}
};

//------------------------------------------------
//	重命名命令
//	格式与move命令相同
//------------------------------------------------	
class MyRenameCommand : public MyMoveCommand {
private:
	std::string oldName;
public:
	MyRenameCommand(std::string uId, std::string oldName, std::string newName, std::string path)
		:MyMoveCommand(uId, newName, path){
		this->oldName = oldName;
	}

	virtual void Execute(MyControl*);
};

//------------------------------------------------
//	改变文件私有性命令
//	参数：文件id，私有或共有
//	发送格式：[mv][+][id][+][is private]
//------------------------------------------------	
class MyShareCommand :public MyCommand {
private:
	std::string uId;
	std::string priv;		//为1表示私有， 为0表示共享
	std::string pass;
public:
	MyShareCommand(std::string uId, std::string priv, std::string pass) {
		this->uId = uId;
		this->priv = priv;
		this->pass = pass;
	}

	virtual void Execute(MyControl*);

	virtual bool GetServerResponse(const char* info, int len);

	virtual std::string ToString() {
		std::stringstream ss;
		ss	<< "chmod" << "+"
			<< uId << "+"
			<< priv << "+"
			<< pass;
		return ss.str();
	}
};

//------------------------------------------------
//	登陆命令
//  格式为用户名 加 密码的MD5值
//------------------------------------------------	
class MyLogInCommand :public MyCommand {
private:
	std::string username;
	std::string password;
	std::string rootName;
	std::string token;
	std::vector<std::string> content;
public:
	MyLogInCommand(std::string un, std::string pw) {
		username = un;
		password = pw;
	}

	virtual bool GetServerResponse(const char* info, int len) {
		token = std::string(info, len);
		return true;
	}
		

	virtual void Execute(MyControl*);

	virtual std::string ToString();
};

//------------------------------------------------
//	被动监听线程接收到服务器的信息后生成这个命令来显示消息
//------------------------------------------------	
class MyMessageCommand : public MyCommand {
private:
	std::string cID;
	std::string content;

public:
	MyMessageCommand(){}
	
	virtual bool GetServerResponse(const char* info, int len) {
		content = std::string(info, len);
		return true;
	}

	virtual void Execute(MyControl*);

	virtual std::string ToString() { return ""; }
};

//------------------------------------------------
//	发送命令
//	格式为 [send][+][对方的昵称][+][发送内容]
//------------------------------------------------	
class MySendCommand : public MyCommand {
private:
	std::string cID;
	std::string content;

public:
	MySendCommand(std::string &cID, std::string &message);

	virtual bool GetServerResponse(const char* info, int len);

	virtual void Execute(MyControl*);

	virtual std::string ToString();
};

//----------------------------------------------------
//	登录后接收服务器发来的用户信息，并在界面显示
//----------------------------------------------------
class MySetInfoCommand : public MyCommand {
private:
	std::string name;
	std::string url;
	unsigned long long currentVolume, totalVolume;
public:
	MySetInfoCommand(std::string &name, std::string &url, unsigned long long currentV, unsigned long long totalV) {
		this->name = name;
		this->url = url;
		currentVolume = currentV;
		totalVolume = totalV;
	}

	virtual bool GetServerResponse(const char* info, int len);

	virtual void Execute(MyControl*);

	virtual std::string ToString();
};

//-----------------------------------------------------------------
//	命令构造器，其实有点多余
//-----------------------------------------------------------------
class MyCommandBuilder {
public:
	static MyGetCommand* MakeGetCommand(std::string &uId) {
		return new MyGetCommand(uId);
	}

	static MyPutCommand* MakePutCommand(std::string name, std::string localPath, std::string netPath) {
		return new MyPutCommand(name, localPath, netPath);
	}

	static MyDeleteCommand* MakeDeleteCommand(std::string &uId, std::string &name) {
		return new MyDeleteCommand(uId, name);
	}

	static MyForkCommand* MakeForkCommand(std::string &uId, std::string &password, std::string &newPath) {
		return new MyForkCommand(uId, password, newPath);
	}

	static MyLsCommand* MakeLsCommand(std::string &showAll, std::string &name, std::string &pathName, std::vector<std::string> &args) {
		return new MyLsCommand(showAll, name, pathName, args);
	}

	static MyMkDirCommand* MakeMkDirCommand(std::string &name, std::string &pathName) {
		return new MyMkDirCommand(name, pathName);
	}

	static MyCopyCommand* MakeCopyCommand(std::string &uId, std::string &newPath) {
		return new MyCopyCommand(uId, newPath);
	}

	static MyMoveCommand* MakeMoveCommand(std::string &uId, std::string &newName, std::string &newPath) {
		return new MyMoveCommand(uId, newName, newPath);
	}

	static MyTouchCommand* MakeTouchCommand(std::string &name, std::string &pathName) {
		return new MyTouchCommand(name, pathName);
	}

	static MyRenameCommand* MakeRenameCommand(std::string &uId, std::string &oldName, std::string &newName, std::string &path) {
		return new MyRenameCommand(uId, oldName, newName, path);
	}

	static MyShareCommand* MakeShareCommand(std::string &uId, std::string &priv, std::string &pass) {
		return new MyShareCommand(uId, priv, pass);
	}

	static MyLogInCommand* MakeLogInCommand(std::string &un, std::string &pw) {
		return new MyLogInCommand(un, pw);
	}

	static MyMessageCommand* MakeMessageCommand() {
		return new MyMessageCommand();
	}

	static MySendCommand* MakeSendCommand(std::string &cID, std::string &content) {
		return new MySendCommand(cID, content);
	}

	static MySetInfoCommand* MakeSetInfoCommand(std::string &name, std::string &url, unsigned long long currentV, unsigned long long totalV) {
		return new MySetInfoCommand(name, url, currentV, totalV);
	}
};
#endif // !MYCOMMAND_H_ 
