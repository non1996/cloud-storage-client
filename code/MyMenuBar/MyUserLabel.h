#ifndef MYUSERLABEL_H
#define MYUSERLABEL_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>

//------------------------------------------
//	用户信息区域
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
    QLabel* lpUserProfilePic;		//用户头像
    QLabel* lpUsername;				//用户名
	QLabel* lpVInfo;				//容量信息
    QProgressBar* lpNetCapacity;	//网盘容量

    QHBoxLayout* lpHLayout;
    QVBoxLayout* lpVLayout;

signals:

public slots:
};

#endif // MYUSERLABEL_H
