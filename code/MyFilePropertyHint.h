#pragma once

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPushButton>

class MyFilePropertyHint :
	public QDialog
{
	Q_OBJECT
private:
	QLabel* lpName;
	QLabel* lpPath;
	QLabel* lpDate;
	QLabel* lpSize;
	QLabel* lpType;
	QPushButton* lpClose;
	QPoint last;
	bool isMoving;

	QVBoxLayout* lpMainLayout;
protected:
	void paintEvent(QPaintEvent *event);

	void mousePressEvent(QMouseEvent *);

	void mouseMoveEvent(QMouseEvent *);

	void mouseReleaseEvent(QMouseEvent *);

public:
	MyFilePropertyHint(QWidget* parent = 0);
	~MyFilePropertyHint();

	void SetText(QString & name, QString & path, QString & date, QString & size, QString & type);
};

