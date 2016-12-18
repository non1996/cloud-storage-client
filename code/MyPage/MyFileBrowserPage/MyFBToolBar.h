
#ifndef MYFBTOOLBAR_H
#define MYFBTOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

class MyToolButton;
class MyFBToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit MyFBToolBar(QWidget *parent = 0);

//    void paintEvent(QPaintEvent *e);
private:
    void createWidget();

    void createLayout();

    void setToolBarStyle();

    void setButtonStyle();

    void setSlotConnect();

private:
    QHBoxLayout* lpLayout;

    QPushButton* lpUpload;
    QPushButton* lpDownload;
    QPushButton* lpShare;
    QPushButton* lpDelete;
    QPushButton* lpNewDir;

    QPalette* lpPal;

	void setIcon(QPushButton* b, QString path);

signals:
    void Upload();
    void Download();
    void Share();
    void Delete();
    void NewDir();

private slots:
    void clickedUpload();
    void clickedDownload();
    void clickedShare();
    void clickedDelete();
    void clickedNewDir();

};

#endif // MYTOOLBAR_H
