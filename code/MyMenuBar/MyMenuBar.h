#ifndef MYMENUBAR_H
#define MYMENUBAR_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class MyUserLabel;
class MyToolButton;

//--------------------------------------------------
//	�˵����࣬��ʾ�û���Ϣ
//	�������������л���ť���ļ���������棬���ؽ��棬�ϴ�����
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
    MyUserLabel* lpUserInfo;				//�û���Ϣ��ʾ
    MyToolButton* lpMainWindowButton;		//�ļ��������ť
    MyToolButton* lpDownloadButton;			//���ؽ��水ť
    MyToolButton* lpUploadButton;			//�ϴ����水ť

    QHBoxLayout* lpSwitchLayout;
signals:
    void SwitchPage(int);

public slots:
	//������ť����֮һ������ʱ��������������ͬʱ�������淢���л�ҳ����ź�
    void ResponseSlot(MyToolButton*);

	void SetUserInfo(QString, QString, unsigned long long, unsigned long long);
};

#endif // MYMENUBAR_H
