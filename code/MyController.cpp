#include "MyController.h"

MyController* MyController::instance = 0;

MyController::MyController()
{
}

MyController::~MyController()
{
	control->SetFinish();
	Sleep(1000);
	control->Close();
	std::cout << "control thread close\n";
	Cleaner::Release<MyControl*>(&control);
	
	Cleaner::Release<MyCommandBuffer*>(&buffer);
	std::cout << "buffer clear\n";
}

MyController * MyController::Instance()
{
	if (instance == 0) {
		instance = new MyController();
	}
	return instance;
}

void MyController::Release()
{
	Cleaner::Delete<MyController*>(&instance);
}
