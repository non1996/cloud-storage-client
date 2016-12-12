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
    lpUpload = new QPushButton("upload", this);
    lpDownload = new QPushButton("download", this);
    lpShare = new QPushButton("share", this);
    lpDelete = new QPushButton("delete", this);
    lpNewDir = new QPushButton("new director", this);

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
    lpPal->setColor(QPalette::Background, QColor(236, 242, 247));
    setPalette(*lpPal);
    setMinimumSize(850, 40);
}

void MyFBToolBar::setButtonStyle()
{
    setStyleSheet( "QPushButton{background-color: rgb(236, 242, 247);"
                   "color: rgb(100, 100, 100);"
                   "border-style: outset;"
                   "border-width: 0px;"
                   "border-color: beige;"
                   "font: bold 14px;"
                   "padding: 6px;"
                   "text-align: center;}"
                   "QPushButton:hover{background-color: rgb(223, 238, 242)}");
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
