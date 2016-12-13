# pragma execution_character_set("utf-8")
#include "MyMissionInfoBar.h"

MyMissionInfoBar::MyMissionInfoBar(QWidget *parent) : QWidget(parent)
{
    InitWidgetDefault();
    InitInfo();
    InitLayout();
    SetThisStyle();
    SetWidgetStyle();
    ConnectSlot();
}

void MyMissionInfoBar::SetName(QString name)
{
    lpFileName->setText(name);
}

void MyMissionInfoBar::SetProgress(float progress)
{
    lpProgress->setValue(100 * progress);
}

void MyMissionInfoBar::SetSpeed(unsigned int i)
{
	QString temp;
	if (i < 1024) {
		temp = QString("%1B/s").arg(i);
	}
	else if (i < 1048576) {
		temp = QString("%1KB/s").arg(i / 1024);
	}
	else if (i < 1073741824) {
		temp = QString("%1MB/s").arg(i / 1048576);
	}
	lpSpeed->setText(temp);
}

void MyMissionInfoBar::SetFinish()
{
	lpSpeed->setText("完成");
	lpProgress->setValue(100);
}

void MyMissionInfoBar::SetRunning(bool r)
{
    isRunning = r;
    if(isRunning){
        lpResumeOrSuspend->setText("暂停");
    }else{
        lpResumeOrSuspend->setText("继续");
    }
}

void MyMissionInfoBar::SetNum(int n)
{
    num = n;
}

void MyMissionInfoBar::ConnectSlot()
{
    connect(lpResumeOrSuspend, SIGNAL(clicked(bool)), this, SLOT(pressedSuspendOrResume()));
    connect(lpCancel, SIGNAL(clicked(bool)), this, SLOT(pressedCancel()));
    connect(lpDir, SIGNAL(clicked(bool)), this, SLOT(pressedOpenDir()));
}

void MyMissionInfoBar::InitWidgetDefault()
{
    lpFileIcon = new QLabel(this);
    lpFileName = new QLabel("default", this);
	lpSpeed = new QLabel(this);
    lpProgress = new QProgressBar(this);
    lpResumeOrSuspend = new QPushButton("暂停", this);
    lpCancel = new QPushButton("取消", this);
    lpDir = new QPushButton("打开文件夹", this);
}

void MyMissionInfoBar::InitInfo()
{
    isRunning = true;
//    currentSize = 0;
//    maxSize = 0;
//    progress = 0;
}

void MyMissionInfoBar::InitLayout()
{
    lpMainLayout = new QHBoxLayout(this);
    lpMainLayout->setMargin(0);
    lpMainLayout->setSpacing(10);

    lpMainLayout->addWidget(lpFileIcon);
    lpMainLayout->addWidget(lpFileName);
	lpMainLayout->addWidget(lpSpeed);
    lpMainLayout->addStretch();
    lpMainLayout->addWidget(lpProgress);
    lpMainLayout->addWidget(lpResumeOrSuspend);
    lpMainLayout->addWidget(lpCancel);
    lpMainLayout->addWidget(lpDir);
	lpMainLayout->addSpacing(5);
    setLayout(lpMainLayout);
}

void MyMissionInfoBar::SetThisStyle()
{
    setMinimumSize(920, 80);
}

void MyMissionInfoBar::SetWidgetStyle()
{
    lpFileIcon->setFixedSize(35, 35);
    lpFileName->setMinimumSize(100, 35);
    lpFileName->setAlignment(Qt::AlignCenter);
	lpSpeed->setMinimumSize(80, 35);
    lpResumeOrSuspend->setFixedSize(35, 35);
    lpCancel->setFixedSize(35, 35);
    lpDir->setFixedSize(100, 35);
    lpProgress->setRange(0, 100);
    lpProgress->setValue(0);
	lpSpeed->setStyleSheet("QLabel{color:rgb(217, 217, 217);font: bold 14px;}");
    lpFileName->setStyleSheet("QLabel{color:rgb(217, 217, 217);font: bold 14px;}");
    lpProgress->setStyleSheet("QProgressBar{border:1px solid #050505;"
                              "height:30;"
                              "border-color: rgb(55, 55, 55);"
                              "text-align:center;"
                              "color:rgb(217, 217, 217);"
							  "font: bold 14px;"
                              "border-radius:10px;}"
                              "QProgressBar::chunk{"
                              "border-top-left-radius: 10px;"
                              "border-bottom-left-radius: 10px;"
                              "border-top-right-radius: 10px;"
                              "border-bottom-right-radius: 10px;"
							  "background-color:rgb(80, 80, 80);}");
	setStyleSheet("QPushButton{background-color: rgb(64, 64, 64);"
				  "border-style: inset;"
                  "border: 1px solid #050505;"
                  "color: rgb(217, 217, 217);}"
                  "QPushButton:hover{background-color:rgb(80, 80, 80);}");
}

void MyMissionInfoBar::pressedSuspendOrResume()
{
    if(isRunning){
        emit Suspend(num);
        return;
    }
    emit Resume(num);
}

void MyMissionInfoBar::pressedCancel()
{
    emit Cancel(num);
}

void MyMissionInfoBar::pressedOpenDir()
{
    emit OpenDir(num);
}

