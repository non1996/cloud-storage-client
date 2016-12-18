#include "MyController.h"
#include "../MyMainWindow.h"

MyController* MyController::instance = 0;

MyController::MyController()
{
}

MyController::~MyController()
{
	Cleaner::Release<MyControl*>(&control);
	Cleaner::Release<MyCommandBuffer*>(&buffer);
}

void MyController::SetMainWindowReferance(MyMainWindow *m)
{
	mainWindow = m;
	control->SetMainWindowReferance(m);
}

void MyController::Close()
{
	control->CloseAllTransmit();
	control->Close();
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
