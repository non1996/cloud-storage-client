# pragma execution_character_set("utf-8")
#include "MyPasteErrorHint.h"

MyPasteErrorHint::MyPasteErrorHint(QWidget *parent) : QWidget(parent)
{
    lpHint = new QLabel("存在同名文件", this);
	lpHint->setStyleSheet("QLabel{color:rgb(217, 217, 217);font: bold 14px;}");
    setGeometry(300, 600, 400, 20);
    hide();
}

void MyPasteErrorHint::SetHint(const QString &str)
{
	lpHint->setText(str);
}

void MyPasteErrorHint::ShareOK()
{
	lpHint->setText("共享成功");
}

void MyPasteErrorHint::ShareFalled()
{
	lpHint->setText("共享失败");
}

void MyPasteErrorHint::EmptyHint()
{
	lpHint->setText("粘贴板无文件");
}

void MyPasteErrorHint::ExistHint()
{
	lpHint->setText("存在同名文件");
}

void MyPasteErrorHint::NetBroken()
{
	lpHint->setText("网络断开");
}
