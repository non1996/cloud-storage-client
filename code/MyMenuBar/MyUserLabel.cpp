# pragma execution_character_set("utf-8")
#include "MyUserLabel.h"
/*
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
*/
MyUserLabel::MyUserLabel(QWidget *parent) : QWidget(parent)
{
    InitWidget();
    InitLayout();
    SetWidgetStyle();
}

void MyUserLabel::SetInfo(QString name, QString url, unsigned long long cv, unsigned long long tv)
{
	lpUsername->setText(name);
	lpNetCapacity->setRange(0, tv/4096);
	lpNetCapacity->setValue(cv/4096);
	QPixmap* pixmap = new QPixmap(100, 100);
	pixmap->load(url);
	QPixmap scaledPixmap = pixmap->scaled(QSize(80, 80), Qt::KeepAspectRatio);
	lpUserProfilePic->setPixmap(scaledPixmap);
	lpVInfo->setText(GetV(cv) + "/" + GetV(tv));
}

void MyUserLabel::SetCapasity(unsigned long long c)
{
	QString temp = lpVInfo->text();
	int pos = temp.indexOf('/');
	QString cc = temp.mid(pos, temp.size()-pos);
	lpNetCapacity->setValue(c/4096);
	lpVInfo->setText(GetV(c) + cc);
}

void MyUserLabel::InitWidget(){
    lpUserProfilePic = new QLabel(this);
	lpUserProfilePic->setScaledContents(true);
    QPixmap* pixmap = new QPixmap(100, 100);
    pixmap->load("temp/default.jpg");
	QPixmap scaledPixmap = pixmap->scaled(QSize(80, 80), Qt::KeepAspectRatio);

    lpUserProfilePic->setPixmap(scaledPixmap);

    lpUsername = new QLabel(this);
    lpUsername->setText("non1996");
	lpVInfo = new QLabel("0GB/0GB", this);
    lpNetCapacity = new QProgressBar(this);
    lpNetCapacity->setRange(0, 100);
    lpNetCapacity->setValue(20);
}

void MyUserLabel::InitLayout(){
    lpHLayout = new QHBoxLayout(this);
    lpVLayout = new QVBoxLayout(this);

    lpVLayout->addWidget(lpUsername);
	lpVLayout->addWidget(lpVInfo);
	lpVLayout->addWidget(lpNetCapacity);
	
    lpHLayout->addWidget(lpUserProfilePic);
    lpHLayout->addLayout(lpVLayout);

    setLayout(lpHLayout);
}

void MyUserLabel::SetWidgetStyle(){
    lpUserProfilePic->setMinimumSize(100, 100);
	lpUserProfilePic->setStyleSheet("QLabel{border: 3px;"
									"background-color:rgb(100, 100, 100);"
									"border-radius: 10px;"
									"border-color: rgb(60, 60, 60)}");
	lpVInfo->setMinimumSize(100, 20);
	lpVInfo->setStyleSheet("QLabel{color: rgb(217, 217, 217);font: bold 14px;}");
	lpVInfo->setAlignment(Qt::AlignCenter);

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
QString MyUserLabel::GetV(unsigned long long v)
{
	if (v < 1024) {
		return QString("%1B").arg(v);
	}
	else if (v < 1048576) {
		return QString("%1KB").arg(v / 1024);
	}
	else if (v < 1073741824) {
		return QString("%1MB").arg(v / 1048576);
	}
	else if (v < 1099511627776) {
		return QString("%1GB").arg(v / 1073741824);
	}
	else {
		return QString("%1TB").arg(v / 1099511627776);
	}
}
