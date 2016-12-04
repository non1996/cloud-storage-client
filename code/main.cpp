//---------------------------------------------------
//	this head contain test code for each module
//---------------------------------------------------
#include "TotalTest.h"

#include <iostream>
using namespace std;

#include "MyEnCoder.h"
#include <sstream>
/*typedef unsigned long long ULL;
std::string username = "non189", password = "non189";



std::string ToString()
{
	MyEnCoder* instance = MyEnCoder::Instance();

	std::string md5Password;
	std::string userAndPassMD5;
	std::string code;

	std::string sizeUserAndPassMD5;			//用户名和密码MD5长度和
	std::string sizeCode;					//加密后的密文长度
	std::string sizeUser;					//用户明文长度

	md5Password = MyEnCoder::MD5(password);
	userAndPassMD5 = username + md5Password;
	code = instance->Encode(userAndPassMD5.c_str());

	sizeUser = MyEnCoder::UllToBytes((ULL)username.size());
	sizeUserAndPassMD5 = MyEnCoder::UllToBytes((ULL)userAndPassMD5.size());
	sizeCode = MyEnCoder::UllToBytes((ULL)code.size());

	return sizeUserAndPassMD5 + sizeCode + sizeUser + code;
}
*/
int main() {
//	Test();
	test();
/*	MyEnCoder* instance = MyEnCoder::Instance();
	instance->SetCoderCFB("AABCDEFGHIJKLMNOPBCDEFGHIJKLMNOP");

	std::string temp = ToString();
	for (int i = 0; i < temp.size(); ++i) {
		printf("%x ", (unsigned char)temp[i]);
	}*/
	system("pause");
	return 0;
}
