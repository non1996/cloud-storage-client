# pragma execution_character_set("utf-8")
#include "MyDirInfoBar.h"

MyDirInfoBar::MyDirInfoBar(QWidget *parent) : QWidget(parent)
{
    InitWidget();
    InitLayout();
    SetThisStyle();
    SetWidgetStyle();
    ConnectSlot();
}

void MyDirInfoBar::SetPath(QString & path)
{
	lpPath->setText(path);
}

void MyDirInfoBar::InitWidget()
{
    lpBack = new QPushButton("<", this);
    lpFront = new QPushButton(">", this);
    lpFresh = new QPushButton(this);
	lpHome = new QPushButton(this);

	lpPath = new QLabel(this);
    lpInputSearch = new QLineEdit(this);
    lpSearch = new QPushButton(this);

	lpBack->setToolTip("回到上级");
	lpFront->setToolTip("上一个访问的目录");
	lpFresh->setToolTip("刷新");
	lpHome->setToolTip("回到根目录");
	lpSearch->setToolTip("查找");

	setIcon(lpFresh, "image/fileBrowser/toolbar/refresh.png");
	setIcon(lpSearch, "image/fileBrowser/toolbar/search.png");
	setIcon(lpHome, "image/fileBrowser/toolbar/home.png");
}

void MyDirInfoBar::InitLayout()
{
    lpMainLayout = new QHBoxLayout(this);
    lpMainLayout->setMargin(0);


    lpMainLayout->addWidget(lpBack);
    lpMainLayout->addWidget(lpFront);
    lpMainLayout->addWidget(lpFresh);
	lpMainLayout->addWidget(lpHome);
    lpMainLayout->addWidget(lpPath);
    lpMainLayout->addWidget(lpInputSearch);
    lpMainLayout->addWidget(lpSearch);

    setLayout(lpMainLayout);
}

void MyDirInfoBar::SetThisStyle()
{
    setAutoFillBackground(true);
    setMinimumSize(950, 40);

    lpPal = new QPalette();
    lpPal->setColor(QPalette::Background, QColor(64, 64, 64));
    setPalette(*lpPal);
}

void MyDirInfoBar::SetWidgetStyle()
{
    lpBack->setMinimumSize(40, 40);
    lpFront->setMinimumSize(40, 40);
    lpFresh->setMinimumSize(40, 40);
	lpHome->setMinimumSize(40, 40);
    lpPath->setFixedSize(500, 30);
    lpInputSearch->setMinimumSize(100, 30);
    lpSearch->setMinimumSize(40, 40);
    setStyleSheet( "QPushButton{background-color: rgb(64, 64, 64);"
                   "color: rgb(217, 217, 217);"
                   "border-style: outset;"
                   "border-width: 0px;"
                   "border-color: beige;"
                   "font: bold 14px;"
                   "padding: 6px;"
                   "text-align: center;}"
                   "QPushButton:hover{background-color: rgb(80, 80, 80)}");
    lpPath->setStyleSheet("QLabel{background-color: rgb(51, 51, 51);"
                          "color: rgb(217, 217, 217);"
                          "border-style: inset;"
                          "border-width: 1px;"
                          "border-color: rgb(45, 45, 45);"
                          "font: bold 14px;"
                          "padding: 6px;}");
    lpInputSearch->setStyleSheet("QLineEdit{background-color: rgb(51, 51, 51);"
                                 "color: rgb(217, 217, 217);"
                                 "border-style: inset;"
                                 "border-width: 1px;"
                                 "border-color: rgb(45, 45, 45);"
                                 "font: bold 14px;"
                                 "padding: 6px;}");
    lpInputSearch->setPlaceholderText("输入文件(夹)名");
}

void MyDirInfoBar::ConnectSlot()
{
    connect(lpBack, SIGNAL(clicked(bool)), this, SLOT(sendBack()));
    connect(lpFront, SIGNAL(clicked(bool)), this, SLOT(sendFront()));
    connect(lpFresh, SIGNAL(clicked(bool)), this, SLOT(sendFresh()));
	connect(lpHome, SIGNAL(clicked(bool)), this, SLOT(sendHome()));
    connect(lpSearch, SIGNAL(clicked(bool)), this, SLOT(sendSearch()));
}

void MyDirInfoBar::setIcon(QPushButton *b, QString n)
{
	QPixmap *pixmap = new QPixmap(22, 22);
	pixmap->load(n);
	QIcon *icon = new QIcon(*pixmap);
	b->setIcon(*icon);
	b->setIconSize(QSize(22, 22));
}

void MyDirInfoBar::sendBack()
{
    emit Back();
}

void MyDirInfoBar::sendFront()
{
    emit Front();
}

void MyDirInfoBar::sendFresh()
{
    emit Fresh();
}

void MyDirInfoBar::sendSearch()
{
    if(!lpInputSearch->text().isEmpty()){
        emit Search(lpInputSearch->text());
    }
}

void MyDirInfoBar::sendHome()
{
	emit Home();
}



