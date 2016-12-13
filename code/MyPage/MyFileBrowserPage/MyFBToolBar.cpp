# pragma execution_character_set("utf-8")
#include "MyFBToolBar.h"
#include <QBitmap>
#include <QPainter>
#include <QDebug>
MyFBToolBar::MyFBToolBar(QWidget *parent) : QWidget(parent)
{
    createWidget();
    createLayout();
    setToolBarStyle();
    setButtonStyle();
    setSlotConnect();
}

void MyFBToolBar::setSlotConnect()
{
    connect(lpDownload, SIGNAL(clicked(bool)), this, SLOT(clickedDownload()));
    connect(lpUpload, SIGNAL(clicked(bool)), this, SLOT(clickedUpload()));
    connect(lpShare, SIGNAL(clicked(bool)), this, SLOT(clickedShare()));
    connect(lpDelete, SIGNAL(clicked(bool)), this, SLOT(clickedDelete()));
    connect(lpNewDir, SIGNAL(clicked(bool)), this, SLOT(clickedNewDir()));
}

void MyFBToolBar::createWidget()
{
    lpUpload = new QPushButton("上传", this);
    lpDownload = new QPushButton("下载", this);
    lpShare = new QPushButton("分享", this);
    lpDelete = new QPushButton("删除", this);
    lpNewDir = new QPushButton("新建文件夹", this);

    lpUpload->setFixedSize(100, 40);
    lpDownload->setFixedSize(100, 40);
    lpShare->setFixedSize(100, 40);
    lpDelete->setFixedSize(100, 40);
    lpNewDir->setFixedSize(100, 40);
}

void MyFBToolBar::createLayout()
{
    lpLayout = new QHBoxLayout(this);

    lpLayout->setMargin(0);
    lpLayout->setSpacing(0);

    lpLayout->addWidget(lpUpload);
    lpLayout->addWidget(lpDownload);
    lpLayout->addWidget(lpShare);
    lpLayout->addWidget(lpDelete);
    lpLayout->addWidget(lpNewDir);
    lpLayout->addStretch();
    this->setLayout(lpLayout);
}

void MyFBToolBar::setToolBarStyle()
{
    setAutoFillBackground(true);
    lpPal = new QPalette();
    lpPal->setColor(QPalette::Background, QColor(64, 64, 64));
    setPalette(*lpPal);
    setMinimumSize(850, 40);
}

void MyFBToolBar::setButtonStyle()
{
    setStyleSheet( "QPushButton{background-color: rgb(64, 64, 64);"
                   "color: rgb(217, 217, 217);"
                   "border-style: outset;"
                   "border-width: 0px;"
                   "border-color: rgb(80, 80, 80);"
                   "font: bold 14px;"
                   "padding: 6px;"
                   "text-align: center;}"
                   "QPushButton:hover{background-color: rgb(80, 80, 80)}");
}

void MyFBToolBar::clickedUpload(){
    emit Upload();
}

void MyFBToolBar::clickedDownload(){
    emit Download();
}

void MyFBToolBar::clickedShare(){
    emit Share();
}

void MyFBToolBar::clickedDelete(){
    emit Delete();
}

void MyFBToolBar::clickedNewDir(){
    emit NewDir();
}
