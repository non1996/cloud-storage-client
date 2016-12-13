# pragma execution_character_set("utf-8")
#include "MyNewDirDialog.h"

#include <QBitmap>
#include <QPainter>

MyNewDirDialog::MyNewDirDialog(QWidget *parent)
    :QDialog(parent)
{
    InitWidget();
    InitLayout();
    SetThisStyle();
    ConnectSlot();
}

void MyNewDirDialog::Clean()
{
    lpNameInput->clear();
    setGeometry(100, 100, 300, 100);
}

void MyNewDirDialog::paintEvent(QPaintEvent *event)
{
    QBitmap bitmap(size());
    QPainter painter(&bitmap);

    painter.fillRect(rect(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(this->rect(), 10, 10);
    setMask(bitmap);
}

void MyNewDirDialog::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
}

void MyNewDirDialog::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx,y()+dy);
}

void MyNewDirDialog::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
    isMoving = false;
}

void MyNewDirDialog::InitWidget(){
    lpOK = new QPushButton("确定", this);
    lpOK->setMinimumSize(30, 20);
    lpCancel = new QPushButton("取消", this);
    lpCancel->setMinimumSize(30, 20);
    lpNameInput = new QLineEdit(this);
    lpHint = new QLabel("输入文件夹名", this);
    lpFalse = new QLabel(this);
}

void MyNewDirDialog::InitLayout()
{
    lpMainLayout = new QVBoxLayout(this);
    lpSubLayout = new QHBoxLayout(this);

    lpMainLayout->addWidget(lpHint);
    lpMainLayout->addWidget(lpNameInput);
//    lpMainLayout->addWidget(lpFalse);
    lpSubLayout->setMargin(0);
    lpSubLayout->addWidget(lpFalse);
    lpSubLayout->addStretch();
    lpSubLayout->addWidget(lpOK);
    lpSubLayout->addWidget(lpCancel);

    lpMainLayout->addLayout(lpSubLayout);
    setLayout(lpMainLayout);
}

void MyNewDirDialog::SetThisStyle()
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
    setGeometry(100, 100, 300, 100);
    setWindowFlags(Qt::FramelessWindowHint);
    hide();
}

void MyNewDirDialog::ConnectSlot()
{
    connect(lpOK, SIGNAL(clicked(bool)), this, SLOT(pressOk()));
    connect(lpCancel, SIGNAL(clicked(bool)), this, SLOT(close()));
}

void MyNewDirDialog::pressOk()
{
    if(lpNameInput->text().isEmpty()){
        lpFalse->setText("name should not be empty");
        return;
    }
    if(lpNameInput->text().lastIndexOf('+') != -1){
        lpFalse->setText("'+' is not valid");
        return;
    }
    accept();
}

