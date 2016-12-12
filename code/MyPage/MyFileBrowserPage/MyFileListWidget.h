#ifndef MYFILELISTWIDGET_H
#define MYFILELISTWIDGET_H

#include <QListWidget>
#include <QMenu>

class MyFileListWidget : public QListWidget
{
    Q_OBJECT
public:
    MyFileListWidget(QWidget* parent = 0);
    void InitAction();
    void ConnectSlot();

protected:
    void contextMenuEvent(QContextMenuEvent* event);

private:
    //over file clicked
    QAction* lpOpen;
    QAction* lpDelete;
    QAction* lpCopy;
    QAction* lpCut;
    QAction* lpRename;
    QAction* lpProperty;

    //other area
    QAction* lpPaste;
    QAction* lpNewDir;

signals:
    void Open();
    void Delete();
    void Copy();
    void Cut();
    void Rename();
    void Property();
    void Paste();
    void NewDir();

private slots:
    void clickedOpen();
    void clickedDelete();
    void clickedCopy();
    void clickedCut();
    void clickedRename();
    void clickedProperty();
    void clickedPaste();
    void clickedNewDir();
};

#endif // MYFILELISTWIDGET_H
