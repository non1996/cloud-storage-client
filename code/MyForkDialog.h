#ifndef MYFORKDIALOG_H
#define MYFORKDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

class MyForkDialog: public QDialog
{
	Q_OBJECT
public:
	MyForkDialog(QWidget* parent = 0);
	~MyForkDialog() {}

	QString GetID();
	QString GetPass();

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

	QLineEdit* lpUIDInput;
	QLineEdit* lpPassInput;
	QLabel* lpHint1;
	QLabel* lpHint2;
	QLabel* lpFalse;

	QVBoxLayout* lpMainLayout;
	QHBoxLayout* lpSubLayout;

	QPoint last;

	bool isMoving;

private slots:
	void pressOk();
};

#endif // MYNEWDIRDIALOG_H