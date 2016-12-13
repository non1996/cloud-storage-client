# pragma execution_character_set("utf-8")
#include "MyDirInfoBar.h"

MyDirInfoBar::MyDirInfoBar(QWidget *parent) : QWidget(parent)
{
    InitWidget();
    InitLayout();
    SetThisStyle();
    SetWidgetStyle();
    ConnectSlot();
}

void MyDirInfoBar::SetPath(QString & path)
{
	lpPath->setText(path);
}

void MyDirInfoBar::InitWidget()
{
    lpBack = new QPushButton("<", this);
    lpFront = new QPushButton(">", this);
    lpFresh = new QPushButton("刷新", this);

//    lpPath = new QLabel("home>", this);
	lpPath = new QLabel(this);
    lpInputSearch = new QLineEdit(this);
    lpSearch = new QPushButton("搜索", this);
}

void MyDirInfoBar::InitLayout()
{
    lpMainLayout = new QHBoxLayout(this);
    lpMainLayout->setMargin(0);


    lpMainLayout->addWidget(lpBack);
    lpMainLayout->addWidget(lpFront);
    lpMainLayout->addWidget(lpFresh);
    lpMainLayout->addWidget(lpPath);
    lpMainLayout->addWidget(lpInputSearch);
    lpMainLayout->addWidget(lpSearch);

    setLayout(lpMainLayout);
}

void MyDirInfoBar::SetThisStyle()
{
    setAutoFillBackground(true);
    setMinimumSize(950, 40);

    lpPal = new QPalette();
    lpPal->setColor(QPalette::Background, QColor(64, 64, 64));
    setPalette(*lpPal);
}

void MyDirInfoBar::SetWidgetStyle()
{
    lpBack->setMinimumSize(40, 40);
    lpFront->setMinimumSize(40, 40);
    lpFresh->setMinimumSize(40, 40);
    lpPath->setFixedSize(500, 30);
    lpInputSearch->setMinimumSize(100, 30);
    lpSearch->setMinimumSize(40, 40);
    setStyleSheet( "QPushButton{background-color: rgb(64, 64, 64);"
                   "color: rgb(217, 217, 217);"
                   "border-style: outset;"
                   "border-width: 0px;"
                   "border-color: beige;"
                   "font: bold 14px;"
                   "padding: 6px;"
                   "text-align: center;}"
                   "QPushButton:hover{background-color: rgb(80, 80, 80)}");
    lpPath->setStyleSheet("QLabel{background-color: rgb(51, 51, 51);"
                          "color: rgb(217, 217, 217);"
                          "border-style: inset;"
                          "border-width: 1px;"
                          "border-color: rgb(45, 45, 45);"
                          "font: bold 14px;"
                          "padding: 6px;}");
    lpInputSearch->setStyleSheet("QLineEdit{background-color: rgb(51, 51, 51);"
                                 "color: rgb(217, 217, 217);"
                                 "border-style: inset;"
                                 "border-width: 1px;"
                                 "border-color: rgb(45, 45, 45);"
                                 "font: bold 14px;"
                                 "padding: 6px;}");
    lpInputSearch->setPlaceholderText("输入文件(夹)名");
}

void MyDirInfoBar::ConnectSlot()
{
    connect(lpBack, SIGNAL(clicked(bool)), this, SLOT(sendBack()));
    connect(lpFront, SIGNAL(clicked(bool)), this, SLOT(sendFront()));
    connect(lpFresh, SIGNAL(clicked(bool)), this, SLOT(sendFresh()));
    connect(lpSearch, SIGNAL(clicked(bool)), this, SLOT(sendSearch()));
}

void MyDirInfoBar::sendBack()
{
    emit Back();
}

void MyDirInfoBar::sendFront()
{
    emit Front();
}

void MyDirInfoBar::sendFresh()
{
    emit Fresh();
}

void MyDirInfoBar::sendSearch()
{
    if(!lpInputSearch->text().isEmpty()){
        emit Search(lpInputSearch->text());
    }
}



