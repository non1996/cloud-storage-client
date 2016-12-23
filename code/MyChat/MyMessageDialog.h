#ifndef MYMESSAGEDIALOG_H
#define MYMESSAGEDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>

//-------------------------------------------
//	简易对话框类
//-------------------------------------------
class MyMessageDialog : public QDialog
{
    Q_OBJECT
public:
    MyMessageDialog(QWidget* parent = 0);
private:
    void SetStyle();
private:
    QListWidget* lpMessage;			//消息框
    QLabel* lpCID;					
    QLineEdit* lpCIDEdit;			//用户昵称输入框
    QTextEdit* lpTextEdit;			//输入框
	QLabel* lpHint;
    QVBoxLayout* lpMainLayout;

    QPushButton* lpHide;
    QPushButton* lpOk;

signals:
    void SendMessage(QString, QString);

public slots:
    void AddSendMessage(QString, QString);
    void AddRecvMessage(QString, QString);

private slots:
    void send();
};

#endif // MYMESSAGEDIALOG_H
