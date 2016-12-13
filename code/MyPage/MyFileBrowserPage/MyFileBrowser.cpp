# pragma execution_character_set("utf-8")
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
    lpListWidget->setIconSize(QSize(80, 80));
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
	lpListWidget->setStyleSheet("QListWidget{background-color:rgb(51, 51, 51);"
		"color:rgb(217, 217, 217);"
		"border-style: inset;"
		"border-width: 2px;"
		"border-color: rgb(45, 45, 45)}"
		"QListWidget::item:hover{show-decoration-selected:0;background:rgb(80, 80, 80);}"
		"QListWidget::item:selected{background:rgb(80, 80, 80);padding:0px;margin:0px;}");
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
	QStringList sections = name.split('.');
	if (sections.size() == 1) {
		iconP.load("image/fileBrowser/director.png");
	}
	else if (sections.back() == "txt") {
		iconP.load("image/fileBrowser/txt.png");
	}
	else if (sections.back() == "doc" || sections.back() == "docx") {
		iconP.load("image/fileBrowser/doc.png");
	}
	else if (sections.back() == "xls" || sections.back() == "xlsx") {
		iconP.load("image/fileBrowser/excel.png");
	}
	else if (sections.back() == "html") {
		iconP.load("image/fileBrowser/html.png");
	}
	else if (sections.back() == "rar" || 
			 sections.back() == "7z" || 
			 sections.back() == "zip") {
		iconP.load("image/fileBrowser/compress.png");
	}
	else if (sections.back() == "bmp" ||
		sections.back() == "gif" ||
		sections.back() == "jpg" ||
		sections.back() == "jpeg" ||
		sections.back() == "png" || 
		sections.back() == "svg" || 
		sections.back() == "tga") {
		iconP.load("image/fileBrowser/photo.png");
	}
	else if (sections.back() == "mp3" ||
		sections.back() == "wav" ||
		sections.back() == "ape" ||
		sections.back() == "acc" ||
		sections.back() == "ogg" ||
		sections.back() == "wma") {
		iconP.load("image/fileBrowser/music.png");
	}
	else if (sections.back() == "rm" ||
		sections.back() == "rmvb" ||
		sections.back() == "wmv" ||
		sections.back() == "avi" ||
		sections.back() == "mp4" ||
		sections.back() == "3gp" ||
		sections.back() == "mkv") {
		iconP.load("image/fileBrowser/video.png");
	}
	else {
		iconP.load("image/fileBrowser/file.png");
	}
	QListWidgetItem* pItem = new QListWidgetItem(QIcon(iconP.scaled(QSize(60, 60))), name);
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

