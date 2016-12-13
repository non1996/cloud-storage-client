# pragma execution_character_set("utf-8")
#include "MyMToolBar.h"

MyMToolBar::MyMToolBar(QWidget *parent) : QWidget(parent)
{
    isRun = true;
    createWidget();
    createLayout();
    setToolBarStyle();
    setButtonStyle();
    ConnectSlot();
}

void MyMToolBar::createWidget()
{
    lpTransmitSpeed = new QLabel("速度: 0B/s");
    lpTransmitSpeed->setMinimumWidth(200);
    lpTransmitSpeed->setAlignment(Qt::AlignCenter);

    lpResumeOrSuspendAll = new QPushButton("全部暂停", this);
	lpResumeOrSuspendAll->setMinimumSize(100, 40);
    lpCancelAll = new QPushButton("全部取消", this);
	lpCancelAll->setMinimumSize(100, 40);
}

void MyMToolBar::createLayout()
{
    lpLayout = new QHBoxLayout(this);

    lpLayout->setMargin(5);
    lpLayout->setSpacing(0);

    lpLayout->addWidget(lpTransmitSpeed);
    lpLayout->addStretch();
    lpLayout->addWidget(lpResumeOrSuspendAll);
    lpLayout->addWidget(lpCancelAll);
    this->setLayout(lpLayout);
}

void MyMToolBar::setToolBarStyle()
{
    setAutoFillBackground(true);
    lpPal = new QPalette();
    lpPal->setColor(QPalette::Background, QColor(64, 64, 64));
    setPalette(*lpPal);
    setMinimumSize(850, 40);
}

void MyMToolBar::setButtonStyle()
{
    setStyleSheet( "QPushButton{background-color: rgb(64, 64, 64);"
                   "color: rgb(217, 217, 217);"
                   "border-style: outset;"
                   "border-width: 0px;"
                   "border-color: beige;"
                   "font: bold 14px;"
                   "padding: 6px;"
                   "text-align: center;}"
                   "QPushButton:hover{background-color: rgb(80, 80, 80)}"
                   "QLabel{background-color: rgb(64, 64, 64);"
                   "color:rgb(217, 217, 217);}");
}

void MyMToolBar::ConnectSlot()
{
    connect(lpResumeOrSuspendAll, SIGNAL(clicked(bool)), this, SLOT(sendRS()));
    connect(lpCancelAll, SIGNAL(clicked(bool)), this, SLOT(sendC()));
}

void MyMToolBar::SetSpeed(float speed)
{
    QString hint = "速度: ";
    if(speed > 1048576){
        speed /= 1048576;
        hint += QString("%1MB/s").arg(speed);
    }else if(speed > 1024){
        speed /= 1024;
        hint += QString("%1KB/s").arg(speed);
    }else{
        hint += QString("%1B/s").arg(speed);
    }
    lpTransmitSpeed->setText(hint);
}

void MyMToolBar::sendRS()
{
    if(isRun){
        isRun = false;
        lpResumeOrSuspendAll->setText("resume all");
        emit SuspendAll();
    }else{
        isRun = true;
        lpResumeOrSuspendAll->setText("suspend all");
        emit ResumeAll();
    }
}

void MyMToolBar::sendC()
{
    emit CancelAll();
}
