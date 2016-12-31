#include "MyControl.h"
#include "MyDownLoadMission.h"
#include "MyUpLoadMission.h"
#include "../MyMainWindow.h"
#include "Split.h"
MyControl* MyControl::instance = 0;

MyControl::MyControl()
{
	buffer = 0;
	netFileManager = 0;
	user = 0;
	sendRecv = 0;
	recv = 0;
	downloadManager = 0;
	uploadManager = 0;
	mainWindow = 0;
	isFinish = false;
}

MyControl::~MyControl()
{
	Cleaner::Release<MyUserInfo*>(&user);
	Cleaner::Release<MyNetFileManager*>(&netFileManager);
	Cleaner::Release<MySendRecvThread*>(&sendRecv);
	Cleaner::Release<MyRecvThread*>(&recv);

	downloadManager->Release();
	uploadManager->Release();
	Cleaner::Delete<MyMissionManager*>(&downloadManager);
	Cleaner::Delete<MyMissionManager*>(&uploadManager);
}

bool MyControl::InitSendRecvThread()
{
	sendRecv = MySendRecvThread::Instance();
	bool r = false;
	r = sendRecv->Init(serverIP.c_str(), serverPort);
	if (r == false) {
		Cleaner::Release<MySendRecvThread*>(&sendRecv);
		return false;
	}
	return true;
}

bool MyControl::StartSendRecvThread()
{
	if (!sendRecv) {
		return false;
	}

	bool r = false;
	r = sendRecv->Start();
	if (r == false) {
		sendRecv->Close();
		return false;
	}
	sendRecv->Resume();
	return true;
}

bool MyControl::CloseStartSendRecvThread()
{
	sendRecv->Close();
	return true;
}

bool MyControl::InitMessageSocket(std::string & token, std::string & username)
{
	recv = MyRecvThread::Instance();
	recv->SetUsername(username);
	recv->SetTokenOne(token);
	bool r = false;
	r = recv->Init(serverIP.c_str(), serverPort);
	if (r == false) {
		Cleaner::Release<MyRecvThread*>(&recv);
		return false;
	}
	return true;
}

bool MyControl::StartMessageSocket()
{
	if (!recv) {
		return false;
	}

	bool r = false;
	r = recv->Start();
	if (r == false) {
		recv->Close();
		return false;
	}
	recv->Resume();
	return true;
}

bool MyControl::CloseMessageSocket()
{
	recv->Close();
	return true;
}

bool MyControl::InitTransmitManager()
{
	downloadManager = new MyMissionManager();
	if (downloadManager == 0) {
		return false;
	}
	
	uploadManager = new MyMissionManager();
	if (uploadManager == 0) {
		Cleaner::Delete<MyMissionManager*>(&downloadManager);
		return false;
	}
	downloadManager->SetMaxCurrency(3);
	uploadManager->SetMaxCurrency(1);
	return true;
}

bool MyControl::Init() {
	buffer = MyCommandBuffer::Instance();
	netFileManager = MyNetFileManager::Instance();
	user = MyUserInfo::Instance();

	if (!ReadNetInfo()) {
		return false;
	}
	if (!InitSendRecvThread()) {
		return false;
	}
	if (!InitTransmitManager()) {
		return false;
	}
	if (!StartSendRecvThread()) {
		return false;
	}
	Start();
	Resume();
	return true;
}

void MyControl::SetMainWindowReferance(MyMainWindow *m)
{
	mainWindow = m;
}

bool MyControl::ReadNetInfo()
{
	std::ifstream fin("init.txt");
	if (!fin.is_open()) {
		return false;
	}
	std::getline(fin, serverIP, '\n');
	std::string port;
	std::getline(fin, port, '\n');
	serverPort = std::stoi(port);
	return true;
}

bool MyControl::NewMissionD(std::string & uID, std::string & token_one, std::string & username, std::string & password)
{
	MyDownLoadMission* temp = 0;
	temp = new MyDownLoadMission();
	if (temp == 0) {
		return false;
	}
	if (temp->InitSocket(serverIP.c_str(), serverPort) == false) {
		Cleaner::Release<MyDownLoadMission*>(&temp);
		return false;
	}
	temp->SetTokenOne(token_one);
	temp->SetUsername(username);
	temp->SetFileID(uID);
	temp->SetPassword(password);
	temp->SetManager(downloadManager);
	downloadManager->NewMission(temp);
	return true;
}

bool MyControl::NewMissionU(std::string & fileName, unsigned long long uID, std::string & localPath, std::string & netPath, std::string & token_one, std::string & username)
{
	MyUpLoadMission* temp = 0;
	temp = new MyUpLoadMission();
	if (temp == 0) {
		return false;
	}
	if (!temp->InitSocket(serverIP.c_str(), serverPort)) {
		Cleaner::Release<MyUpLoadMission*>(&temp);
		return false;
	}
	for (int i = 0; i < localPath.size(); ++i) {
		if (localPath.at(i) == '/') {
			localPath.at(i) = '\\';
		}
	}
	temp->SetTokenOne(token_one);
	temp->SetUsername(username);
	temp->SetUID(uID);
	temp->SetFileName(fileName);
	temp->SetLocalPath(localPath);
	temp->SetNetPath(netPath);
	temp->SetManager(uploadManager);
	uploadManager->NewMission(temp);
	
	return true;
}

