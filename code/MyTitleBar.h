#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H

#include <QWidget>
#include <QHBoxLayout>

class QLabel;
class QPushButton;

//-------------------------------------
//	标题栏类
//-------------------------------------
class MyTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit MyTitleBar(QWidget *parent = 0);
    ~MyTitleBar(){}

private:
    void createWidget();

    void createLayout();

    void setButtonStyle();

    void setIcon(QPushButton* b, QString path);
private:
    QLabel *lpIconLabel;				//标志
    QLabel *lpTitleLabel;				//程序名
    QPushButton *lpMinimizeButton;		//最小化按钮
    QPushButton *lpMaximizeButton;		//最大化按钮
    QPushButton *lpCloseButton;			//关闭按钮

    QHBoxLayout *lpLayout;
signals:
	void CloseWindow();

public slots:
    void showSmall(){
        parentWidget()->showMinimized();
    }
	void sendCloseWindow();
};

#endif // MYTITLEBAR_H
