#include "MyEnCoder.h"

MyEnCoder* MyEnCoder::instance = 0;

MyEnCoder::MyEnCoder()
{
	SetCoderCFB();
}


MyEnCoder::~MyEnCoder()
{
	Cleaner::Delete<MyCodeImp*>(&coder);
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
