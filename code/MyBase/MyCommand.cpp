#include "MyEnCoder.h"
#include "MyCommand.h"
#include "MyControl.h"

#include <cstdio>
#include <exception>
#include <vector>

typedef unsigned long long ULL;


MyCommand::MyCommand()
{
	isOk = false;
}

MyCommand::~MyCommand()
{
}

void MyGetCommand::Execute(MyControl *c)
{
	c->NewMissionD(	uId,
					c->GetSendRecvToken(),
					c->GetUserNameM(),
					c->GetPassword());
}

void MyPutCommand::Execute(MyControl *c)
{
	if (isOk) {
		c->NewMissionU(	fileName,
						GetUID(), 
						localPath,
						netPath,
						c->GetSendRecvToken(),
						c->GetUserNameM());
	}
}

void MyDeleteCommand::Execute(MyControl *c)
{
	if (isOk) {
		std::cout << "200\n";
//		c->GetManager()->DeleteNetFile(name);
		c->DeleteNetFile(name);
	}
	else {
		std::cout << "错误\n";
	}
}

bool MyDeleteCommand::GetServerResponse(const char * info, int len)
{
	if (len != 8) {
		isOk = false;
		return false;
	}
	std::string res(info, len);
	unsigned long long resNum = MyEnCoder::BytesToUll(res);
	if (resNum == 200) {
		isOk = true;
	}
	return true;
}

void MyForkCommand::Execute(MyControl *c)
{
	std::cout << "haven't implement fork command" << std::endl;
	if (isOk) {
		std::cout << "200\n";
	}
}

bool MyForkCommand::GetServerResponse(const char * info, int len)
{
	if (len != 8) {
		isOk = false;
		return false;
	}
	std::string res(info, len);
	unsigned long long resNum = MyEnCoder::BytesToUll(res);
	if (resNum == 200) {
		isOk = true;
	}
	return true;
}

bool MyLsCommand::GetServerResponse(const char * info, int len)
{
	resOrContent.assign(info, len);
	if (resOrContent == "error happens when querying files") {
		isOk = false;
	}
	std::cout << "目录信息为:" << std::endl;
	std::cout << resOrContent << std::endl;
	isOk = true;
	return true;
}

void MyLsCommand::Execute(MyControl *c)
{
	if (isOk) {
		std::vector<std::string> fileInfos;
		split(resOrContent, '\n', fileInfos);
		c->SetPath((pathName + dirName));
		c->ReplaceContent(fileInfos);
	}
}

void MyMkDirCommand::Execute(MyControl *c)
{
	t.Execute(c);
}

void MyCopyCommand::Execute(MyControl *c)
{
	if (isOk) {
		c->Paste();
	}
	else {
		std::cout << "复制不成功\n";
	}
}

bool MyCopyCommand::GetServerResponse(const char * info, int len)
{
	if (len != 8) {
		isOk = false;
		return false;
	}
	std::string res(info, len);
	unsigned long long resNum = MyEnCoder::BytesToUll(res);
	if (resNum == 200) {
		isOk = true;
	}
	return true;
}

void MyMoveCommand::Execute(MyControl *c)
{
	if (isOk) {
		c->Paste();
	}
}

bool MyMoveCommand::GetServerResponse(const char * info, int len)
{
	if (len != 8) {
		isOk = false;
		return false;
	}
	std::string res(info, len);
	unsigned long long resNum = MyEnCoder::BytesToUll(res);
	if (resNum == 200) {
		isOk = true;
	}
	return true;
}

void MyShareCommand::Execute(MyControl *)
{
	if (isOk) {
		std::cout << "200\n";
		//	std::cout << "haven't implement share command" << std::endl;
	}
}

bool MyShareCommand::GetServerResponse(const char * info, int len)
{
	if (len != 8) {
		isOk = false;
		return false;
	}
	std::string res(info, len);
	unsigned long long resNum = MyEnCoder::BytesToUll(res);
	if (resNum == 200) {
		isOk = true;
	}
	return true;
}

bool MyTouchCommand::GetServerResponse(const char * info, int len)
{
	if (len != 16) {
		std::cout << "长度不对, len为:" << len << std::endl;
		return false;
	}
	std::string res(info, len);

	unsigned long long resNum = MyEnCoder::BytesToUll(res.substr(0, 8));
	if (resNum == 200) {
		uId = MyEnCoder::BytesToUll(res.substr(8, 8));
		std::cout << "创建文件成功，uid是:" << uId << std::endl;
		isOk = true;
		return true;
	}
	std::cout << "创建文件失败,状态码为:" << resNum << std::endl;
	return false;
}

void MyTouchCommand::Execute(MyControl *c)
{
	if (isOk) {
		std::stringstream ss;
		ss << uId;
		if (type == "0") {		
			c->AddNormFile(ss.str(), pathName, fileName, date, 0, 0);
		}
		else if (type == "1") {
			c->AddDirector(ss.str(), pathName, fileName, date, 0, 0);
		}
	}	
}

void MyRenameCommand::Execute(MyControl *c)
{
	if (isOk) {
		c->Rename(oldName, GetNewName());
	}
}

void MyLogInCommand::Execute(MyControl *c)
{
	if(token == c->GetSendRecvToken()){
		c->LogIn(username, password);
		c->InitMessageSocket(token, username);
		c->StartMessageSocket();
		std::cout << "connect to server-------------------log in command\n";
	}
}

std::string MyLogInCommand::ToString()
{
	std::string md5Password;
	std::string userAndPassMD5;
	std::string sizeUserAndPassMD5;			//用户名和密码MD5长度和
	std::string sizeUser;					//用户明文长度

	md5Password = MyEnCoder::MD5(password);
	userAndPassMD5 = username + md5Password;
	sizeUser = MyEnCoder::UllToBytes((ULL)username.size());
	sizeUserAndPassMD5 = MyEnCoder::UllToBytes((ULL)userAndPassMD5.size());
	return sizeUserAndPassMD5 + sizeUser + userAndPassMD5;
}

void MyMessageCommand::Execute(MyControl *c)
{
	std::cout << "not implement message yet\n";
	std::cout << content << std::endl;
}
