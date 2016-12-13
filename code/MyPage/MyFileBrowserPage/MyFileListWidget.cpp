# pragma execution_character_set("utf-8")
#include "MyFileListWidget.h"


MyFileListWidget::MyFileListWidget(QWidget* parent)
    :QListWidget(parent)
{
    InitAction();
    ConnectSlot();
}

void MyFileListWidget::InitAction()
{
    lpOpen = new QAction("打开", this);
    lpDelete = new QAction("删除", this);
    lpCopy = new QAction("复制", this);
    lpCut = new QAction("剪切", this);
    lpRename = new QAction("重命名", this);
    lpProperty = new QAction("属性", this);

    lpPaste = new QAction("paste", this);
    lpNewDir = new QAction("new director", this);
}

void MyFileListWidget::ConnectSlot()
{
    connect(lpOpen, SIGNAL(triggered(bool)), this, SLOT(clickedOpen()));
    connect(lpDelete, SIGNAL(triggered(bool)), this, SLOT(clickedDelete()));
    connect(lpCopy, SIGNAL(triggered(bool)), this, SLOT(clickedCopy()));
    connect(lpCut, SIGNAL(triggered(bool)), this, SLOT(clickedCut()));
    connect(lpRename, SIGNAL(triggered(bool)), this, SLOT(clickedRename()));
    connect(lpProperty, SIGNAL(triggered(bool)), this, SLOT(clickedProperty()));

    connect(lpPaste, SIGNAL(triggered(bool)), this, SLOT(clickedPaste()));
    connect(lpNewDir, SIGNAL(triggered(bool)), this, SLOT(clickedNewDir()));
}

void MyFileListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu* menu = new QMenu(this);
    menu->setStyleSheet("QMenu{background-color: white;}");
    if(this->itemAt(mapFromGlobal(QCursor::pos())) != 0){
        menu->addAction(lpOpen);
        menu->addAction(lpDelete);
        menu->addAction(lpCopy);
        menu->addAction(lpCut);
        menu->addAction(lpRename);
        menu->addAction(lpProperty);
    }else{
        menu->addAction(lpPaste);
        menu->addAction(lpNewDir);
    }
    menu->exec(QCursor::pos());
}

void MyFileListWidget::clickedOpen()
{
    emit Open();
}

void MyFileListWidget::clickedDelete()
{
    emit Delete();
}

void MyFileListWidget::clickedCopy()
{
    emit Copy();
}

void MyFileListWidget::clickedCut()
{
    emit Cut();
}

void MyFileListWidget::clickedRename()
{
    emit Rename();
}

void MyFileListWidget::clickedProperty()
{
    emit Property();
}

void MyFileListWidget::clickedPaste()
{
    emit Paste();
}

void MyFileListWidget::clickedNewDir()
{
    emit NewDir();
}

