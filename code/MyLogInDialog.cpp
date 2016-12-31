# pragma execution_character_set("utf-8")
#include "MyLogInDialog.h"
#include "MyBase\MyController.h"

MyLogInDialog::MyLogInDialog(QWidget* parent)
    :QDialog(parent)
{
    lpU = new QLabel("�û���", this);
    lpU->setMinimumWidth(40);
    lpP = new QLabel("����", this);
    lpP->setMinimumWidth(40);
	lpHint = new QLabel(this);
	lpHint->setMinimumHeight(10);

    lpUsername = new QLineEdit("non1996@forec.cn", this);
    lpUsername->setPlaceholderText("�����û���");
    lpPassword = new QLineEdit("zenith", this);
    lpPassword->setPlaceholderText("��������");
	lpPassword->setEchoMode(QLineEdit::Password);
	lpUsername->setMinimumWidth(280);
	lpPassword->setMinimumWidth(280);

    lpLogIn = new QPushButton("��¼", this);
    lpCancel = new QPushButton("ȡ��", this);

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
		//���͵�½����󣬵ȴ�1��
		//�����Ǽ��̼߳��ͬ�����⣬�е�͵��
		Sleep(1000);
		if (!c->GetControl()->IsLogIn()) {
			lpHint->setText("�˻�����");
			return;
		}
		//�����½�ɹ���Ҫ����������͸�Ŀ¼�ļ���Ϣ
		c->PushLsCommand(std::string("0"), std::string(""), std::string("/"), std::vector<std::string>());
		accept();
	}
	else {
		lpHint->setText("����Ͽ�");
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

