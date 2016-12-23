#ifndef MYLOGINDIALOG_H
#define MYLOGINDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

//-----------------------------------------
//	登陆框
//-----------------------------------------
class MyLogInDialog : public QDialog
{
    Q_OBJECT
public:
    MyLogInDialog(QWidget* parent = 0);
private:
    QLabel* lpU;
    QLabel* lpP;
	QLabel* lpHint;

    QLineEdit* lpUsername;			//用户名输入框
    QLineEdit* lpPassword;			//密码输入框

    QPushButton* lpLogIn;			//登陆按钮
    QPushButton* lpCancel;			//取消按钮

    QVBoxLayout* lpMainLayout;
    QHBoxLayout* lpSubLayout1;
    QHBoxLayout* lpSubLayout2;
    QHBoxLayout* lpSubLayout3;

	void SetStyle();
private slots:
    void ok();
    void cancel();
};

#endif // MYLOGINDIALOG_H
