#ifndef MYMTOOLBAR_H
#define MYMTOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

//------------------------------------------------
//	传输页面工具栏
//------------------------------------------------
class MyMToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit MyMToolBar(QWidget *parent = 0);

private:
    void createWidget();

    void createLayout();

    void setToolBarStyle();

    void setButtonStyle();

    void setSlotConnect();

    void ConnectSlot();

private:
    QHBoxLayout* lpLayout;

    QLabel* lpTransmitSpeed;
    QPushButton* lpResumeOrSuspendAll;
    QPushButton* lpCancelAll;

    QPalette* lpPal;

    bool isRun;
signals:
    void ResumeAll();
    void SuspendAll();
    void CancelAll();

public slots:
    void SetSpeed(float);
private slots:
    void sendRS();
    void sendC();
};

#endif // MYMTOOLBAR_H
