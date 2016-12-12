//---------------------------------------------------
//	this head contain test code for each module
//---------------------------------------------------
#include "TotalTest.h"

#include <iostream>
#include <algorithm>
using namespace std;

#include "MyEnCoder.h"
#include <sstream>

char key[33] = "7bc7c60b804b0323c2bd294f81ffe54d";

bool SendFromReader()
{
	char *sendBuf = new char[4096 * 1024];
	if (sendBuf == 0) {
		std::cout << "发送缓冲创建失败\n";
		return false;
	}
	std::ifstream file("D:\\test.jpg", std::ios_base::binary);
	std::string sendCode;
	string sizeCode;
	string sizeRead;
	unsigned long long sendLength = 4096 * 1024;
	unsigned long long encodeBufLen = 4096 * 1024 / 3 - 16;
	unsigned long long actualLength = 0;
	unsigned long long alrSend = 0;

	file.read(sendBuf, sendLength);
	actualLength = file.gcount();
	std::cout << "实际读出的文件大小: " << actualLength << std::endl;

	for (int i = 0; i < 16; ++i) {
		printf("%x ", (unsigned char)sendBuf[actualLength - 16 + i]);
	}
	std::cout << "\n为文件块加密\n";
	sendCode = MyEnCoder::Instance()->Encode(sendBuf, key, actualLength);
	sizeCode = MyEnCoder::UllToBytes(sendCode.size());
	sizeRead = MyEnCoder::UllToBytes(actualLength);
	sizeRead += sizeCode + sendCode;
	
	for (int i = 0; i < 8; ++i) {
		printf("%x ", (unsigned char)sizeCode[i]);
	}
	cout << endl;
	for (int i = 0; i < 8; ++i) {
		printf("%x ", (unsigned char)sizeRead[i]);
	}
	cout << endl << "发送包长：" << sizeRead.size() << endl;
	

	cout << endl;
	std::string dec;
	dec = MyEnCoder::Instance()->Decode(sendCode.c_str(), key, sendCode.size());
	std::cout << "解码长度：" << dec.size() << std::endl;
	std::cout << "最后16个字节:\n";
	for (int i = 0; i < 16; ++i) {
		printf("%x ", (unsigned char)dec[dec.size() - 16 + i]);
	}

	std::cout << endl << endl << endl;
	for (int i = 279000; i < sendCode.size(); ++i) {
		printf("%d ", (unsigned char)sendCode[i]);
	}

	delete[] sendBuf;
	return true;
}

int main() {
//	Test();
//	string temp = "haha.txt";
//	cout << PrivateFileMD5(temp);
	test();
//	std::cout << MyEnCoder::PrivateFileMD5(std::string("D:\\6")) << std::endl;
//	std::cout << MyEnCoder::PrivateFileMD5(std::string("D:\\test.jpg")) << std::endl;
//	std::cout << MyEnCoder::FileMD5(std::string("D:\\6.jpg")) << std::endl;
//	std::cout << MyEnCoder::FileMD5(std::string("D:\\test.jpg")) << std::endl;

//	char* buf = new char[4096 * 1024];

	
//	SendFromReader();

	system("pause");
	return 0;
}
