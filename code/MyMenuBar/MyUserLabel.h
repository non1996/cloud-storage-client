#ifndef MYUSERLABEL_H
#define MYUSERLABEL_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>

class MyUserLabel : public QWidget
{
    Q_OBJECT
public:
    explicit MyUserLabel(QWidget *parent = 0);

private:
    void InitWidget();

    void InitLayout();

    void SetWidgetStyle();

private:
    QLabel* lpUserProfilePic;
    QLabel* lpUsername;
    QProgressBar* lpNetCapacity;

    QHBoxLayout* lpHLayout;
    QVBoxLayout* lpVLayout;

signals:

public slots:
};

#endif // MYUSERLABEL_H
