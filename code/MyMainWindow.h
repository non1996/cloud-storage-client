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
//	��������
//	
//-------------------------------------------
class MyMainWindow : public QFrame
{
    Q_OBJECT
public:
    MyMainWindow();

	//�ɿ����̵߳��øı����
public:					
	//�ı��ļ�������Ľ���
	bool SetPath(std::string &path);
	bool CleanBrowser();
	void AddFile(std::string &name);
	void Rename(std::string &oldName, std::string &newName);
	void DeleteNetFile(std::string &name);
	void AddUploadMission(std::string &s);

	//�ı��ϴ������������
	void SetUploadProgress(int i, float pro, unsigned int speed);
	void SetDownloadProgress(int i, float pro, unsigned int speed);
	void DownloadComplete(int i);
	void UploadComplete(int i);

	//Ϊ�Ի��������Ϣ
	void showMessage(std::string, std::string);
	void sendSuccess(std::string, std::string);
	
	//�����û���Ϣ��ʾ
	void SetUserInfo(std::string &name, std::string &url, unsigned long long currentV, unsigned long long totalV);

	//��ʾ������ʾ
	void showNetBrokenInfo();
	void showShareInfo(bool);
	void showInfo();

signals:
	//��Ϊ���̲߳���ֱ�Ӹı�Qwidget������˵������������󷢳�������Ӧ�ź��������ؼ�
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

	//��дQWiget������¼����ƶ�����
    void mousePressEvent(QMouseEvent *);

    void mouseMoveEvent(QMouseEvent *);

    void mouseReleaseEvent(QMouseEvent *);

private:
	//��ʼ�����
	void InitWidget();
	//���ò���
    void InitLayout();
	//�����źŲ�
    void InitSlot();
	//������ʽ
    void setThisStyle();

private:
    QVBoxLayout* lpMainLayout;

    MyTitleBar* lpTitleBar;				//������
    MyMenuBar* lpMenuBar;				//�˵���
    MyPage* lpPage;						//������棬�����ļ���������ϴ�������棬���ع������֮���л�

    MyPasteErrorHint* lpError;			//������ʾ����ʾ�������ʧ
	MyFilePropertyHint* lpFileProperty;	//��ʾ�ļ���Ϣ
	
	MyChatBar* lpChatBar;				//���칤����
	MyMessageDialog* lpChat;			//�����

    QPoint last;

    bool isMoving;

    int timerHandle;

private slots:
	//���Ӳ����ź����ӵ��źŲۣ�����controller�ķ�����������
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

	//��δʵ��
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
