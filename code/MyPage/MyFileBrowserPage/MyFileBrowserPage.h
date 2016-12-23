#ifndef MYFILEBROWSERPAGE_H
#define MYFILEBROWSERPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
//#include <QScrollArea>

class MyFBToolBar;
class MyDirInfoBar;
class MyFileBrowser;
class MyNewDirDialog;

//------------------------------------------
//	文件浏览器界面
//------------------------------------------
class MyFileBrowserPage : public QWidget
{
    Q_OBJECT
public:
    explicit MyFileBrowserPage(QWidget *parent = 0);

	void SetPath(QString &path);
	bool CleanBrowser();
	void AddFile(QString &name);
	void RenameFile(QString &oldName, QString &newName);
	void DeleteNetFile(QString &name);

private:
    void InitWidget();
    void InitLayout();
    void SetThisStyle();
    void ConnectSlot();
private:
    QVBoxLayout* lpMainLayout;

    MyFBToolBar* lpToolBar;		//文件工具栏
    MyDirInfoBar* lpDirBar;		//文件夹信息
    MyFileBrowser* lpBrowser;	//文件浏览器
    MyNewDirDialog* lpDialog;	//新建文件夹对话框

    QPalette* lpPal;

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

    void Open(QString);
    void Rename(QString, QString);
    void Property(QString);
    void Copy(QString);
    void Cut(QString);
    void Paste();

	void AddUploadBar(QString);
private slots:
    //与工具栏相关的信号槽
    void showDialog();
    void clickedUpload();
    void clickedDownload();
    void clickedShare();
    void clickedDelete();
    void clickedAdd();

    //与文件夹信息栏相关的信号槽
    void clickedBack();
    void clickedFront();
    void clickedRefresh();
	void clickedHome();
    void clickedSearch(QString);

    //与文件浏览器相关的信号槽
    void clickedOpen();
    void clickedRename();
    void clickedProperty();
    void clickedPaste();
    void clickedCopy();
    void clickedCut();
};

#endif // MYFILEBROWSERPAGE_H
