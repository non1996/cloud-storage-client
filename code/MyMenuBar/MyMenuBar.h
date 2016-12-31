#ifndef MYMENUBAR_H
#define MYMENUBAR_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class MyUserLabel;
class MyToolButton;

//--------------------------------------------------
//	菜单栏类，显示用户信息
//	包含三个界面切换按钮：文件浏览器界面，下载界面，上传界面
//--------------------------------------------------
class MyMenuBar : public QWidget
{
    Q_OBJECT
public:
    explicit MyMenuBar(QWidget *parent = 0);

	void SetCapasity(unsigned long long);
private:
    void createWidget();
    void createLayout();
    void setMenuBarStyle();
    void setSlotConnect();

private:
    MyUserLabel* lpUserInfo;				//用户信息显示
    MyToolButton* lpMainWindowButton;		//文件浏览器按钮
    MyToolButton* lpDownloadButton;			//下载界面按钮
    MyToolButton* lpUploadButton;			//上传界面按钮

    QHBoxLayout* lpSwitchLayout;
signals:
    void SwitchPage(int);

public slots:
	//三个按钮其中之一被按下时，将另两个弹起，同时向主界面发出切换页面的信号
    void ResponseSlot(MyToolButton*);

	void SetUserInfo(QString, QString, unsigned long long, unsigned long long);
};

#endif // MYMENUBAR_H
