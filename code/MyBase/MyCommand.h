#pragma once

#ifndef MYCOMMAND_H_ 
#define MYCOMMAND_H_

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

//----------------------------------------
//	������࣬ʹ��commandģʽ
//	����ͨ����дexecute������ʵ�ֲ�������������µ�����
//----------------------------------------
class MyControl;
class MyCommand {
protected:
	bool isOk;
public:
	MyCommand();
	virtual ~MyCommand();

	virtual void Execute(MyControl*) = 0;

	//�ȴ����Է������ķ���
	virtual bool GetServerResponse(const char* info, int len) = 0;

	//������ת��Ϊ���͵ĸ�ʽ
	virtual std::string ToString() = 0;
};

//------------------------------------------------
//	��������
//	�������ļ�id
//	����͸�ʽ�� [get][+][uId]
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
//	�������ļ����У�����
//	�������ļ����ƣ��ļ�·�����ļ����ͣ��ļ����ļ��У�
//	����͸�ʽ�� [touch][+][fileName][+][pathName][+][type]
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
		SetType("0");	//0��ʾ�ļ���1��ʾ�ļ���
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
//	�ϴ�����
//	����touchָ���ȴ���һ�����ļ�
//	�����Ӵ����߳��ڷ����ϴ�ָ��
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
//	ɾ������
//	�������ļ�id
//	���͸�ʽ��[rm][+][uid]
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
//	��ȡ�����ļ�����
//	�������ļ�id����ȡ�룬���·��
//	���͸�ʽ��[fork][+][id][+][password][+][new path]
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
//	�鿴Ŀ¼���ļ�����
//	�������Ƿ�ݹ���ʾ�ļ���·������������Ϣ����ѯʱʹ�ã�
//	���͸�ʽ��[ls][+][showAll][+][path][+][arguments]
//------------------------------------------------	
class MyLsCommand : public MyCommand {
private:
	std::string showAll;				//Ϊ��1������Ŀ¼�������ļ����У�
										//Ϊ��0�����ظ�Ŀ¼���ļ����У�
	std::string dirName;
	std::string pathName;
	std::vector<std::string> args;		//�������������ļ�ʱ��
	std::string resOrContent;			//���������ص���Ϣ
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
//	�����ļ�������, ��ʽ��touch��ͬ��Ϊ����ʹ��
//------------------------------------------------	
class MyMkDirCommand : public MyCommand {
private:
	MyTouchCommand t;
public:
	MyMkDirCommand(std::string name, std::string pathName) {
		t = MyTouchCommand(name, pathName);
		t.SetType("1");										//1��ʾ�ļ���
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
//	��������
//	�������ļ�id�����·��
//	���͸�ʽ��[cp][+][id][+][new path]
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
//	�ƶ��ļ����У�����
//	�������ļ�id�������֣���·��
//	���͸�ʽ��[mv][+][id][+][new name][+][new path]
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
//	����������
//	��ʽ��move������ͬ
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
//	�ı��ļ�˽��������
//	�������ļ�id��˽�л���
//	���͸�ʽ��[mv][+][id][+][is private]
//------------------------------------------------	
class MyShareCommand :public MyCommand {
private:
	std::string uId;
	std::string priv;		//Ϊ1��ʾ˽�У� Ϊ0��ʾ����
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
//	��½����
//  ��ʽΪ�û��� �� �����MD5ֵ
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
//	���������߳̽��յ�����������Ϣ�����������������ʾ��Ϣ
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
//	��������
//	��ʽΪ [send][+][�Է����ǳ�][+][��������]
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
//	��¼����շ������������û���Ϣ�����ڽ�����ʾ
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
//	�����������ʵ�е����
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
