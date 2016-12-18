#include "MyChatBar.h"

MyChatBar::MyChatBar(QWidget* parent)
    :QWidget(parent)
{
    lpShowChat = new QPushButton(this);
    lpInfo = new QLabel(this);
    lpShowChat->setFixedSize(30, 30);
    lpInfo->setMinimumSize(100, 30);
    lpLayout = new QHBoxLayout(this);

	QPixmap *pixmap = new QPixmap(22, 22);
	pixmap->load("image/chat/chat.png");
	QIcon *icon = new QIcon(*pixmap);
	lpShowChat->setIcon(*icon);
	lpShowChat->setIconSize(QSize(22, 22));

    lpLayout->addWidget(lpInfo);
    lpLayout->addStretch();
    lpLayout->addWidget(lpShowChat);
    setLayout(lpLayout);

    setMinimumSize(950, 35);
    setAutoFillBackground(true);
    setStyleSheet("QWidget{"
                  "color:rgb(217, 217, 217);"
                  "border-style: inset;"
                  "border-width: 0px;"
                  "border-color: rgb(45, 45, 45);"
                  "color: rgb(217, 217, 217);"
                  "font: bold 14px;}"
                  "QPushButton{background-color:rgb(64, 64, 64);"
                  "border-style: outset;"
                  "border-width: 0px;}"
                  "QPushButton:hover{background-color: rgb(80, 80, 80)}");
    connect(lpShowChat, SIGNAL(clicked(bool)), this, SLOT(clickedShowChat()));

    QPalette lpPal = palette();
    lpPal.setColor(QPalette::Background, QColor(64, 64, 64));
    setPalette(lpPal);
}

void MyChatBar::SetText()
{
    lpInfo->setText("new message");
}

void MyChatBar::CleanText()
{
    lpInfo->clear();
}

void MyChatBar::clickedShowChat()
{
    emit ShowChat();
}
