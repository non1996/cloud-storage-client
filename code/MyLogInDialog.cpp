# pragma execution_character_set("utf-8")
#include "MyLogInDialog.h"
#include "MyBase\MyController.h"

MyLogInDialog::MyLogInDialog(QWidget* parent)
    :QDialog(parent)
{
    lpU = new QLabel("username", this);
    lpU->setMinimumWidth(40);
    lpP = new QLabel("password", this);
    lpP->setMinimumWidth(40);
	lpHint = new QLabel(this);
	lpHint->setMinimumHeight(20);

    lpUsername = new QLineEdit("forec@bupt.edu.cn", this);
    lpUsername->setPlaceholderText("input username");
    lpPassword = new QLineEdit("TESTTHISPASSWORD", this);
    lpPassword->setPlaceholderText("input password");

    lpLogIn = new QPushButton("log in", this);
    lpCancel = new QPushButton("cancel", this);

    lpMainLayout = new QVBoxLayout(this);
    lpSubLayout1 = new QHBoxLayout(this);
    lpSubLayout2 = new QHBoxLayout(this);
    lpSubLayout3 = new QHBoxLayout(this);

    lpMainLayout->addLayout(lpSubLayout1);
    lpMainLayout->addLayout(lpSubLayout2);
    lpMainLayout->addLayout(lpSubLayout3);
	lpMainLayout->addWidget(lpHint);

    lpSubLayout1->addWidget(lpU);
    lpSubLayout1->addWidget(lpUsername);
    lpSubLayout2->addWidget(lpP);
    lpSubLayout2->addWidget(lpPassword);
    lpSubLayout3->addWidget(lpLogIn);
    lpSubLayout3->addWidget(lpCancel);
    setLayout(lpMainLayout);

    connect(lpLogIn, SIGNAL(clicked(bool)), this, SLOT(ok()));
    connect(lpCancel, SIGNAL(clicked(bool)), this, SLOT(cancel()));

}

void MyLogInDialog::ok()
{
	if (lpUsername->text().isEmpty() || lpPassword->text().isEmpty()) {
		lpHint->setText("wrong input");
		return;
	}
	MyController* c = MyController::Instance();
	if (c->GetControl()->IsConnect()) {
		std::string username, password;
		username = lpUsername->text().toStdString();
		password = lpPassword->text().toStdString();
		c->PushLogInCommand(username, password);
		while (!c->GetControl()->IsLogIn()) {
			if (!c->GetControl()->IsConnect()) {
				lpHint->setText("error username or password");
				return;
			}
			lpHint->setText("is loging...");
		}
		c->PushLsCommand(std::string("0"), std::string(""), std::string("/"), std::vector<std::string>());
		accept();
	}
	else {
		lpHint->setText("net disconnect");
	}
}

void MyLogInDialog::cancel()
{
    close();
}

