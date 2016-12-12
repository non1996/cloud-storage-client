#include "MyUserLabel.h"

MyUserLabel::MyUserLabel(QWidget *parent) : QWidget(parent)
{
    InitWidget();
    InitLayout();
    SetWidgetStyle();
}

void MyUserLabel::InitWidget(){
    lpUserProfilePic = new QLabel(this);
    QPixmap* pixmap = new QPixmap(100, 100);
    pixmap->load("image/menu/test.png");
    lpUserProfilePic->setPixmap(*pixmap);

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
    lpUserProfilePic->setFixedSize(100, 100);
    lpUserProfilePic->setScaledContents(true);
    lpUsername->setMinimumSize(100, 20);
    lpNetCapacity->setFixedSize(200, 20);
    lpUsername->setAlignment(Qt::AlignCenter);
    lpUsername->setStyleSheet("QLabel{color: rgb(100, 100, 100);}");
    lpNetCapacity->setStyleSheet("QProgressBar{border:1px solid #DDDDDD;"
                                "height:30;"
                                "background: rgb(240, 240, 240);"
                                "text-align:center;"
                                "color:rgb(100, 100, 100);"
                                "border-radius:5px;}"
                                "QProgressBar::chunk{"
                                "border-top-left-radius: 5px;"
                                "border-bottom-left-radius: 5px;"
                                "border-top-right-radius: 5px;"
                                "border-bottom-right-radius: 5px;"
                                "background-color:rgb(23, 210, 29);}");
}
