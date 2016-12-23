#ifndef MYPAGE_H
#define MYPAGE_H

#include <QWidget>
#include <QStackedLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

class MyFileBrowserPage;
class MyMissionPage;

//---------------------------------------------------
//	维护三个页面，并控制它们切换
//---------------------------------------------------
class MyPage : public QWidget
{
    Q_OBJECT
public:
    explicit MyPage(QWidget *parent = 0);

	void SetPath(QString &p);
	bool CleanBrowser();
	void AddFile(QString &name);
	void RenameFile(QString &oldName, QString &newName);
	void DeleteNetFile(QString &name);

	void AddUploadMission(QString &);
signals:
	void SetUploadProgress_signal(int i, float pro, unsigned int speed);
	void SetDownloadProgress_signal(int i, float pro, unsigned int speed);
	void DownloadComplete_signal(int);
	void UploadComplete_signal(int);

public slots:
	void SetUploadProgress(int i, float pro, unsigned int speed);
	void SetDownloadProgress(int i, float pro, unsigned int speed);
	void DownloadComplete(int i);
	void UploadComplete(int i);

private:
    void InitWidget();

    void InitLayOut();

    void ConnectSlot();

private:
    QStackedLayout* lpStackLayout;		//用于切换页面的布局
    QVBoxLayout* lpMainLayout;

    MyFileBrowserPage* lpFileBrowser;	//文件浏览器界面
    MyMissionPage* lpDownload;			//下载界面
    MyMissionPage* lpUpload;			//上传界面

signals:
    void Download(QString);
    void Upload(QString);
    void Share(QString, QString);
    void Delete(QString);
    void NewDir(QString);
    void Back();
    void Front();
    void Refresh();
	void Home();
    void Search(QString);

    void EnterDir(QString);
    void Rename(QString, QString);
    void Property(QString);
    void Copy(QString);
    void Cut(QString);
    void Paste();

    void SuspendD(int);
    void ResumeD(int);
    void CancelD(int);
    void OpenDirD(int);
    void SuspendU(int);
    void ResumeU(int);
    void CancelU(int);
    void OpenDirU(int);

public slots:
    void SetCurrentPage(int);

private slots:
    void sendDownloadSignal(QString);
    void sendUploadSignal(QString);
    void sendShareSignal(QString, QString);
    void sendDeleteSignal(QString);
    void sendNewDirSignal(QString);
    void sendBackSignal();
    void sendFrontSignal();
    void sendRefreshSignal();
	void sendHomeSignal();
    void sendSearchSignal(QString);

    void sendEnterDirSignal(QString);
    void SendRenameSignal(QString, QString);
    void SendPropertySignal(QString);
    void SendCopySignal(QString);
    void SendCutSignal(QString);
    void SendPasteSignal();

    void sendSuspendD(int);
    void sendResumeD(int);
    void sendCancelD(int);
    void sendOpenDirD(int);

    void sendSuspendU(int);
    void sendResumeU(int);
    void sendCancelU(int);
    void sendOpenDirU(int);
};

#endif // MYPAGE_H
