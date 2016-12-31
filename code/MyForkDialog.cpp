# pragma execution_character_set("utf-8")
#include "MyForkDialog.h"

#include <QBitmap>
#include <QPainter>

MyForkDialog::MyForkDialog(QWidget *parent)
	:QDialog(parent)
{
	InitWidget();
	InitLayout();
	SetThisStyle();
	ConnectSlot();
}

QString MyForkDialog::GetID()
{
	return lpUIDInput->text();
}

QString MyForkDialog::GetPass()
{
	return lpPassInput->text();
}

void MyForkDialog::paintEvent(QPaintEvent *event)
{
	QBitmap bitmap(size());
	QPainter painter(&bitmap);

	painter.fillRect(rect(), Qt::white);
	painter.setBrush(QColor(0, 0, 0));
	painter.drawRoundedRect(this->rect(), 10, 10);
	setMask(bitmap);
}

void MyForkDialog::mousePressEvent(QMouseEvent *e)
{
	last = e->globalPos();
}

void MyForkDialog::mouseMoveEvent(QMouseEvent *e)
{
	int dx = e->globalX() - last.x();
	int dy = e->globalY() - last.y();
	last = e->globalPos();
	move(x() + dx, y() + dy);
}

void MyForkDialog::mouseReleaseEvent(QMouseEvent *e)
{
	int dx = e->globalX() - last.x();
	int dy = e->globalY() - last.y();
	move(x() + dx, y() + dy);
	isMoving = false;
}

void MyForkDialog::InitWidget() {
	lpOK = new QPushButton("确定", this);
	lpOK->setMinimumSize(30, 20);
	lpCancel = new QPushButton("取消", this);
	lpCancel->setMinimumSize(30, 20);
	lpUIDInput = new QLineEdit(this);
	lpUIDInput->setMinimumHeight(35);
	lpPassInput = new QLineEdit(this);
	lpPassInput->setMinimumHeight(35);
	lpHint1 = new QLabel("文件ID", this);
	lpHint2 = new QLabel("提取码", this);
	lpFalse = new QLabel(this);
}

void MyForkDialog::InitLayout()
{
	lpMainLayout = new QVBoxLayout(this);
	lpSubLayout = new QHBoxLayout(this);

	lpMainLayout->addWidget(lpHint1);
	lpMainLayout->addWidget(lpUIDInput);
	lpMainLayout->addWidget(lpHint2);
	lpMainLayout->addWidget(lpPassInput);
	lpSubLayout->setMargin(0);
	lpSubLayout->addWidget(lpFalse);
	lpSubLayout->addStretch();
	lpSubLayout->addWidget(lpOK);
	lpSubLayout->addWidget(lpCancel);

	lpMainLayout->addLayout(lpSubLayout);
	setLayout(lpMainLayout);
}

void MyForkDialog::SetThisStyle()
{
	setStyleSheet("QDialog{background-color:rgb(64, 64, 64);"
		"border-style: inset;"
		"border-width: 2px;"
		"border-color: rgb(60, 60, 60)}"
		"QPushButton{background-color:rgb(64, 64, 64);"
		"border-style: outset;"
		"border-width: 0px;}"
		"QPushButton:hover{background-color: rgb(80, 80, 80)}"
		"QWidget{color:rgb(217, 217, 217); font: bold 14px;}"
		"QLineEdit{background-color: rgb(51, 51, 51);"
		"border-style: inset;"
		"border-width: 1px;"
		"border-color: rgb(45, 45, 45);"
		"padding: 6px;}");
	setGeometry(100, 100, 300, 200);
	setWindowFlags(Qt::FramelessWindowHint);
	hide();
}

void MyForkDialog::ConnectSlot()
{
	connect(lpOK, SIGNAL(clicked(bool)), this, SLOT(pressOk()));
	connect(lpCancel, SIGNAL(clicked(bool)), this, SLOT(close()));
}

void MyForkDialog::pressOk()
{
	if (lpUIDInput->text().isEmpty()) {
		lpFalse->setText("请输入UID");
		return;
	}
	if (lpPassInput->text().lastIndexOf('+') != -1) {
		lpFalse->setText("请输入提取码");
		return;
	}
	accept();
}