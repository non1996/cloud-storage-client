# pragma execution_character_set("utf-8")
#include "MyMenuBar.h"
#include "MyToolButton.h"
#include "MyUserLabel.h"
#include <QDebug>

MyMenuBar::MyMenuBar(QWidget *parent) : QWidget(parent)
{
    createWidget();
    createLayout();
    setSlotConnect();
}

void MyMenuBar::SetCapasity(unsigned long long c)
{
	lpUserInfo->SetCapasity(c);
}

void MyMenuBar::createWidget()
{
    lpUserInfo = new MyUserLabel(this);

    lpMainWindowButton = new MyToolButton("image/menu/cloud.png", "myCloud", this);
    lpMainWindowButton->setMinimumSize(120, 116);
    lpMainWindowButton->SetPress(true);
    lpDownloadButton = new MyToolButton("image/menu/download.png", "download", this);
    lpDownloadButton->setMinimumSize(120, 116);
    lpUploadButton = new MyToolButton("image/menu/upload.png", "upload", this);
    lpUploadButton->setMinimumSize(120, 116);
	
	lpMainWindowButton->setToolTip("文件浏览页面");
	lpDownloadButton->setToolTip("下载管理页面");
	lpUploadButton->setToolTip("上传管理页面");
}

void MyMenuBar::createLayout()
{
    lpSwitchLayout = new QHBoxLayout(this);
    lpSwitchLayout->setContentsMargins(5, 0, 0, 0);
    lpSwitchLayout->setSpacing(0);

    lpSwitchLayout->addWidget(lpUserInfo);
    lpSwitchLayout->addStretch();
    lpSwitchLayout->addWidget(lpMainWindowButton);
    lpSwitchLayout->addWidget(lpDownloadButton);
    lpSwitchLayout->addWidget(lpUploadButton);
    this->setLayout(lpSwitchLayout);
}

void MyMenuBar::setMenuBarStyle()
{

}

void MyMenuBar::setSlotConnect()
{
    connect(lpMainWindowButton, SIGNAL(SignalParent(MyToolButton*)),
            this, SLOT(ResponseSlot(MyToolButton*)));
    connect(lpDownloadButton, SIGNAL(SignalParent(MyToolButton*)),
            this, SLOT(ResponseSlot(MyToolButton*)));
    connect(lpUploadButton, SIGNAL(SignalParent(MyToolButton*)),
            this, SLOT(ResponseSlot(MyToolButton*)));
}

void MyMenuBar::SetUserInfo(QString name, QString url, unsigned long long cv, unsigned long long tv)
{
	lpUserInfo->SetInfo(name, url, cv, tv);
}

void MyMenuBar::ResponseSlot(MyToolButton *b)
{
    if(b == lpMainWindowButton){
        lpDownloadButton->SetPress(false);
        lpDownloadButton->update();
        lpUploadButton->SetPress(false);
        lpUploadButton->update();
        emit SwitchPage(0);
    }
    else if(b == lpDownloadButton){
        lpMainWindowButton->SetPress(false);
        lpMainWindowButton->update();
        lpUploadButton->SetPress(false);
        lpUploadButton->update();
        emit SwitchPage(1);
    }
    else if(b == lpUploadButton){
        lpMainWindowButton->SetPress(false);
        lpMainWindowButton->update();
        lpDownloadButton->SetPress(false);
        lpDownloadButton->update();
        emit SwitchPage(2);
    }
}
