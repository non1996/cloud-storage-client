#include "MyEnCoder.h"
#include <algorithm>

MyEnCoder* MyEnCoder::instance = 0;

MyEnCoder::MyEnCoder()
{
	SetCoderCFB();
}


MyEnCoder::~MyEnCoder()
{
	Cleaner::Delete<MyCodeImp*>(&coder);
}

std::string MyEnCoder::MD5(std::string & text)
{
	std::string digest;
	Weak1::MD5 md5;
	StringSource(text, true, new HashFilter(md5, new HexEncoder(new StringSink(digest))));
	return digest;
}

std::string MyEnCoder::PrivateFileMD5(std::string & fileName)
{
	std::string connectMD5;
	Weak1::MD5 md5;
	std::locale loc = std::locale::global(std::locale(""));
	std::ifstream file(fileName.c_str(), std::ios_base::binary);
	std::locale::global(loc);
	if (!file.is_open()) {
		return "";
	}
	char* buf = new char[4096 * 1024 + 1];
	if (!buf) {
		return "";
	}

	unsigned long long expected = 4096 * 1024, actually = 0;
	while (1) {
		memset(buf, 0, expected);
		file.clear();
		file.read(buf, expected);
		actually = file.gcount();
		connectMD5 += MyEnCoder::MD5(std::string(buf, actually));
		if (actually < expected) {
			break;
		}
	}
	transform(connectMD5.begin(), connectMD5.end(), connectMD5.begin(), ::tolower);
	delete[] buf;
	return MyEnCoder::MD5(connectMD5);
}

std::string MyEnCoder::FileMD5(std::string & fileName)
{
	Weak1::MD5 md5;
	std::string digest;
	FileSource(fileName.c_str(), true, new HashFilter(md5, new HexEncoder(new StringSink(digest))));
	return digest;
}

MyEnCoder * MyEnCoder::Instance()
{
	if (instance == 0) {
		instance = new MyEnCoder();
	}
	return instance;
}

void MyEnCoder::Release()
{
	Cleaner::Delete<MyEnCoder*>(&instance);
}
