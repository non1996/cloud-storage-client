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

    MyFBToolBar* lpToolBar;
    MyDirInfoBar* lpDirBar;
    MyFileBrowser* lpBrowser;
    MyNewDirDialog* lpDialog;

    QPalette* lpPal;

signals:
    void Download(QString);
    void Upload(QString);
    void Share(QString);
    void Delete(QString);
    void NewDir(QString);

    void Back();
    void Front();
    void Refresh();
    void Search(QString);

    void Open(QString);
    //oldname, newname
    void Rename(QString, QString);
    void Property(QString);
    void Copy(QString);
    void Cut(QString);
    void Paste();

	void AddUploadBar(QString);
private slots:
    //toolbar
    void showDialog();
    void clickedUpload();
    void clickedDownload();
    void clickedShare();
    void clickedDelete();
    void clickedAdd();

    //dirInfoBar
    void clickedBack();
    void clickedFront();
    void clickedRefresh();
    void clickedSearch(QString);

    //filebrowser
    void clickedOpen();
    void clickedRename();
    void clickedProperty();
    void clickedPaste();
    void clickedCopy();
    void clickedCut();
};

#endif // MYFILEBROWSERPAGE_H
