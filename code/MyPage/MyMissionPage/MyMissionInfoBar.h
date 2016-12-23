#ifndef MYMISSIONINFOBAR_H
#define MYMISSIONINFOBAR_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QHBoxLayout>

//---------------------------------------------
//	任务信息框
//	显示任务名字，任务下载速度，任务下载进度
//---------------------------------------------
class MyMissionInfoBar : public QWidget
{
    Q_OBJECT
public:
    explicit MyMissionInfoBar(QWidget *parent = 0);

    void SetName(QString name);
    void SetProgress(float);
	void SetSpeed(unsigned int);
	void SetFinish();
    void SetRunning(bool);
    void SetNum(int);
    void ConnectSlot();
private:
    void InitWidgetDefault();
    void InitInfo();
    void InitLayout();
    void SetThisStyle();
    void SetWidgetStyle();

private:
    //widget
    QLabel* lpFileIcon;
    QLabel* lpFileName;
	QLabel* lpSpeed;
    QProgressBar* lpProgress;
    QPushButton* lpResumeOrSuspend;
    QPushButton* lpCancel;
    QPushButton* lpDir;

    QHBoxLayout* lpMainLayout;

    int num;
    bool isRunning;
signals:
    void Suspend(int);
    void Resume(int);
    void Cancel(int);
    void OpenDir(int);
private slots:
    void pressedSuspendOrResume();
    void pressedCancel();
    void pressedOpenDir();
};

#endif // MYMISSIONINFOBAR_H
