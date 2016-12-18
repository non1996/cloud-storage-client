# pragma execution_character_set("utf-8")
#include "MyFileBrowserPage.h"
#include "MyFBToolBar.h"
#include "MyDirInfoBar.h"
#include "MyFileBrowser.h"
#include "MyNewDirDialog.h"
#include "MyDeleteDialog.h"
#include <QFileDialog>
#include <QDebug>

MyFileBrowserPage::MyFileBrowserPage(QWidget *parent) : QWidget(parent)
{
    InitWidget();
    InitLayout();
    SetThisStyle();
    ConnectSlot();
}

void MyFileBrowserPage::SetPath(QString & path)
{
	lpDirBar->SetPath(path);
}

bool MyFileBrowserPage::CleanBrowser()
{
	lpBrowser->CleanBrowser();
	return true;
}

void MyFileBrowserPage::AddFile(QString & name)
{
	lpBrowser->AddFile(name);
}

void MyFileBrowserPage::RenameFile(QString & oldName, QString & newName)
{
	lpBrowser->RenameFile(oldName, newName);
}

void MyFileBrowserPage::DeleteNetFile(QString & name)
{
	lpBrowser->DeleteNetFile(name);
}

void MyFileBrowserPage::InitWidget()
{
    lpToolBar = new MyFBToolBar(this);
    lpDirBar = new MyDirInfoBar(this);
    lpBrowser = new MyFileBrowser(this);
    lpDialog = new MyNewDirDialog(this);
}

void MyFileBrowserPage::InitLayout()
{
    lpMainLayout = new QVBoxLayout(this);
    lpMainLayout->setMargin(0);
    lpMainLayout->setSpacing(0);

    lpMainLayout->addWidget(lpToolBar);
    lpMainLayout->addWidget(lpDirBar);
    lpMainLayout->addWidget(lpBrowser);
    setLayout(lpMainLayout);
}

void MyFileBrowserPage::SetThisStyle()
{
    setAutoFillBackground(true);
	QPalette bgpal = palette();
	bgpal.setColor(QPalette::Background, QColor(51, 51, 51));
	setPalette(bgpal);
}

void MyFileBrowserPage::ConnectSlot()
{
    connect(lpToolBar, SIGNAL(Upload()), this, SLOT(clickedUpload()));
    connect(lpToolBar, SIGNAL(Download()), this, SLOT(clickedDownload()));
    connect(lpToolBar, SIGNAL(Share()), this, SLOT(clickedShare()));
    connect(lpToolBar, SIGNAL(Delete()), this, SLOT(clickedDelete()));
    connect(lpToolBar, SIGNAL(NewDir()), this, SLOT(clickedAdd()));

    connect(lpDirBar, SIGNAL(Back()), this, SLOT(clickedBack()));
    connect(lpDirBar, SIGNAL(Front()), this, SLOT(clickedFront()));
    connect(lpDirBar, SIGNAL(Fresh()), this, SLOT(clickedRefresh()));
    connect(lpDirBar, SIGNAL(Search(QString)), this, SLOT(clickedSearch(QString)));

    connect(lpBrowser, SIGNAL(Open()), this, SLOT(clickedOpen()));
    connect(lpBrowser, SIGNAL(Delete()), this, SLOT(clickedDelete()));
    connect(lpBrowser, SIGNAL(Rename()), this, SLOT(clickedRename()));
    connect(lpBrowser, SIGNAL(Property()), this, SLOT(clickedProperty()));
    connect(lpBrowser, SIGNAL(Copy()), this, SLOT(clickedCopy()));
    connect(lpBrowser, SIGNAL(Cut()), this, SLOT(clickedCut()));
    connect(lpBrowser, SIGNAL(Paste()), this, SLOT(clickedPaste()));
    connect(lpBrowser, SIGNAL(NewDir()), this, SLOT(clickedAdd()));

}

void MyFileBrowserPage::showDialog()
{
    lpDialog->Clean();
    lpDialog->exec();
}

void MyFileBrowserPage::clickedUpload()
{
    QStringList fileNames;
    QFileDialog* lpSelect = new QFileDialog(this);
    lpSelect->setFileMode(QFileDialog::ExistingFile);
    lpSelect->setViewMode(QFileDialog::Detail);

    if(lpSelect->exec()){
        fileNames = lpSelect->selectedFiles();
		if (fileNames.isEmpty()) {
			return;
		}
        QString filePath = *(fileNames.begin());
        QString fileName;
        int pos = filePath.lastIndexOf('/');
		fileName = filePath.mid(pos + 1, filePath.length() - pos);
		emit AddUploadBar(fileName);
		emit Upload(filePath);
    }
}

void MyFileBrowserPage::clickedDownload()
{
    if(!lpBrowser->HasFileFocus()){
        return ;
    }
    QString name = lpBrowser->GetSeletedFileName();
    emit Download(name);
}

void MyFileBrowserPage::clickedShare()
{
    if(!lpBrowser->HasFileFocus()){
        return ;
    }
    QString name = lpBrowser->GetSeletedFileName();
    emit Share(name);
}

void MyFileBrowserPage::clickedDelete()
{
    if(!lpBrowser->HasFileFocus()){
        return ;
    }
    QString name = lpBrowser->GetSeletedFileName();
    MyDeleteDialog* dialog = new MyDeleteDialog(this);
    dialog->SetHint(name);
    if(dialog->exec() == QDialog::Accepted){
        emit Delete(name);
    }
    delete dialog;
}

void MyFileBrowserPage::clickedAdd()
{
    lpDialog->Clean();
    if(lpDialog->exec() == QDialog::Accepted){
//        lpBrowser->AddDirector(lpDialog->GetContent());
        emit NewDir(lpDialog->GetContent());
    }
}

void MyFileBrowserPage::clickedBack()
{
    emit Back();
}

void MyFileBrowserPage::clickedFront()
{
    emit Front();
}

void MyFileBrowserPage::clickedRefresh()
{
    emit Refresh();
}

void MyFileBrowserPage::clickedSearch(QString name)
{
    emit Search(name);
}

void MyFileBrowserPage::clickedOpen()
{
    QString name = lpBrowser->GetSeletedFileName();
    emit Open(name);
}

void MyFileBrowserPage::clickedRename()
{
    MyNewDirDialog* renamedialog = new MyNewDirDialog(this);
    renamedialog->SetHint("ÊäÈëÐÂÃû×Ö");
    renamedialog->SetEditText(lpBrowser->GetSeletedFileName());
    if(renamedialog->exec() == QDialog::Accepted){
		QString temp = renamedialog->GetContent();
        emit Rename(lpBrowser->GetSeletedFileName(), temp);
    }
}

void MyFileBrowserPage::clickedProperty()
{
    QString name = lpBrowser->GetSeletedFileName();
    emit Property(name);
}

void MyFileBrowserPage::clickedPaste()
{
    emit Paste();
}

void MyFileBrowserPage::clickedCopy()
{
    emit Copy(lpBrowser->GetSeletedFileName());
}

void MyFileBrowserPage::clickedCut()
{
    emit Cut(lpBrowser->GetSeletedFileName());
}
