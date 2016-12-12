#include "MyFileBrowser.h"
#include <QFile>
#include <QDebug>
#include "MyFileListWidget.h"

MyFileBrowser::MyFileBrowser(QWidget *parent) : QWidget(parent)
{
    isCopy = true;
    InitWidget();
    InitCounter();
    SetThisStyle();

    ConnectSlot();
}

bool MyFileBrowser::HasFileFocus()
{
    QList<QListWidgetItem*> list = lpListWidget->selectedItems();
    if(list.size() == 0){
        return false;
    }
    return true;
}

QString MyFileBrowser::GetSeletedFileName()
{
    QList<QListWidgetItem*> list = lpListWidget->selectedItems();
    return list[0]->text();
}

QString MyFileBrowser::GetCopyFileName()
{
    return copyFilename;
}

bool MyFileBrowser::IsCopy()
{
    return isCopy;
}

bool MyFileBrowser::CleanBrowser()
{
	while (lpListWidget->count() != 0) {
		QListWidgetItem* temp = lpListWidget->takeItem(0);
		delete temp;
	}
	return true;
}

void MyFileBrowser::RenameFile(QString &oldName, QString &newName)
{
	for (int i = 0; i < lpListWidget->count(); ++i) {
		if (lpListWidget->item(i)->text() == oldName) {
			lpListWidget->item(i)->setText(newName);
			break;
		}
	}
}

void MyFileBrowser::DeleteNetFile(QString &name)
{
	for (int i = 0; i < lpListWidget->count(); ++i) {
		if (lpListWidget->item(i)->text() == name) {
			QListWidgetItem* temp = lpListWidget->takeItem(i);
			delete temp;
			break;
		}
	}
}

void MyFileBrowser::InitWidget()
{
    lpListWidget = new MyFileListWidget(this);
    lpListWidget->setIconSize(QSize(100, 100));
    lpListWidget->setResizeMode(QListView::Adjust);
    lpListWidget->setViewMode(QListView::IconMode);
    lpListWidget->setMovement(QListView::Static);
    lpListWidget->setSpacing(10);
    lpListWidget->setGeometry(0, 0, 950, 410);
}

void MyFileBrowser::InitCounter()
{
    fileNum = 0;
}

void MyFileBrowser::SetThisStyle()
{
    setMinimumSize(950, 410);
}

void MyFileBrowser::ConnectSlot()
{
    connect(lpListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(enterDir(QListWidgetItem*)));

    connect(lpListWidget, SIGNAL(Open()), this, SLOT(clickedOpen()));
    connect(lpListWidget, SIGNAL(Delete()), this, SLOT(clickedDelete()));
    connect(lpListWidget, SIGNAL(Copy()), this, SLOT(clickedCopy()));
    connect(lpListWidget, SIGNAL(Cut()), this, SLOT(clickedCut()));
    connect(lpListWidget, SIGNAL(Rename()), this, SLOT(clickedRename()));
    connect(lpListWidget, SIGNAL(Property()), this, SLOT(clickedProperty()));
    connect(lpListWidget, SIGNAL(Paste()), this, SLOT(clickedPaste()));
    connect(lpListWidget, SIGNAL(NewDir()), this, SLOT(clickedNewDir()));
}

void MyFileBrowser::AddDirector(QString &name)
{
    QPixmap iconP;
    qDebug() << iconP.load("image/fileBrowser/director.png");
    QListWidgetItem* pItem = new QListWidgetItem(QIcon(iconP.scaled(QSize(80, 80))), name);
    pItem->setSizeHint(QSize(100, 100));
    lpListWidget->addItem(pItem);
}

void MyFileBrowser::AddFile(QString &name)
{
	QPixmap iconP;
	qDebug() << iconP.load("image/fileBrowser/director.png");
	QListWidgetItem* pItem = new QListWidgetItem(QIcon(iconP.scaled(QSize(80, 80))), name);
	pItem->setSizeHint(QSize(100, 100));
	lpListWidget->addItem(pItem);
}
/*
void MyFileBrowser::DeleteFile()
{
    QList<QListWidgetItem*> list = lpListWidget->selectedItems();
    if(list.size() == 0){
        return;
    }
    QListWidgetItem* temp = list[0];
    if(temp){
        int row = lpListWidget->row(temp);
        lpListWidget->takeItem(row);
        delete temp;
    }
}*/

void MyFileBrowser::enterDir(QListWidgetItem *t)
{
    emit Open();
}

void MyFileBrowser::clickedOpen()
{
    emit Open();
}

void MyFileBrowser::clickedDelete()
{
    emit Delete();
}

void MyFileBrowser::clickedCopy()
{
//    copyFilename = GetSeletedFileName();
//   isCopy = true;
    emit Copy();
}

void MyFileBrowser::clickedCut()
{
//    copyFilename = GetSeletedFileName();
//    isCopy = false;
    emit Cut();
}

void MyFileBrowser::clickedRename()
{
    emit Rename();
}

void MyFileBrowser::clickedProperty()
{
    emit Property();
}

void MyFileBrowser::clickedPaste()
{
    emit Paste();
}

void MyFileBrowser::clickedNewDir()
{
    emit NewDir();
}

