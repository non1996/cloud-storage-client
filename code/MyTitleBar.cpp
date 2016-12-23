# pragma execution_character_set("utf-8")
#include "MyTitleBar.h"

#include <QLabel>
#include <QPushButton>

MyTitleBar::MyTitleBar(QWidget *parent)
    :QWidget(parent)
{
    setFixedHeight(22);

    createWidget();

    createLayout();

    setButtonStyle();

    connect(lpCloseButton, SIGNAL(clicked()), parent, SLOT(close()));
    connect(lpMinimizeButton, SIGNAL(clicked()), this, SLOT(showSmall()));
}

void MyTitleBar::createWidget(){
    lpIconLabel = new QLabel(this);
    lpTitleLabel = new QLabel(this);
    lpMinimizeButton = new QPushButton(this);
    lpMaximizeButton = new QPushButton(this);
    lpCloseButton = new QPushButton(this);

    QPixmap pixmap(22, 22);
    pixmap.load("image/title/icon.png");
    lpIconLabel->setPixmap(pixmap);
    lpIconLabel->setFixedSize(22, 22);
    lpIconLabel->setScaledContents(true);

    lpTitleLabel->setText("顶点云");
    lpTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	lpTitleLabel->setStyleSheet("QLabel{color:rgb(217, 217, 217);font: bold 14px;}");

    lpMinimizeButton->setFixedSize(27, 22);
    lpMaximizeButton->setFixedSize(27, 22);
    lpCloseButton->setFixedSize(27, 22);

    lpTitleLabel->setObjectName("whiteLabel");
    lpMinimizeButton->setObjectName("minimizeButton");
    lpMaximizeButton->setObjectName("maximizeButton");
    lpCloseButton->setObjectName("closeButton");

    lpMinimizeButton->setToolTip("最小化");
    lpMaximizeButton->setToolTip("最大化");
    lpCloseButton->setToolTip("关闭");
}

void MyTitleBar::createLayout()
{
    lpLayout = new QHBoxLayout(this);
    lpLayout->setMargin(0);

    lpLayout->addWidget(lpIconLabel);
    lpLayout->addSpacing(5);
    lpLayout->addWidget(lpTitleLabel);
    lpLayout->addWidget(lpMinimizeButton);
    lpLayout->addWidget(lpMaximizeButton);
    lpLayout->addWidget(lpCloseButton);
    lpLayout->setSpacing(0);
    lpLayout->setContentsMargins(5, 0, 0, 0);

    setLayout(lpLayout);
}

void MyTitleBar::setButtonStyle()
{
    setStyleSheet("QPushButton{background-color: rgb(57, 63, 63);"
                  "color: rgb(0, 0, 0);"
                  "border-style: outset;"
                  "border-width: 0px;"
                  "border-color: beige;"
                  "font: bold 14px;"
                  "padding: 6px;"
                  "text-align: center;}"
                  "QPushButton:hover{background-color: rgb(105, 112, 112)}");

    setIcon(lpMinimizeButton, "image/title/min.png");
    setIcon(lpMaximizeButton, "image/title/max.png");
    setIcon(lpCloseButton, "image/title/close.png");
}

void MyTitleBar::setIcon(QPushButton* b, QString path)
{
    QPixmap *pixmap = new QPixmap(22, 22);
    pixmap->load(path);
    QIcon *icon = new QIcon(*pixmap);
    b->setIcon(*icon);
    b->setIconSize(QSize(22, 22));
}

void MyTitleBar::sendCloseWindow()
{
	emit CloseWindow();
}
