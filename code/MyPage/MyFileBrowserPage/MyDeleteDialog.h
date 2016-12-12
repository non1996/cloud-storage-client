#ifndef MYDELETEDIALOG_H
#define MYDELETEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MyDeleteDialog : public QDialog
{
    Q_OBJECT
public:
    MyDeleteDialog(QWidget* parent = 0);

    void SetHint(QString);
private:
    QLabel* lpHint;
    QPushButton* lpOk;
    QPushButton* lpCancel;

    QVBoxLayout* lpMainLayout;
    QHBoxLayout* lpSubLayout;
};

#endif // MYDELETEDIALOG_H
