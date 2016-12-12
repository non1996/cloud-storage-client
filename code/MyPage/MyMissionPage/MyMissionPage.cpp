#include "MyMissionPage.h"
#include "MyMissionInfoBar.h"
#include "MyMToolBar.h"
#include <QDebug>

MyMissionPage::MyMissionPage(QWidget *parent) : QWidget(parent)
{
    InitWidget();
    InitLayout();
    SetThisStyle();

    connect(lpToolBar, SIGNAL(SuspendAll()), this, SLOT(pressedSuspendAll()));
    connect(lpToolBar, SIGNAL(ResumeAll()), this, SLOT(pressedResumeAll()));
    connect(lpToolBar, SIGNAL(CancelAll()), this, SLOT(pressedCancelAll()));
}

void MyMissionPage::SetProgress(int i, float pro, unsigned int speed)
{
	info.at(i)->SetProgress(pro);
	info.at(i)->SetSpeed(speed);
}

void MyMissionPage::Complete(int i)
{
	info.at(i)->SetFinish();
}

void MyMissionPage::InitWidget()
{
    lpMissionList = new QListWidget(this);
    lpToolBar = new MyMToolBar(this);

    lpMissionList->setResizeMode(QListView::Adjust);
    lpMissionList->setMovement(QListView::Static);
    lpMissionList->setSpacing(1);
    lpMissionList->setGeometry(0, 40, 950, 450);

//    lpList = new QList<MyMissionInfoBar*>();
}

void MyMissionPage::InitLayout()
{
    lpMainLayout = new QVBoxLayout(this);
    lpMainLayout->setSpacing(0);
    lpMainLayout->setMargin(0);

    lpMainLayout->addWidget(lpToolBar);
    lpMainLayout->addWidget(lpMissionList);
    setLayout(lpMainLayout);
}

void MyMissionPage::SetThisStyle()
{
    setMinimumSize(950, 450);
    setAutoFillBackground(true);
    lpPal = new QPalette();
    lpPal->setColor(QPalette::Background, QColor(255, 255, 255));
    setPalette(*lpPal);
}

void MyMissionPage::AddMission(QString name)
{
    QListWidgetItem* item = new QListWidgetItem(lpMissionList);
    MyMissionInfoBar* temp = new MyMissionInfoBar();
    lpMissionList->addItem(item);
    lpMissionList->setItemWidget(item, temp);
    item->setSizeHint (QSize(temp->rect().width(),temp->rect().height()));
	temp->SetProgress(0);
	temp->SetName(name);
    temp->SetRunning(true);
    temp->SetNum(lpMissionList->count() - 1);
	info.append(temp);
    connect(temp, SIGNAL(Suspend(int)), this, SLOT(pressedSuspend(int)));
    connect(temp, SIGNAL(Resume(int)), this, SLOT(pressedResume(int)));
    connect(temp, SIGNAL(Cancel(int)), this, SLOT(pressedCancel(int)));
    connect(temp, SIGNAL(OpenDir(int)), this, SLOT(pressedOpenDir(int)));
}

void MyMissionPage::pressedSuspend(int n)
{
    emit Suspend(n);
//    qDebug() << "任务" << n << "暂停" << endl;
}

void MyMissionPage::pressedResume(int n)
{
    emit Resume(n);
}

void MyMissionPage::pressedCancel(int n)
{
    emit Cancel(n);
}

void MyMissionPage::pressedOpenDir(int n)
{
    emit OpenDir(n);
}

void MyMissionPage::pressedSuspendAll()
{
    for(int i = 0; i < lpMissionList->count(); ++i){
        emit Suspend(i);
    }
}

void MyMissionPage::pressedResumeAll()
{
    for(int i = 0; i < lpMissionList->count(); ++i){
        emit Resume(i);
    }
}

void MyMissionPage::pressedCancelAll()
{
    for(int i = 0; i < lpMissionList->count(); ++i){
        emit Cancel(i);
    }
}
