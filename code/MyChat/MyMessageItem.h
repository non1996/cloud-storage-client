#ifndef MYMESSAGEITEM_H
#define MYMESSAGEITEM_H

#include <QWidget>
#include <QTextBrowser>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

//--------------------------------------------
//	�Ի��࣬ÿ���������һ���Ի���Ϣ
//--------------------------------------------
class MyMessageItem : public QWidget
{
    Q_OBJECT
public:
    enum{
        RECV,
        SEND
    };
public:
    explicit MyMessageItem(int type, QString &cID, QString & msg, QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
private:
    void SetStyle();

private:
    QLabel* lpMessage;			//��Ϣ��
    QLabel* lpUserCID;			//�û��ǳ�
    QVBoxLayout* lpMainLayout;	
    QAction* lpCopy;
signals:

private slots:
    void copyContent();
};

#endif // MYMESSAGEITEM_H
