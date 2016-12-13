# pragma execution_character_set("utf-8")
#include "MyUserLabel.h"

MyUserLabel::MyUserLabel(QWidget *parent) : QWidget(parent)
{
    InitWidget();
    InitLayout();
    SetWidgetStyle();
}

void MyUserLabel::InitWidget(){
    lpUserProfilePic = new QLabel(this);
	lpUserProfilePic->setScaledContents(true);
    QPixmap* pixmap = new QPixmap(100, 100);
    pixmap->load("image/menu/test.png");
	QPixmap scaledPixmap = pixmap->scaled(QSize(80, 80), Qt::KeepAspectRatio);

    lpUserProfilePic->setPixmap(scaledPixmap);

    lpUsername = new QLabel(this);
    lpUsername->setText("non1996");

    lpNetCapacity = new QProgressBar(this);
    lpNetCapacity->setRange(0, 100);
    lpNetCapacity->setValue(20);
}

void MyUserLabel::InitLayout(){
    lpHLayout = new QHBoxLayout(this);
    lpVLayout = new QVBoxLayout(this);

    lpVLayout->addWidget(lpUsername);
    lpVLayout->addWidget(lpNetCapacity);

    lpHLayout->addWidget(lpUserProfilePic);
    lpHLayout->addLayout(lpVLayout);

    setLayout(lpHLayout);
}

void MyUserLabel::SetWidgetStyle(){
    lpUserProfilePic->setMinimumSize(100, 100);
    lpUserProfilePic->setScaledContents(true);
	lpUserProfilePic->setStyleSheet("QLabel{border: 3px;"
									"background-color:rgb(100, 100, 100);"
									"border-radius: 10px;"
									"border-color: rgb(60, 60, 60)}");

    lpUsername->setMinimumSize(100, 20);
    lpNetCapacity->setFixedSize(200, 20);
    lpUsername->setAlignment(Qt::AlignCenter);
    lpUsername->setStyleSheet("QLabel{color: rgb(217, 217, 217);font: bold 14px;}");
    lpNetCapacity->setStyleSheet("QProgressBar{border:1px solid #DDDDDD;"
								 "height:30;"
								 "background: rgb(43, 47, 47);"
                                 "text-align:center;"
                                 "color:rgb(217, 217, 217);"
                                 "border-radius:5px;"
								 "border-color: rgb(55, 55, 55);"
								 "font: bold 14px;}"
                                 "QProgressBar::chunk{"
                                 "border-top-left-radius: 5px;"
                                 "border-bottom-left-radius: 5px;"
                                 "border-top-right-radius: 5px;"
                                 "border-bottom-right-radius: 5px;"
                                 "background-color:rgb(80, 80, 80);}");
}
