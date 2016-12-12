#ifndef MYNEWDIRDIALOG_H
#define MYNEWDIRDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

class MyNewDirDialog : public QDialog
{
    Q_OBJECT
public:
    MyNewDirDialog(QWidget* parent = 0);
    void Clean();
    void SetHint(QString txt){
        lpHint->setText(txt);
    }
    void SetEditText(QString txt){
        lpNameInput->setText(txt);
    }
    QString GetContent(){
        return lpNameInput->text();
    }

protected:
    void paintEvent(QPaintEvent* event);

    void mousePressEvent(QMouseEvent *);

    void mouseMoveEvent(QMouseEvent *);

    void mouseReleaseEvent(QMouseEvent *);

private:
    void InitWidget();
    void InitLayout();
    void SetThisStyle();
    void ConnectSlot();

private:
    QPushButton* lpOK;
    QPushButton* lpCancel;

    QLineEdit* lpNameInput;
    QLabel* lpHint;
    QLabel* lpFalse;

    QVBoxLayout* lpMainLayout;
    QHBoxLayout* lpSubLayout;

    QPoint last;

    bool isMoving;

signals:
    void GetDirName(QString);

private slots:
    void pressOk();
};

#endif // MYNEWDIRDIALOG_H
