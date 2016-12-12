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
	r = sendRecv->Init(SERVER_IP, SERVER_PORT);
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
	r = recv->Init(SERVER_IP, SERVER_PORT);
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

	if (!InitSendRecvThread()) {
		return false;
	}
	std::cout << "交互线程初始化成功----control\n";
	if (!InitTransmitManager()) {
		return false;
	}
	std::cout << "传输管理器初始化成功----control\n";
	if (!StartSendRecvThread()) {
		return false;
	}
	std::cout << "交互线程启动----control\n";
	Start();
	Resume();
	std::cout << "控制线程启动----control\n";
	return true;
}

void MyControl::SetMainWindowReferance(MyMainWindow *m)
{
	mainWindow = m;
}

bool MyControl::NewMissionD(std::string & uID, std::string & token_one, std::string & username, std::string & password)
{
	MyDownLoadMission* temp = 0;
	temp = new MyDownLoadMission();
	if (temp == 0) {
		return false;
	}
	if (temp->InitSocket(SERVER_IP, SERVER_PORT) == false) {
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
	if (!temp->InitSocket(SERVER_IP, SERVER_PORT)) {
		Cleaner::Release<MyUpLoadMission*>(&temp);
		return false;
	}
	temp->SetTokenOne(token_one);
	temp->SetUsername(username);
	temp->SetUID(uID);
	temp->SetFileName(fileName);
	temp->SetLocalPath(localPath);
	temp->SetNetPath(netPath);
	temp->SetManager(uploadManager);
	uploadManager->NewMission(temp);
//	mainWindow->AddUploadMission(fileName);
	return true;
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
	netFileManager->ReplaceContent(fileInfo);
	mainWindow->CleanBrowser();
	for (int i = 1; i < fileInfo.size(); ++i) {
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
	return user->IsLogIn();
}

bool MyControl::LogIn(std::string & un, std::string & pw)
{
	return user->LogIn(un, pw);
}

//void MyControl::showInfo(std::string content)
//{
//	mainWindow->showInfo(QString::fromStdString(content));
//}

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

void MyControl::DeleteNetFile(std::string & name)
{
	netFileManager->DeleteNetFile(name);
	mainWindow->DeleteNetFile(name);
}

void MyControl::GetProgress(MyMissionManager* m, int i, float pro, unsigned int speed)
{
	if (m == downloadManager) {
		std::cout << "下载任务" << i << "的进度为 " << pro << std::endl;
		mainWindow->SetDownloadProgress(i, pro, speed);
	}
	else if (m == uploadManager) {
//		std::cout << "上传任务" << i << "的进度为 " << pro << std::endl;
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

void MyControl::CloseAllTransmit()
{
	sendRecv->Close();
	std::cout << "sendRecv退出\n";
	sendRecv->DisConnect();
	if (recv) {
		recv->Close();
		std::cout << "recv退出\n";
		recv->DisConnect();
	}
	downloadManager->CloseAllThread();
	std::cout << "关闭所有下载线程\n";
	uploadManager->CloseAllThread();
	std::cout << "关闭所有上传线程\n";
}

void MyControl::Execute()
{
	MyCommand* temp = 0;
	while (!IsFinish()) {
		buffer->GetRecvCommand(&temp);
		temp->Execute(this);
		Cleaner::Delete<MyCommand*>(&temp);
	}
	std::cout << "control quit\n";
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
