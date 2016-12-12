#ifndef MYMISSIONPAGE_H
#define MYMISSIONPAGE_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QList>

class MyMToolBar;
class MyMissionInfoBar;

class MyMissionPage : public QWidget
{
    Q_OBJECT
public:
    explicit MyMissionPage(QWidget *parent = 0);
	
public slots:	
	void SetProgress(int i, float pro, unsigned int speed);
	void Complete(int i);

private:
    void InitWidget();
    void InitLayout();
    void SetThisStyle();

private:
    QListWidget* lpMissionList;
    MyMToolBar* lpToolBar;
	QList<MyMissionInfoBar*> info;
    QVBoxLayout* lpMainLayout;

    QPalette* lpPal;
signals:
    void Suspend(int);
    void Resume(int);
    void Cancel(int);
    void OpenDir(int);

public slots:
    void AddMission(QString);

private slots:
    void pressedSuspend(int);
    void pressedResume(int);
    void pressedCancel(int);
    void pressedOpenDir(int);

    void pressedSuspendAll();
    void pressedResumeAll();
    void pressedCancelAll();

};

#endif // MYMISSIONPAGE_H
