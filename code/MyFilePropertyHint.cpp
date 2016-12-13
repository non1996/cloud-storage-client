# pragma execution_character_set("utf-8")
#include "MyFilePropertyHint.h"
#include <qbitmap.h>
#include <qpainter.h>
MyFilePropertyHint::MyFilePropertyHint(QWidget * parent)
	:QDialog(parent)
{
	lpType = new QLabel(this);
	lpName = new QLabel("Name: ", this);
	lpPath = new QLabel("Path: ", this);
	lpDate = new QLabel("Date: ", this);
	lpSize = new QLabel("Size: ", this);
	lpClose = new QPushButton("close", this);
	lpClose->setMinimumHeight(40);
	lpMainLayout = new QVBoxLayout(this);
	lpMainLayout->setSpacing(3);

	lpMainLayout->addWidget(lpType);
	lpMainLayout->addWidget(lpName);
	lpMainLayout->addWidget(lpPath);
	lpMainLayout->addWidget(lpDate);
	lpMainLayout->addWidget(lpSize);
	lpMainLayout->addWidget(lpClose);
	setLayout(lpMainLayout);

	setGeometry(300, 200, 200, 300);

	connect(lpClose, SIGNAL(clicked(bool)), this, SLOT(hide()));

	setWindowFlags(Qt::FramelessWindowHint);
	setStyleSheet("QDialog{background-color:rgb(57, 63, 63);"
		"border-style:outset;"
		"border-width:1px;"
		"border-color:rbg(60, 60, 60);}"
		"QWidget{color:rgb(217, 217, 217);"
		"font: bold 14px;}"
		"QPushButton{background-color:rgb(57, 63, 63);"
		"border-style: outset;"
		"border-width:0px;}"
		"QPushButton::hover{background-color:rgb(70, 75, 75)}"
		"QLabel{border: 3px;"
		"background-color:rgb(57, 63, 63);"
		"border-radius: 10px;"
		"border-color: rgb(80, 80, 80);}");
}

MyFilePropertyHint::~MyFilePropertyHint()
{
}

void MyFilePropertyHint::paintEvent(QPaintEvent * event)
{
	QBitmap bitmap(size());
	QPainter painter(&bitmap);

	painter.fillRect(rect(), Qt::white);
	painter.setBrush(QColor(0, 0, 0));
	painter.drawRoundedRect(this->rect(), 10, 10);
	setMask(bitmap);
}

void MyFilePropertyHint::mousePressEvent(QMouseEvent *e) {
	if (e->y() <= 22) {
		isMoving = true;
		last = e->globalPos();
	}
}

void MyFilePropertyHint::mouseMoveEvent(QMouseEvent *e) {
	if (isMoving == true) {
		int dx = e->globalX() - last.x();
		int dy = e->globalY() - last.y();
		last = e->globalPos();
		move(x() + dx, y() + dy);
	}
}

void MyFilePropertyHint::mouseReleaseEvent(QMouseEvent *e) {
	if (e->y() <= 30) {
		int dx = e->globalX() - last.x();
		int dy = e->globalY() - last.y();
		move(x() + dx, y() + dy);
		isMoving = false;
	}
}

void MyFilePropertyHint::SetText(QString & name, QString & path, QString & date, QString & size, QString & type)
{
	lpType->setText(type);
	QString temp = lpName->text();
	lpName->setText(temp.mid(0, 6) + name);
	temp = lpPath->text();
	lpPath->setText(temp.mid(0, 6) + path);
	temp = lpDate->text();
	lpDate->setText(temp.mid(0, 6) + date.split('T').front());
	temp = lpSize->text();
	lpSize->setText(temp.mid(0, 6) + size);
}
