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
    lpFresh = new QPushButton("refresh", this);

//    lpPath = new QLabel("home>", this);
	lpPath = new QLabel(this);
    lpInputSearch = new QLineEdit(this);
    lpSearch = new QPushButton("search", this);
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
    lpPal->setColor(QPalette::Background, QColor(230, 230, 230));
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
    setStyleSheet( "QPushButton{background-color: rgb(230, 230, 230);"
                   "color: rgb(100, 100, 100);"
                   "border-style: outset;"
                   "border-width: 0px;"
                   "border-color: beige;"
                   "font: bold 14px;"
                   "padding: 6px;"
                   "text-align: center;}"
                   "QPushButton:hover{background-color: rgb(236, 242, 247)}");
    lpPath->setStyleSheet("QLabel{background-color: rgb(255, 255, 255);"
                          "color: rgb(100, 100, 100);"
                          "border-style: outset;"
                          "border-width: 1px;"
                          "border-color: rgb(200, 200, 200);"
                          "font: bold 14px;"
                          "padding: 6px;}");
    lpInputSearch->setStyleSheet("QLineEdit{background-color: rgb(255, 255, 255);"
                                 "color: rgb(100, 100, 100);"
                                 "border-style: outset;"
                                 "border-width: 1px;"
                                 "border-color: rgb(200, 200, 200);"
                                 "font: bold 14px;"
                                 "padding: 6px;}");
    lpInputSearch->setPlaceholderText("file or director");
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



