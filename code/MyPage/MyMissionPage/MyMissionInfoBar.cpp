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
	lpSpeed->setText("complete");
	lpProgress->setValue(100);
}

void MyMissionInfoBar::SetRunning(bool r)
{
    isRunning = r;
    if(isRunning){
        lpResumeOrSuspend->setText("suspend");
    }else{
        lpResumeOrSuspend->setText("resume");
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
    lpResumeOrSuspend = new QPushButton("suspend", this);
    lpCancel = new QPushButton("cancel", this);
    lpDir = new QPushButton("open director", this);
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
    lpFileName->setStyleSheet("QLabel{color:rgb(100, 100, 100);}");
    lpProgress->setStyleSheet("QProgressBar{border:1px solid #DDDDDD;"
                              "height:30;"
                              "background: rgb(240, 240, 240);"
                              "text-align:center;"
                              "color:rgb(100, 100, 100);"
                              "border-radius:10px;}"
                              "QProgressBar::chunk{"
                              "border-top-left-radius: 10px;"
                              "border-bottom-left-radius: 10px;"
                              "border-top-right-radius: 10px;"
                              "border-bottom-right-radius: 10px;"
                              "background-color:skyblue;}"
                              );
    setStyleSheet("QPushButton{background-color: white;"
                  "border: 1px solid #DDDDDD;"
                  "color: rgb(100, 100, 100);}"
                  "QPushButton:hover{background-color:rgb(230, 230, 230);}");
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

