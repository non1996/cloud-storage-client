#include "MyEnCoder.h"
#include "MyCommand.h"
#include "MyControl.h"

#include <cstdio>
#include <exception>
#include <vector>

typedef unsigned long long ULL;


MyCommand::MyCommand()
{
}

MyCommand::~MyCommand()
{
}

void MyGetCommand::Execute(MyControl *c)
{
//	c->GetNetWork()->NewMissionD(	uId,
//									c->GetSendRecv()->GetToken(),
//									c->GetUser()->GetUsername(),
//									c->GetUser()->GetPassword());
	c->NewMissionD(	uId,
					c->GetSendRecvToken(),
					c->GetUserNameM(),
					c->GetPassword());
}

void MyPutCommand::Execute(MyControl *c)
{
//	if (ok) {
//		c->GetNetWork()->NewMissionU(fileName,
//			GetUID(),
//			localPath,
//			netPath,
//			c->GetSendRecv()->GetToken(),
//			c->GetUser()->GetUsername());
//	}
	if (ok) {
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
	c->GetManager()->DeleteNetFile(name);
}

void MyForkCommand::Execute(MyControl *c)
{
	std::cout << "haven't implement fork command" << std::endl;
}

void MyLsCommand::Execute(MyControl *c)
{
	if (c->GetManager()->EnterDir(dirName)) {
//		c->GetManager()->ReplaceContent(content);
	}
}

void MyMkDirCommand::Execute(MyControl *c)
{
	t.Execute(c);
}

void MyCopyCommand::Execute(MyControl *c)
{
	c->GetManager()->Paste();
}

void MyMoveCommand::Execute(MyControl *c)
{
	c->GetManager()->Paste();
}

void MyShareCommand::Execute(MyControl *)
{
	std::cout << "haven't implement share command" << std::endl;
}

bool MyTouchCommand::GetServerResponse(const char * info, int len)
{
	if (len != 16) {
		std::cout << "长度不对, len为:" << len << std::endl;
		return false;
	}
	std::string res(info, len);
	std::cout << "前8个字节\n";
	for (int i = 0; i < 8; ++i) {
		printf("%x ", (unsigned char)res[i]);
	}
	std::cout << std::endl;

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
	if (type == "0") {
//		c->GetManager()->AddNormFile(uId, fileName, date, 0);
	}else if(type == "1"){
//		c->GetManager()->AddDirector(uId, fileName, date, 0);
	}
}

void MyRenameCommand::Execute(MyControl *c)
{
	c->GetManager()->Rename(oldName, GetNewName());
}

void MyLogInCommand::Execute(MyControl *c)
{
//	c->GetUser()->LogIn(username, password);
	if(token == c->GetSendRecvToken()){
		//c->GetUser()->LogIn(username, password);			//record the username and password
		//c->GetNetWork()->InitMessageSocket(token, username);//create recv thread
		//c->GetNetWork()->StartMessageSocket();
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
