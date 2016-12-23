# pragma execution_character_set("utf-8")
#include "MyMessageDialog.h"
#include "MyMessageItem.h"
#include "MyChatBar.h"

MyMessageDialog::MyMessageDialog(QWidget *parent)
    :QDialog(parent)
{
    lpMessage = new QListWidget(this);
    lpCID = new QLabel("êÇ³Æ", this);
    lpTextEdit = new QTextEdit(this);
    lpCIDEdit = new QLineEdit(this);
    lpHide = new QPushButton(this);
    lpOk = new QPushButton("send", this);
	lpHint = new QLabel(this);

	QPixmap *pixmap = new QPixmap(22, 22);
	pixmap->load("image/chat/min.png");
	QIcon *icon = new QIcon(*pixmap);
	lpHide->setIcon(*icon);
	lpHide->setIconSize(QSize(22, 22));

    lpCID->setMinimumSize(50, 20);
    lpCIDEdit->setMinimumSize(50, 40);
    lpTextEdit->setMinimumSize(300, 80);
    lpMessage->setMinimumSize(300, 380);
    lpOk->setMinimumSize(50, 30);
    lpHide->setMinimumSize(30, 30);
	lpHint->setMinimumSize(250, 30);
    setMinimumSize(330, 600);
    setWindowFlags(Qt::FramelessWindowHint);

    lpMainLayout = new QVBoxLayout(this);

    QHBoxLayout* lpSub0 = new QHBoxLayout(this);
    QHBoxLayout* lpSub1 = new QHBoxLayout(this);
    QHBoxLayout* lpSub2 = new QHBoxLayout(this);
    lpSub0->addStretch();
    lpSub0->addWidget(lpHide);

    lpSub1->addWidget(lpCID);
    lpSub1->addWidget(lpCIDEdit);
    lpSub1->addStretch();

	lpSub2->addWidget(lpHint);
    lpSub2->addStretch();
    lpSub2->addWidget(lpOk);

    lpMainLayout->addLayout(lpSub0);
    lpMainLayout->addWidget(lpMessage);
    lpMainLayout->addLayout(lpSub1);
    lpMainLayout->addWidget(lpTextEdit);
    lpMainLayout->addLayout(lpSub2);
    setLayout(lpMainLayout);

    connect(lpOk, SIGNAL(clicked(bool)), this, SLOT(send()));
	connect(lpHide, SIGNAL(clicked(bool)), this, SLOT(hide()));

    SetStyle();
}

void MyMessageDialog::SetStyle()
{
    setStyleSheet(  "QDialog{background-color:rgb(64, 64, 64);}"
                    "QPushButton{background-color:rgb(64, 64, 64);"
                    "border-style: outset;"
                    "border-width: 0px;}"
                    "QPushButton:hover{background-color: rgb(80, 80, 80)}"
                    "QListWidget{background-color:rgb(51, 51, 51);"
                    "outline:0px;"
                    "color:rgb(217, 217, 217);"
                    "border-style: inset;"
                    "border-width: 2px;"
                    "border-color: rgb(45, 45, 45)}"
                    "QListWidget::item:hover{show-decoration-selected:0;background:rgb(80, 80, 80);}"
                    "QListWidget::item:selected{background:rgb(80, 80, 80);padding:0px;margin:0px;}"
                    "QLineEdit{background-color: rgb(51, 51, 51);"
                    "color: rgb(217, 217, 217);"
                    "border-style: inset;"
                    "border-width: 1px;"
                    "border-color: rgb(45, 45, 45);"
                    "padding: 6px;}"
                    "QTextEdit{background-color:rgb(51, 51, 51);"
                    "color:rgb(217, 217, 217);"
                    "border-style: inset;"
                    "border-width: 2px;"
                    "border-color: rgb(45, 45, 45)}"
                    "QWidget{color: rgb(217, 217, 217);"
                    "font: bold 14px;}");
}

void MyMessageDialog::AddRecvMessage(QString cID, QString message)
{
    QListWidgetItem* temp = new QListWidgetItem(lpMessage);
    MyMessageItem* item = new MyMessageItem(MyMessageItem::RECV, QString("from:" + cID), message, this);
    lpMessage->addItem(temp);
    lpMessage->setItemWidget(temp, item);
    temp->setSizeHint(QSize(item->rect().width(), item->rect().height()));
    lpMessage->setCurrentRow(lpMessage->count() - 1);
}

void MyMessageDialog::AddSendMessage(QString cID, QString message)
{
	if (message == "error") {
		lpHint->setText("´íÎóµÄêÇ³Æ");
		return;
	}
    QListWidgetItem* temp = new QListWidgetItem(lpMessage);
    MyMessageItem* item = new MyMessageItem(MyMessageItem::SEND, QString("to:" + cID), message, this);
    lpMessage->addItem(temp);
    lpMessage->setItemWidget(temp, item);
    temp->setSizeHint(QSize(item->rect().width(), item->rect().height()));
    lpMessage->setCurrentRow(lpMessage->count() - 1);
    lpTextEdit->clear();
}

void MyMessageDialog::send()
{
    if (lpCIDEdit->text().size() == 0){
		lpHint->setText("ÇëÊäÈëÓÃ»§êÇ³Æ");
        return ;
    }
	if (lpTextEdit->toPlainText().size() == 0) {
		lpHint->setText("ÊäÈë¿ò²»ÄÜÎª¿Õ");
	}
    emit SendMessage(lpCIDEdit->text(), lpTextEdit->toPlainText());
}
