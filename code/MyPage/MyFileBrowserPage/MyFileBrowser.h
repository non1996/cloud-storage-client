#ifndef MYFILEBROWSER_H
#define MYFILEBROWSER_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QListWidget>

//------------------------------------------------
//	文件浏览器类
//------------------------------------------------
class MyFileBrowser : public QWidget
{
    Q_OBJECT
private:
    const static int maxNumPerLine = 7;
public:
    explicit MyFileBrowser(QWidget *parent = 0);
    bool HasFileFocus();
    QString GetSeletedFileName();
    QString GetCopyFileName();
    bool IsCopy();
	bool CleanBrowser();
	void RenameFile(QString&, QString&);
	void DeleteNetFile(QString &);

private:
    void InitWidget();
    void InitCounter();
    void SetThisStyle();
    void ConnectSlot();

private:
    QVBoxLayout* lpMainLayout;
    QListWidget* lpListWidget;

    QString copyFilename;
    bool isCopy;
    int fileNum;
signals:
    void Open();
    void Delete();
    void Copy();
    void Cut();
    void Rename();
    void Property();
    void Paste();
    void NewDir();

public slots:
    void AddDirector(QString&);
	void AddFile(QString &);

private slots:
    void enterDir(QListWidgetItem*);

    void clickedOpen();
    void clickedDelete();
    void clickedCopy();
    void clickedCut();
    void clickedRename();
    void clickedProperty();
    void clickedPaste();
    void clickedNewDir();
};

#endif // MYFILEBROWSER_H
