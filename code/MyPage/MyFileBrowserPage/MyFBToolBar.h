
#ifndef MYFBTOOLBAR_H
#define MYFBTOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

class MyToolButton;
//---------------------------------------------
//	�ļ���������湤����
//---------------------------------------------
class MyFBToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit MyFBToolBar(QWidget *parent = 0);

private:
    void createWidget();

    void createLayout();

    void setToolBarStyle();

    void setButtonStyle();

    void setSlotConnect();

private:
    QHBoxLayout* lpLayout;

    QPushButton* lpUpload;		//�ϴ���ť
    QPushButton* lpDownload;	//���ذ�ť
    QPushButton* lpShare;		//����ť
	QPushButton* lpFork;		//fork��ť
	QPushButton* lpDelete;		//ɾ����ť
    QPushButton* lpNewDir;		//�½��ļ��а�ť
	

    QPalette* lpPal;

	void setIcon(QPushButton* b, QString path);

signals:
    void Upload();
    void Download();
    void Share();
    void Delete();
    void NewDir();
	void Fork();

private slots:
    void clickedUpload();
    void clickedDownload();
    void clickedShare();
    void clickedDelete();
    void clickedNewDir();
	void clickedFork();
};

#endif // MYTOOLBAR_H
