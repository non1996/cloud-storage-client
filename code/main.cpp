#include "MyLogInDialog.h"
#include "MyMainWindow.h"
#include "MyBase\MyController.h"
#include <QApplication>


int main(int argc, char *argv[]){
	QApplication a(argc, argv);
	MyController* controller = MyController::Instance();

	if (!controller->Init()) {
		return 0;
	}
    MyLogInDialog* login;
    login = new MyLogInDialog();
	MyMainWindow w;
	controller->SetMainWindowReferance(&w);
    if(login->exec() != QDialog::Accepted){
		controller->Close();
		controller->Release();
		return 0;
    }
    
	w.show();

    return a.exec();
}
