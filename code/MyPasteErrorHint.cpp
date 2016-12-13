# pragma execution_character_set("utf-8")
#include "MyPasteErrorHint.h"

MyPasteErrorHint::MyPasteErrorHint(QWidget *parent) : QWidget(parent)
{
    lpHint = new QLabel("存在同名文件", this);
	lpHint->setStyleSheet("QLabel{color:rgb(217, 217, 217);font: bold 14px;}");
    setGeometry(300, 600, 400, 20);
    hide();
}

void MyPasteErrorHint::SetHint(QString str)
{
	lpHint->setText(str);
}
