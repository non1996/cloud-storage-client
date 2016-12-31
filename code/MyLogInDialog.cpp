# pragma execution_character_set("utf-8")
#include "MyLogInDialog.h"
#include "MyBase\MyController.h"

MyLogInDialog::MyLogInDialog(QWidget* parent)
    :QDialog(parent)
{
    lpU = new QLabel("用户名", this);
    lpU->setMinimumWidth(40);
    lpP = new QLabel("密码", this);
    lpP->setMinimumWidth(40);
	lpHint = new QLabel(this);
	lpHint->setMinimumHeight(10);

    lpUsername = new QLineEdit("non1996@forec.cn", this);
    lpUsername->setPlaceholderText("输入用户名");
    lpPassword = new QLineEdit("zenith", this);
    lpPassword->setPlaceholderText("输入密码");
	lpPassword->setEchoMode(QLineEdit::Password);
	lpUsername->setMinimumWidth(280);
	lpPassword->setMinimumWidth(280);

    lpLogIn = new QPushButton("登录", this);
    lpCancel = new QPushButton("取消", this);

    lpMainLayout = new QVBoxLayout(this);
    lpSubLayout1 = new QHBoxLayout(this);
    lpSubLayout2 = new QHBoxLayout(this);
    lpSubLayout3 = new QHBoxLayout(this);

    lpMainLayout->addLayout(lpSubLayout1);
    lpMainLayout->addLayout(lpSubLayout2);
	lpMainLayout->addWidget(lpHint);
	lpMainLayout->addLayout(lpSubLayout3);

    lpSubLayout1->addWidget(lpU);
	lpSubLayout1->addStretch();
    lpSubLayout1->addWidget(lpUsername);
    lpSubLayout2->addWidget(lpP);
	lpSubLayout2->addStretch();
	lpSubLayout2->addWidget(lpPassword);
    lpSubLayout3->addWidget(lpLogIn);
    lpSubLayout3->addWidget(lpCancel);
    setLayout(lpMainLayout);
	setAutoFillBackground(true);
    connect(lpLogIn, SIGNAL(clicked(bool)), this, SLOT(ok()));
    connect(lpCancel, SIGNAL(clicked(bool)), this, SLOT(cancel()));
	SetStyle();
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
		//发送登陆命令后，等待1秒
		//这里是简化线程间的同步问题，有点偷懒
		Sleep(1000);
		if (!c->GetControl()->IsLogIn()) {
			lpHint->setText("账户错误");
			return;
		}
		//如果登陆成功则要求服务器发送根目录文件信息
		c->PushLsCommand(std::string("0"), std::string(""), std::string("/"), std::vector<std::string>());
		accept();
	}
	else {
		lpHint->setText("网络断开");
	}
}

void MyLogInDialog::SetStyle()
{
	QPalette bgpal = palette();
	bgpal.setColor(QPalette::Background, QColor(57, 63, 63));
	setPalette(bgpal);

	setMinimumWidth(350);
	setMinimumHeight(160);
	setWindowFlags(Qt::FramelessWindowHint);
	setStyleSheet("QPushButton{background-color: rgb(80, 80, 80);"
		"color: rgb(217, 217, 217);"
		"border-style: outset;"
		"border-width: 0px;"
		"border-color: beige;"
		"font: bold 14px;"
		"padding: 6px;"
		"text-align: center;}"
		"QPushButton:hover{background-color: rgb(100, 100, 100)}"
		"QLabel{background-color: rgb(57, 63, 63);"
		"color: rgb(217, 217, 217);"
		"border-style: inset;"
		"border-width: 0px;"
		"border-color: rgb(45, 45, 45);"
		"font: bold 14px;"
		"padding: 6px;}"
		"QLineEdit{background-color: rgb(51, 51, 51);"
		"color: rgb(217, 217, 217);"
		"border-style: inset;"
		"border-width: 1px;"
		"border-color: rgb(45, 45, 45);"
		"font: bold 14px;"
		"padding: 6px;}");
}

void MyLogInDialog::cancel()
{
    close();
}

