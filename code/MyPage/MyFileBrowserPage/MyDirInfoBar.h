#ifndef MYDIRINFOBAR_H
#define MYDIRINFOBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>


//----------------------------------------------
//	·����
//----------------------------------------------
class MyDirInfoBar : public QWidget
{
    Q_OBJECT
public:
    explicit MyDirInfoBar(QWidget *parent = 0);

	void SetPath(QString &path);
private:
    void InitWidget();
    void InitLayout();
    void SetThisStyle();
    void SetWidgetStyle();
    void ConnectSlot();
	void setIcon(QPushButton*, QString);
private:
    QPushButton* lpBack;			//���˵��ϼ�Ŀ¼��ť
    QPushButton* lpFront;			//����ǰһ�����ʵ�Ŀ¼��ť
    QPushButton* lpFresh;			//ˢ�½��水ť
	QPushButton* lpHome;			//�ص���Ŀ¼
    QLabel* lpPath;

    QLineEdit* lpInputSearch;		//�ļ��������
    QPushButton* lpSearch;			//�����ļ���ť

    QHBoxLayout* lpMainLayout;

    QPalette* lpPal;
signals:
    void Back();
    void Front();
    void Fresh();
    void Search(QString);
	void Home();
private slots:
    void sendBack();
    void sendFront();
    void sendFresh();
    void sendSearch();
	void sendHome();
};

#endif // MYDIRINFOBAR_H
