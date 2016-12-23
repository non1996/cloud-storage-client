#include "MySendRecvThread.h"
#include "MyEnCoder.h"
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

MySendRecvThread* MySendRecvThread::instance = 0;

MySendRecvThread::MySendRecvThread(): MyTransmitThreadBase()
{
	buffer = 0;
}


MySendRecvThread::~MySendRecvThread()
{
//	buffer = 0;
}

bool MySendRecvThread::Init(const char * ipAddr, int port)
{
	if (true == MyTransmitThreadBase::Init(ipAddr, port)) {
		buffer = MyCommandBuffer::Instance();
		return true;
	}
	return false;
}

std::string MySendRecvThread::GetCertificationInfo()
{
	std::string temp;

	buffer->GetSendCommand(&cmd);			//等待用户输入用户名和密码
	temp = cmd->ToString();
	if (temp.size() <= 8) {
		return "";
	}

	//验证信息格式
	//第一个8字节为用户名明文和密码md5值的总长度，
	//第二个8字节为用户名和密码MD5值加密后的密文长度
	//第三个8字节表示用户名长度，最后跟密文
	//--------------------------------------------------------------------
	//  8 bytes  |  8 bytes  |  8 bytes  |  encoded username and password  
	//--------------------------------------------------------------------
	std::string userAndPassMD5;
	std::string code;
	std::string sizeUserAndPassMD5;			//用户名和密码MD5长度和
	std::string sizeCode;					//加密后的密文长度
	std::string sizeUser;					//用户明文长度
	
	sizeUserAndPassMD5 = temp.substr(0, 8);
	sizeUser = temp.substr(8, 8);
	userAndPassMD5 = temp.substr(16);
	code = MyEnCoder::Instance()->Encode(userAndPassMD5.c_str(), GetToken().c_str(), userAndPassMD5.size());
	sizeCode = MyEnCoder::UllToBytes((unsigned long long)code.size());

	return sizeUserAndPassMD5 + sizeCode + sizeUser + code;
}

bool MySendRecvThread::InfoRight()
{
	std::string rcv;
	if (false == GetSocket()->RecvBytes(rcv, GetToken().c_str())) {
		SetConnect(false);
		return false;
	}
	cmd->GetServerResponse(rcv.c_str(), rcv.size());
	buffer->PutRecvCommand(cmd);
	return true;
}

bool MySendRecvThread::GetUserInfo()
{
	std::string name;
	unsigned long long currentVolume, totalVolume;
	std::string graphName;
	std::string rcv;
	//获得用户昵称
	if (false == GetSocket()->RecvBytes(name, GetToken().c_str())) {
		SetConnect(false);
		return false;
	}
	//获取用户当前网盘使用量
	if (false == GetSocket()->RecvBytes(rcv, GetToken().c_str())) {
		SetConnect(false);
		return false;
	}
	currentVolume = MyEnCoder::BytesToUll(rcv);
	rcv.clear();
	//获取用户网盘总容量
	if (false == GetSocket()->RecvBytes(rcv, GetToken().c_str())) {
		SetConnect(false);
		return false;
	}
	totalVolume = MyEnCoder::BytesToUll(rcv);
	//获取用户的头像链接
	if (false == GetSocket()->RecvBytes(graphName, GetToken().c_str())) {
		SetConnect(false);
		return false;
	}
	//如果下载成功则使用该头像，否则使用默认头像
	MyCommand* uInfo = 0;
	std::ofstream test("test.txt", std::ios_base::ate);
	test << "123456:::::" << graphName;
	test.flush();
	test.close();
	if (graphName == "none") {
		graphName = "temp\\default.jpg";
		uInfo = MyCommandBuilder::MakeSetInfoCommand(name, graphName, currentVolume, totalVolume);
	}
	else if (graphName.find('/') == std::string::npos) {
		std::string url = "http://cloud.forec.cn/static/thumbnail/";
		url += graphName;
		graphName = "temp\\" + graphName;
		HRESULT hr = URLDownloadToFileA(0, url.c_str(), graphName.c_str(), 0, NULL);
		if (hr == S_OK) {
			uInfo = MyCommandBuilder::MakeSetInfoCommand(name, graphName, currentVolume, totalVolume);
		}
		else {
			graphName = "temp\\default.jpg";
			uInfo = MyCommandBuilder::MakeSetInfoCommand(name, graphName, currentVolume, totalVolume);
		}
	}
	else if(graphName == "http://www.avastar.com/askldfjdasjfklsajgjaklsj"){
		HRESULT hr = URLDownloadToFileA(0, "http://www.avastar.com/askldfjdasjfklsajgjaklsj", "temp\\random.jpg", 0, NULL);
		if (hr == S_OK) {
			uInfo = MyCommandBuilder::MakeSetInfoCommand(name, std::string("temp\\random.jpg"), currentVolume, totalVolume);
		}
		else {
			uInfo = MyCommandBuilder::MakeSetInfoCommand(name, std::string("temp\\default.jpg"), currentVolume, totalVolume);
		}
	}
	else {
		uInfo = MyCommandBuilder::MakeSetInfoCommand(name, std::string("temp\\default.jpg"), currentVolume, totalVolume);
	}
	
	buffer->PutRecvCommand(uInfo);
	return true;
}

bool MySendRecvThread::Communicate()
{
	while (!IsFinish()) {
		std::string readBuf;

		//从发送队列获取命令并发送
		buffer->GetSendCommand(&cmd);		
		if (GetSocket()->SendBytes(cmd->ToString(), GetToken().c_str()) == false) {
			SetLogIn(false);
			SetConnect(false);
			return false;
		}

		//接收服务器的响应并将命令放入接收队列
		if (GetSocket()->RecvBytes(readBuf, GetToken().c_str()) == false) {
			SetLogIn(false);
			SetConnect(false);
			return false;
		}
		cmd->GetServerResponse(readBuf.c_str(), readBuf.size());
		buffer->PutRecvCommand(cmd);
	}
	return true;
}

void MySendRecvThread::Execute()
{
	if (!IsInit()) {
		return;
	}
	
	while (!IsFinish()) {
		if (!Connect()) {			//调用父类提供的连接函数
			continue;
		}
		if (!GetTokenFromServer()) {//调用父类提供的获取秘钥的方法
			DisConnect();
			ReInit();
			continue;
		}
		if (!Certification()) {		//验证信息
			DisConnect();
			ReInit();
			continue;
		}
		if (!GetUserInfo()) {		//验证成功后获取用户信息
			DisConnect();
			ReInit();
			continue;
		}
		break;
	}
	Communicate();		//与服务器交互
	SetLogIn(false);
	DisConnect();
}

MySendRecvThread * MySendRecvThread::Instance()
{
	if (instance == 0) {
		instance = new MySendRecvThread();
	}
	return instance;
}

void MySendRecvThread::Release()
{
	Cleaner::Delete<MySendRecvThread*>(&instance);
}