void MyControl::ShowAlreadySendMessage(std::string &c, std::string &m)
{
	mainWindow->sendSuccess(c, m);
}

void MyControl::ShowReceiveMessage(std::string &c, std::string &m)
{
	mainWindow->showMessage(c, m);
}

void MyControl::Rename(std::string &oldName, std::string &newName)
{
	netFileManager->Rename(oldName, newName);
	mainWindow->Rename(oldName, newName);
}

void MyControl::Paste()
{
	netFileManager->Paste();
	mainWindow->AddFile(netFileManager->GetCopyFileName());
}

bool MyControl::SetPath(std::string & s)
{
	std::vector<std::string> path;
	path.push_back(std::string(""));
	split::split(s, '/', path);
	netFileManager->SetPath(path);

	if (mainWindow) {
		mainWindow->SetPath(s);
	}
	return true;
}

bool MyControl::ReplaceContent(std::vector<std::string>& fileInfo)
{
	while (mainWindow == 0) {}
	netFileManager->ReplaceContent(fileInfo);
	mainWindow->CleanBrowser();
	for (unsigned int i = 1; i < fileInfo.size(); ++i) {
		std::vector<std::string> temp;
		bool share = false;
		split::split(fileInfo[i], '+', temp);
		if (temp.size() != 7) {
			continue;
		}
		if (temp[5] == "1") {
			share = true;
		}
		if (temp[6] == "DIR") {
			netFileManager->AddDirector(temp[0], temp[1], temp[2], temp[3], std::stoull(temp[4]), share);
		}
		else {
			netFileManager->AddNormFile(temp[0], temp[1], temp[2], temp[3], std::stoull(temp[4]), share);
		}
		mainWindow->AddFile(temp[2]);
	}

	return false;
}

bool MyControl::IsConnect()
{
	return sendRecv->IsConnect();
}

bool MyControl::IsLogIn()
{
	return sendRecv->IsLogIn();
}

bool MyControl::LogIn(std::string & un, std::string & pw)
{
	return user->LogIn(un, pw);
}

void MyControl::ShowShareInfo(bool b)
{
	mainWindow->showShareInfo(b);
}

void MyControl::ShowForkInfo(bool b)
{
	mainWindow->showForkInfo(b);
}

void MyControl::ResumeAllNotComplete()
{
	downloadManager->ResumeAll();
	uploadManager->ResumeAll();
}

void MyControl::SetUserInfo(std::string &name, std::string &url, unsigned long long currentV, unsigned long long totalV)
{
	while (mainWindow == 0) {}
	mainWindow->SetUserInfo(name, url, currentV, totalV);
}

std::string MyControl::GetPath(std::string & name)
{
	return netFileManager->GetFilePath(name);
}

bool MyControl::AddNormFile(std::string & uId, std::string & path, std::string & name, std::string & date, unsigned long long size, bool shared)
{
	netFileManager->AddNormFile(uId, path, name, date, size, shared);
	mainWindow->AddFile(name);
	return true;
}

bool MyControl::AddDirector(std::string & uId, std::string & path, std::string & name, std::string & date, unsigned long long size, bool shared)
{
	netFileManager->AddDirector(uId, path, name, date, size, shared);
	mainWindow->AddFile(name);
	return true;
}

void MyControl::SetCapasity(unsigned long long c)
{
	mainWindow->SetCapasity(c);
}

void MyControl::DeleteNetFile(std::string & name)
{
	netFileManager->DeleteNetFile(name);
	mainWindow->DeleteNetFile(name);
}

void MyControl::GetProgress(MyMissionManager* m, int i, float pro, unsigned int speed)
{
	if (m == downloadManager) {
		mainWindow->SetDownloadProgress(i, pro, speed);
	}
	else if (m == uploadManager) {
		mainWindow->SetUploadProgress(i, pro, speed);
	}
}

void MyControl::Complete(MyMissionManager* m, int i)
{
	if (m == downloadManager) {
		mainWindow->DownloadComplete(i);
	}
	else if (m == uploadManager) {
		mainWindow->UploadComplete(i);
	}
}

void MyControl::Restart()
{
	sendRecv->ReInit();
	sendRecv->_Close();
	sendRecv->Start();
	sendRecv->Resume();
}

void MyControl::CloseAllTransmit()
{
	sendRecv->Close();
	sendRecv->DisConnect();
	if (recv) {
		recv->Close();
		recv->DisConnect();
	}
	downloadManager->CloseAllThread();
	uploadManager->CloseAllThread();
}

void MyControl::Execute()
{
	MyCommand* temp = 0;
	while (!IsFinish()) {
		buffer->GetRecvCommand(&temp);
		temp->Execute(this);
		Cleaner::Delete<MyCommand*>(&temp);
	}
}

MyControl * MyControl::Instance()
{
	if (instance == 0) {
		instance = new MyControl();
	}
	return instance;
}

void MyControl::Release()
{
	Cleaner::Delete<MyControl*>(&instance);
}
