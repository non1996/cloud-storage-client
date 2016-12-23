#ifndef MYLOGINDIALOG_H
#define MYLOGINDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

//-----------------------------------------
//	��½��
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

    QLineEdit* lpUsername;			//�û��������
    QLineEdit* lpPassword;			//���������

    QPushButton* lpLogIn;			//��½��ť
    QPushButton* lpCancel;			//ȡ����ť

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
