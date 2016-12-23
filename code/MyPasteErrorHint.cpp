# pragma execution_character_set("utf-8")
#include "MyPasteErrorHint.h"

MyPasteErrorHint::MyPasteErrorHint(QWidget *parent) : QWidget(parent)
{
    lpHint = new QLabel("����ͬ���ļ�", this);
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
	lpHint->setText("����ɹ�");
}

void MyPasteErrorHint::ShareFalled()
{
	lpHint->setText("����ʧ��");
}

void MyPasteErrorHint::EmptyHint()
{
	lpHint->setText("ճ�������ļ�");
}

void MyPasteErrorHint::ExistHint()
{
	lpHint->setText("����ͬ���ļ�");
}

void MyPasteErrorHint::NetBroken()
{
	lpHint->setText("����Ͽ�");
}
