#ifndef MYUSERLABEL_H
#define MYUSERLABEL_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>

//------------------------------------------
//	�û���Ϣ����
//------------------------------------------
class MyUserLabel : public QWidget
{
    Q_OBJECT
public:
    explicit MyUserLabel(QWidget *parent = 0);

	void SetInfo(QString, QString, unsigned long long, unsigned long long);

private:
    void InitWidget();

    void InitLayout();

    void SetWidgetStyle();

	QString GetV(unsigned long long);

private:
    QLabel* lpUserProfilePic;		//�û�ͷ��
    QLabel* lpUsername;				//�û���
	QLabel* lpVInfo;				//������Ϣ
    QProgressBar* lpNetCapacity;	//��������

    QHBoxLayout* lpHLayout;
    QVBoxLayout* lpVLayout;

signals:

public slots:
};

#endif // MYUSERLABEL_H
