#include "MyPasteErrorHint.h"

MyPasteErrorHint::MyPasteErrorHint(QWidget *parent) : QWidget(parent)
{
    lpHint = new QLabel("has same name file", this);
    setGeometry(300, 600, 400, 20);
    hide();
}

void MyPasteErrorHint::SetHint(QString str)
{
	lpHint->setText(str);
}
