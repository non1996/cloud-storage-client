#ifndef MYPASTEERRORHINT_H
#define MYPASTEERRORHINT_H

#include <QWidget>
#include <QLabel>

class MyPasteErrorHint : public QWidget
{
    Q_OBJECT
public:
    explicit MyPasteErrorHint(QWidget *parent = 0);
	void SetHint(QString str);
private:
    QLabel* lpHint;

signals:

public slots:
};

#endif // MYPASTEERRORHINT_H
