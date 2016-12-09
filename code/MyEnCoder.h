#pragma once

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "Cleaner.h"
#include <string>
#include <aes.h>
#include <hex.h>
#include <md5.h>
#include <files.h>
#include <modes.h>
#include <algorithm>

using namespace CryptoPP;

//--------------------------
//	implementor
//--------------------------
class MyCodeImp {
public:
	//the first param is the block you want to encode(decode)
	//the seconde param is the key
	//the third param is the size of block
	virtual const char* GetKey() = 0;
	virtual std::string Encode(const char*, const char*, unsigned int = 0) = 0;
	virtual std::string Decode(const char*, const char*, unsigned int = 0) = 0;
};

class MyCFBImp : public MyCodeImp {
private:
	char key[AES::DEFAULT_KEYLENGTH * 2 + 1];
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

class MyEnCoder
{
private:
	//use to encode and decode message or file block
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
//			temp += (unsigned int)c[i];
			temp ^= 0xff & c[i];
//			std::cout << "temp is:" << temp << std::endl;
		}
		return temp;
	}

	static unsigned long long BytesToUll(const char* c) {
		unsigned long long temp = 0;
		for (int i = 0; i < 8; ++i) {
			temp = temp << 8;
//			temp += (unsigned int)c[i];
			temp ^= 0xff & c[i];
		}
		return temp;
	}

	#pragma endregion

	#pragma region Md5
	static std::string MD5(std::string &text) {
		std::string digest;
		Weak1::MD5 md5;
		StringSource(text, true, new HashFilter(md5, new HexEncoder(new StringSink(digest))));
		return digest;
	}

	//协议自定文件MD5计算方法
	static std::string PrivateFileMD5(std::string &fileName) {
		std::string connectMD5;
		Weak1::MD5 md5;
		std::ifstream file(fileName.c_str(), std::ios_base::binary);
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
			std::cout << actually << std::endl;
			connectMD5 += MyEnCoder::MD5(std::string(buf, actually));
			if (actually < expected) {
				break;
			}
		}
		std::cout << connectMD5 << std::endl;
		transform(connectMD5.begin(), connectMD5.end(), connectMD5.begin(), ::tolower);
		delete[] buf;
		return MyEnCoder::MD5(connectMD5);
	}

	static std::string FileMD5(std::string &fileName) {
		Weak1::MD5 md5;
		std::string digest;
		FileSource(fileName.c_str(), true, new HashFilter(md5, new HexEncoder(new StringSink(digest))));
		return digest;
	}
	#pragma endregion

	static MyEnCoder* Instance();
	static void Release();
};

