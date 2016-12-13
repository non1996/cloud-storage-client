# pragma execution_character_set("utf-8")
#include "MyDeleteDialog.h"

MyDeleteDialog::MyDeleteDialog(QWidget *parent)
    :QDialog(parent)
{
    lpHint = new QLabel(this);
    lpOk = new QPushButton("ok", this);
    lpOk->setMinimumHeight(40);
    lpCancel = new QPushButton("cancel", this);
    lpCancel->setMinimumHeight(40);

    lpMainLayout = new QVBoxLayout(this);
    lpSubLayout = new QHBoxLayout(this);
    lpSubLayout->addWidget(lpOk);
    lpSubLayout->addWidget(lpCancel);
    lpMainLayout->addWidget(lpHint);
    lpMainLayout->addLayout(lpSubLayout);
    setLayout(lpMainLayout);

	setStyleSheet("QDialog{background-color:rgb(64, 64, 64);"
						"border-style: inset;"
						"border-width: 2px;"
						"border-color: rgb(60, 60, 60)}"
						"QPushButton{background-color:rgb(64, 64, 64);"
						"color: rgb(217, 217, 217);"
						"border-style: outset;"
						"border-width: 0px;}"
						"QPushButton:hover{background-color: rgb(80, 80, 80)}"
						"QWidget{color:rgb(217, 217, 217);font: bold 14px;}");
    setWindowFlags(Qt::FramelessWindowHint);

    setModal(true);
    setGeometry(200, 200, 400, 100);
    connect(lpOk, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(lpCancel, SIGNAL(clicked(bool)), this, SLOT(close()));
}

void MyDeleteDialog::SetHint(QString name)
{
    QString hint = "确定要删除文件";
    hint += name + "吗?";
    lpHint->setText(hint);
}
