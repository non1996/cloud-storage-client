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

//-------------------------------------------
//	主窗口类
//	
//-------------------------------------------
class MyMainWindow : public QFrame
{
    Q_OBJECT
public:
    MyMainWindow();

	//由控制线程调用改变界面
public:					
	//改变文件浏览器的界面
	bool SetPath(std::string &path);
	bool CleanBrowser();
	void AddFile(std::string &name);
	void Rename(std::string &oldName, std::string &newName);
	void DeleteNetFile(std::string &name);
	void AddUploadMission(std::string &s);

	//改变上传下载任务进度
	void SetUploadProgress(int i, float pro, unsigned int speed);
	void SetDownloadProgress(int i, float pro, unsigned int speed);
	void DownloadComplete(int i);
	void UploadComplete(int i);

	//为对话框添加消息
	void showMessage(std::string, std::string);
	void sendSuccess(std::string, std::string);
	
	//设置用户信息显示
	void SetUserInfo(std::string &name, std::string &url, unsigned long long currentV, unsigned long long totalV);

	//显示错误提示
	void showNetBrokenInfo();
	void showShareInfo(bool);
	void showInfo();

signals:
	//因为子线程不能直接改变Qwidget对象，因此调用上述函数后发出下述对应信号来操作控件
	void SetUploadProgress_signal(int, float, unsigned int);
	void SetDownloadProgress_signal(int, float, unsigned int);
	void DownloadComplete_signal(int);
	void UploadComplete_signal(int);
	void ShowMessage(QString, QString);
	void ShowSendMessage(QString, QString);
	void SetUserInfo(QString, QString, unsigned long long, unsigned long long);
	void ShareInfo(bool);

protected:
    void paintEvent(QPaintEvent *event);

	//重写QWiget的鼠标事件，移动窗口
    void mousePressEvent(QMouseEvent *);

    void mouseMoveEvent(QMouseEvent *);

    void mouseReleaseEvent(QMouseEvent *);

private:
	//初始化组件
	void InitWidget();
	//设置布局
    void InitLayout();
	//连接信号槽
    void InitSlot();
	//设置样式
    void setThisStyle();

private:
    QVBoxLayout* lpMainLayout;

    MyTitleBar* lpTitleBar;				//标题栏
    MyMenuBar* lpMenuBar;				//菜单栏
    MyPage* lpPage;						//浏览界面，可在文件浏览器，上传管理界面，下载管理界面之间切换

    MyPasteErrorHint* lpError;			//错误提示，显示两秒后消失
	MyFilePropertyHint* lpFileProperty;	//显示文件信息
	
	MyChatBar* lpChatBar;				//聊天工具栏
	MyMessageDialog* lpChat;			//聊天框

    QPoint last;

    bool isMoving;

    int timerHandle;

private slots:
	//与子部件信号连接的信号槽，调用controller的方法发送命令
    void startDownloadMission(QString);	
    void startUploadMission(QString);	
    void shareFile(QString, QString);			
    void deleteFile(QString);			
    void makeNewDir(QString);			
    void toUpperDir();					
    void toBeforeDir();					
    void refreshDir();					
    void searchFile(QString);			

	void backToHome();
    void enterDir(QString);				
    void renameFile(QString, QString);	
    void getProperty(QString);			
    void copy(QString);					
    void cut(QString);					
    void paste();						
	void send(QString, QString);		
	
	void showFileInfo(QString &name, QString &path, QString &date, QString &size, QString &type);

	//尚未实现
    void suspendD(int);
    void resumeD(int);
    void cancelD(int);
    void openDirD(int);					

    void suspendU(int);
    void resumeU(int);
    void cancelU(int);
    void openDirU(int);					

	void showChatDialog();

	void CleanAndClose();
	void ShowShare(bool);
};

#endif // MYMAINWINDOW_H
