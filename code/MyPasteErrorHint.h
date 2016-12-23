#ifndef MYPASTEERRORHINT_H
#define MYPASTEERRORHINT_H

#include <QWidget>
#include <QLabel>

//-----------------------------------
//	错误提示，好像没必要再继承一遍
//-----------------------------------
class MyPasteErrorHint : public QWidget
{
    Q_OBJECT
public:
    explicit MyPasteErrorHint(QWidget *parent = 0);
	void SetHint(const QString &str);
	
	void ShareOK();
	void ShareFalled();

	void EmptyHint();
	void ExistHint();
	void NetBroken();
private:
    QLabel* lpHint;

signals:

public slots:
};

#endif // MYPASTEERRORHINT_H
