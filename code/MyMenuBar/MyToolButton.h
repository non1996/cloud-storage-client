#ifndef MYTOOLBUTTON_H
#define MYTOOLBUTTON_H

#include <QToolButton>
#include <QPalette>
#include <QPainter>
#include <QColor>

//---------------------------------------------
//	�Զ��尴ť�࣬���ڲ˵���
//---------------------------------------------
class MyToolButton : public QToolButton
{
    Q_OBJECT
public:
    MyToolButton(QWidget* parent);
    MyToolButton(const QString &strImage, const QString &strInfo, QWidget *parent);

    void SetOver(bool o){
        if(over != o){
            over = o;
            update();
        }
    }

    void SetPress(bool p){
        if(press != p){
            press = p;
            update();
        }
    }

    void paintEvent(QPaintEvent *event);

protected:
    void enterEvent(QEvent *event);

    void leaveEvent(QEvent *event);

    void painterinfo(int topPartOpacity, int bottomPartOpacity, QPainter *painter);
private:
    bool over;
    bool press;
    QString strImage;
    QString strInfo;

signals:
	//������ʱ���������ϼ��ؼ��������ϼ��ؼ����ĸ���ť������
    void SignalParent(MyToolButton*);
public slots:
    void SlotPressed();

//    void SlotUnPressed();
};

#endif // MYTOOLBUTTON_H
