#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>

class MyTitleBar;
class MyMenuBar;
class MyPage;
class MyPasteErrorHint;
class MyFilePropertyHint;
class MyChatBar;
class MyMessageDialog;
class MyMessageDialog;

class MyMainWindow : public QFrame
{
    Q_OBJECT
public:
    MyMainWindow();

public:					//this function are invoke by control
	bool SetPath(std::string &path);
	bool CleanBrowser();
	void AddFile(std::string &name);
	void Rename(std::string &oldName, std::string &newName);
	void DeleteNetFile(std::string &name);
	void AddUploadMission(std::string &s);

	void SetUploadProgress(int i, float pro, unsigned int speed);
	void SetDownloadProgress(int i, float pro, unsigned int speed);
	void DownloadComplete(int i);
	void UploadComplete(int i);

	void showMessage(std::string, std::string);
	void sendSuccess(std::string, std::string);

	void showInfo(QString s);
	
signals:
	void SetUploadProgress_signal(int, float, unsigned int);
	void SetDownloadProgress_signal(int, float, unsigned int);
	void DownloadComplete_signal(int);
	void UploadComplete_signal(int);

	void ShowMessage(QString, QString);
	void ShowSendMessage(QString, QString);

protected:
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *);

    void mouseMoveEvent(QMouseEvent *);

    void mouseReleaseEvent(QMouseEvent *);

private:
    void InitWidget();

    void InitLayout();

    void InitSlot();

    void setThisStyle();



private:
    QVBoxLayout* lpMainLayout;
    MyTitleBar* lpTitleBar;
    MyMenuBar* lpMenuBar;
    MyPage* lpPage;

    MyPasteErrorHint* lpError;
	MyFilePropertyHint* lpFileProperty;
	
	MyChatBar* lpChatBar;
	MyMessageDialog* lpChat;

    QPoint last;

    bool isMoving;

    int timerHandle;

private slots:
    void startDownloadMission(QString);	//
    void startUploadMission(QString);	//
    void shareFile(QString);			//
    void deleteFile(QString);			//
    void makeNewDir(QString);			//
    void toUpperDir();					//
    void toBeforeDir();					//
    void refreshDir();					//
    void searchFile(QString);			//

    void enterDir(QString);				//
    void renameFile(QString, QString);	//
    void getProperty(QString);			//
    void copy(QString);					//
    void cut(QString);					//
    void paste();						//
	void send(QString, QString);
	
	void showFileInfo(QString &name, QString &path, QString &date, QString &size, QString &type);

	//protocol did not support yet
    void suspendD(int);
    void resumeD(int);
    void cancelD(int);
    void openDirD(int);					//

    void suspendU(int);
    void resumeU(int);
    void cancelU(int);
    void openDirU(int);					

	void showChatDialog();

	void CleanAndClose();
};

#endif // MYMAINWINDOW_H
