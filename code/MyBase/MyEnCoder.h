#pragma once

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "Cleaner.h"
#include <string>
#include <aes.h>
#include <hex.h>
#include <md5.h>
#include <files.h>
#include <modes.h>

using namespace CryptoPP;

//--------------------------
//	声明加解密器的接口，子类通过实现接口来得到使用不同加密算法的加密器
//--------------------------
class MyCodeImp {
public:
	virtual const char* GetKey() = 0;
	//第一个参数是需要加密或解密的字符串，第二个参数是秘钥，第三个参数是字符串长度
	virtual std::string Encode(const char*, const char*, unsigned int = 0) = 0;
	virtual std::string Decode(const char*, const char*, unsigned int = 0) = 0;
};


//-----------------------------------------
//	AES CFB模式加解密器，实现了加解密器的接口，并定义它的具体实现
//-----------------------------------------
class MyCFBImp : public MyCodeImp {
private:
	char key[AES::DEFAULT_KEYLENGTH * 2 + 1];

	//AES加密需要使用的秘钥向量
	byte iv[AES::BLOCKSIZE] = { 0x00, 0x01, 0x02, 0x03, 0x04,
								0x05, 0x06, 0x07, 0x08, 0x09,
								0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
public:
	MyCFBImp() {
		memset(key, 0, sizeof(key));
	}

	MyCFBImp(const char* key) {
		SetKey(key);
	}

	void SetKey(const char* key) {
		strcpy(this->key, key);
	}

	virtual const char* GetKey() {
		return key;
	}

	virtual std::string Encode(const char* input, const char* passPhrase, unsigned int size) {
		std::string outstr;

		AES::Encryption aesEncryption((byte *)passPhrase, AES::DEFAULT_KEYLENGTH * 2);
		CFB_Mode_ExternalCipher::Encryption cfbEncryption(aesEncryption, iv);
		StreamTransformationFilter cfbEncryptor(cfbEncryption, new StringSink(outstr));
		cfbEncryptor.Put((byte *)input, size);
		cfbEncryptor.MessageEnd();
		return outstr;
	}

	virtual std::string Decode(const char* input, const char* passPhrase, unsigned int size) {
		std::string outstr;
		CFB_Mode<AES>::Decryption cfbDecryption((byte*)passPhrase, AES::DEFAULT_KEYLENGTH * 2, iv);
		StreamTransformationFilter decryptor(cfbDecryption, new StringSink(outstr));
		decryptor.Put((byte*)input, size);
		decryptor.MessageEnd();
		return outstr;
	}
};

//--------------------------------------------------------
//	加解密器的抽象，维护一个加解密器的实现使得可以在程序运行时改变加解密算法
//--------------------------------------------------------
class MyEnCoder
{
private:
	//整个程序只有一个加密器，因此使用单例模式
	static MyEnCoder* instance;	

	MyCodeImp* coder;
public:
	MyEnCoder();
	~MyEnCoder();

	void SetCoderCFB() {
		SetCoder(new MyCFBImp());
	}

	void SetCoder(MyCodeImp* c) {
		coder = c;
	}

	std::string GetKey() {
		return coder->GetKey();
	}

	std::string Encode(const char* input, const char* key, unsigned int size) {
		return coder->Encode(input, key, size);
	}

	std::string Decode(const char* input, const char* key, unsigned int size) {
		return coder->Decode(input, key, size);
	}

	//int64变量和8字节的大端序字符串相互转换
	#pragma region Exchange Int64 to bytes (upper bound)
	static std::string UllToBytes(unsigned long long num) {
		static char out[8];

		memset(out, 0, sizeof(out));
		for (int i = 0; i < 8; ++i) {
			out[7 - i] = num & 0xff;
			num = num >> 8;
		}
		return std::string(out, 8);
	}

	static unsigned long long BytesToUll(std::string c) {
		if (c.size() < 8) {
			return 0;
		}
		unsigned long long temp = 0;
		for (int i = 0; i < 8; ++i) {
			temp = temp << 8;
			temp ^= 0xff & c[i];
		}
		return temp;
	}

	static unsigned long long BytesToUll(const char* c) {
		unsigned long long temp = 0;
		for (int i = 0; i < 8; ++i) {
			temp = temp << 8;
			temp ^= 0xff & c[i];
		}
		return temp;
	}

	#pragma endregion

	//提供MD5算法
	#pragma region Md5
	static std::string MD5(std::string &text);

	//协议自定的文件MD5计算方法
	static std::string PrivateFileMD5(std::string &fileName);

	static std::string FileMD5(std::string &fileName);
	#pragma endregion

	static MyEnCoder* Instance();
	static void Release();
};

