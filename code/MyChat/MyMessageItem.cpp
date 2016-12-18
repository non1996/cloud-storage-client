#include "MyMessageItem.h"
#include <QMenu>
#include <QApplication>
#include <QClipboard>

MyMessageItem::MyMessageItem(int type, QString &cID, QString & msg, QWidget *parent)
    : QWidget(parent)
{
    lpMessage = new QLabel(msg, this);
    lpUserCID = new QLabel(cID, this);

    lpMessage->setMinimumSize(100, 30);
    lpMessage->adjustSize();
    lpMessage->setWordWrap(true);

    lpCopy = new QAction("copy", this);
    lpCopy->setEnabled(true);
    lpMainLayout = new QVBoxLayout(this);
    setLayout(lpMainLayout);

    lpMessage->setAlignment(Qt::AlignVCenter);
    lpMessage->setMargin(3);
    lpUserCID->setAlignment(Qt::AlignCenter);

    QHBoxLayout *lpSub1 = new QHBoxLayout(this);
    QHBoxLayout *lpSub2 = new QHBoxLayout(this);

    if(type == RECV){
        lpSub1->addWidget(lpUserCID);
        lpSub1->addStretch();
        lpSub2->addWidget(lpMessage);
        lpSub2->addStretch();
    }
    else{
        lpSub1->addStretch();
        lpSub1->addWidget(lpUserCID);
        lpSub2->addStretch();
        lpSub2->addWidget(lpMessage);
    }
    lpMainLayout->addLayout(lpSub1);
    lpMainLayout->addLayout(lpSub2);
    SetStyle();

    connect(lpCopy, SIGNAL(triggered(bool)), this, SLOT(copyContent()));
}

//void MyMessageItem::SetCID(QString &c)
//{
//    lpUserCID->setText(c);
//}

//void MyMessageItem::SetMessage(QString &m)
//{
//    lpMessage->setText(m);
//}

void MyMessageItem::mousePressEvent(QMouseEvent *event)
{
    QMenu* menu = new QMenu(this);
    if(event->button() == Qt::RightButton){
        menu->addAction(lpCopy);
        menu->exec(QCursor::pos());
    }
}

void MyMessageItem::SetStyle()
{
    setStyleSheet("QWidget{color:rgb(217, 217, 217);"
                  "font: bold 14px;}");
    lpMessage->setStyleSheet("QLabel{background-color:rgb(120, 120, 120);"
                             "border-radius: 3px;"
                             "border-color: rgb(60, 60, 60)}");
}

void MyMessageItem::copyContent()
{
    QClipboard* board = QApplication::clipboard();
    board->setText(lpMessage->text());
}

