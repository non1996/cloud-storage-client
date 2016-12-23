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

	buffer->GetSendCommand(&cmd);			//�ȴ��û������û���������
	temp = cmd->ToString();
	if (temp.size() <= 8) {
		return "";
	}

	//��֤��Ϣ��ʽ
	//��һ��8�ֽ�Ϊ�û������ĺ�����md5ֵ���ܳ��ȣ�
	//�ڶ���8�ֽ�Ϊ�û���������MD5ֵ���ܺ�����ĳ���
	//������8�ֽڱ�ʾ�û������ȣ���������
	//--------------------------------------------------------------------
	//  8 bytes  |  8 bytes  |  8 bytes  |  encoded username and password  
	//--------------------------------------------------------------------
	std::string userAndPassMD5;
	std::string code;
	std::string sizeUserAndPassMD5;			//�û���������MD5���Ⱥ�
	std::string sizeCode;					//���ܺ�����ĳ���
	std::string sizeUser;					//�û����ĳ���
	
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
	//����û��ǳ�
	if (false == GetSocket()->RecvBytes(name, GetToken().c_str())) {
		SetConnect(false);
		return false;
	}
	//��ȡ�û���ǰ����ʹ����
	if (false == GetSocket()->RecvBytes(rcv, GetToken().c_str())) {
		SetConnect(false);
		return false;
	}
	currentVolume = MyEnCoder::BytesToUll(rcv);
	rcv.clear();
	//��ȡ�û�����������
	if (false == GetSocket()->RecvBytes(rcv, GetToken().c_str())) {
		SetConnect(false);
		return false;
	}
	totalVolume = MyEnCoder::BytesToUll(rcv);
	//��ȡ�û���ͷ������
	if (false == GetSocket()->RecvBytes(graphName, GetToken().c_str())) {
		SetConnect(false);
		return false;
	}
	//������سɹ���ʹ�ø�ͷ�񣬷���ʹ��Ĭ��ͷ��
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

		//�ӷ��Ͷ��л�ȡ�������
		buffer->GetSendCommand(&cmd);		
		if (GetSocket()->SendBytes(cmd->ToString(), GetToken().c_str()) == false) {
			SetLogIn(false);
			SetConnect(false);
			return false;
		}

		//���շ���������Ӧ�������������ն���
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
		if (!Connect()) {			//���ø����ṩ�����Ӻ���
			continue;
		}
		if (!GetTokenFromServer()) {//���ø����ṩ�Ļ�ȡ��Կ�ķ���
			DisConnect();
			ReInit();
			continue;
		}
		if (!Certification()) {		//��֤��Ϣ
			DisConnect();
			ReInit();
			continue;
		}
		if (!GetUserInfo()) {		//��֤�ɹ����ȡ�û���Ϣ
			DisConnect();
			ReInit();
			continue;
		}
		break;
	}
	Communicate();		//�����������
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
