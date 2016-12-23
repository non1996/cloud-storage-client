
#ifndef MYFBTOOLBAR_H
#define MYFBTOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

class MyToolButton;
//---------------------------------------------
//	文件浏览器界面工具栏
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

    QPushButton* lpUpload;		//上传按钮
    QPushButton* lpDownload;	//下载按钮
    QPushButton* lpShare;		//共享按钮
	QPushButton* lpFork;		//fork按钮
	QPushButton* lpDelete;		//删除按钮
    QPushButton* lpNewDir;		//新建文件夹按钮
	

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
