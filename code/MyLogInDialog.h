#ifndef MYLOGINDIALOG_H
#define MYLOGINDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class MyLogInDialog : public QDialog
{
    Q_OBJECT
public:
    MyLogInDialog(QWidget* parent = 0);
private:
    QLabel* lpU;
    QLabel* lpP;
	QLabel* lpHint;

    QLineEdit* lpUsername;
    QLineEdit* lpPassword;

    QPushButton* lpLogIn;
    QPushButton* lpCancel;

    QVBoxLayout* lpMainLayout;
    QHBoxLayout* lpSubLayout1;
    QHBoxLayout* lpSubLayout2;
    QHBoxLayout* lpSubLayout3;
private slots:
    void ok();
    void cancel();

};

#endif // MYLOGINDIALOG_H
