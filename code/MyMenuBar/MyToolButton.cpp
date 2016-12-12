#include "MyToolButton.h"

MyToolButton::MyToolButton(QWidget* parent)
    :QToolButton(parent)
{

}

MyToolButton::MyToolButton(const QString &_strImage, const QString &_strInfo, QWidget *parent)
    :QToolButton(parent), over(false), press(false), strImage(_strImage), strInfo(_strInfo)
{
    //文本颜色
    QPalette objPalette = palette();
    objPalette.setColor(QPalette::ButtonText, QColor(100, 100, 100));
    setPalette(objPalette);
    //文本粗体
    QFont &objFont = const_cast<QFont &>(font());
    objFont.setWeight(QFont::Bold);
    //样式
    setStyleSheet("QToolButton{border:0px;}");
    //大小
    setIconSize(QSize(50, 50));
    resize(100, 100);
    setIcon(QPixmap(strImage));
    setText(strInfo);

    connect(this, SIGNAL(pressed()), this, SLOT(SlotPressed()));
}

void MyToolButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(press){
        painterinfo(255, 255, &painter);
    }
    else if(over){
        painterinfo(255, 255, &painter);
    }
    QToolButton::paintEvent(event);
}

void MyToolButton::enterEvent(QEvent *event)
{
    SetOver(true);
}

void MyToolButton::leaveEvent(QEvent *event)
{
    SetOver(false);
}

void MyToolButton::painterinfo(int topPartOpacity, int bottomPartOpacity, QPainter *painter)
{
    QPen pen(Qt::NoBrush, 1);
    painter->setPen(pen);
    QLinearGradient linear(rect().topLeft(), rect().bottomLeft());
    linear.setColorAt(0, QColor(061, 165, 252, topPartOpacity));
    linear.setColorAt(0.5, QColor(061, 165, 252, 255));
    linear.setColorAt(1, QColor(061, 165, 252, bottomPartOpacity));

    QBrush brush(linear);
    painter->setBrush(brush);
    painter->drawRoundedRect(rect(), 0, 0);
}

void MyToolButton::SlotPressed()
{
    SetPress(true);
    emit SignalParent(this);
}

