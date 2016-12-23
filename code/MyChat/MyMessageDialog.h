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
//	���׶Ի�����
//-------------------------------------------
class MyMessageDialog : public QDialog
{
    Q_OBJECT
public:
    MyMessageDialog(QWidget* parent = 0);
private:
    void SetStyle();
private:
    QListWidget* lpMessage;			//��Ϣ��
    QLabel* lpCID;					
    QLineEdit* lpCIDEdit;			//�û��ǳ������
    QTextEdit* lpTextEdit;			//�����
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
