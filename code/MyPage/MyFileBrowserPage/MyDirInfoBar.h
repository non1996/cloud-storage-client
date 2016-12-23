#ifndef MYDIRINFOBAR_H
#define MYDIRINFOBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>


//----------------------------------------------
//	路径栏
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
    QPushButton* lpBack;			//回退到上级目录按钮
    QPushButton* lpFront;			//访问前一个访问的目录按钮
    QPushButton* lpFresh;			//刷新界面按钮
	QPushButton* lpHome;			//回到根目录
    QLabel* lpPath;

    QLineEdit* lpInputSearch;		//文件名输入框
    QPushButton* lpSearch;			//查找文件按钮

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
