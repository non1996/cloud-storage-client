#ifndef MYMESSAGEITEM_H
#define MYMESSAGEITEM_H

#include <QWidget>
#include <QTextBrowser>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

//--------------------------------------------
//	对话类，每个对象代表一条对话信息
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
    QLabel* lpMessage;			//消息框
    QLabel* lpUserCID;			//用户昵称
    QVBoxLayout* lpMainLayout;	
    QAction* lpCopy;
signals:

private slots:
    void copyContent();
};

#endif // MYMESSAGEITEM_H
