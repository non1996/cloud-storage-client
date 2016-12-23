#ifndef MYCHATBAR_H
#define MYCHATBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

//------------------------------------------------
//	聊天工具栏
//------------------------------------------------
class MyChatBar: public QWidget
{
    Q_OBJECT
public:
    explicit MyChatBar(QWidget* parent = 0);
    void SetText();
    void CleanText();
private:
    QPushButton* lpShowChat;			//打开或隐藏聊天输入框的按钮
    QLabel* lpInfo;						//显示未看的消息数量
    QHBoxLayout* lpLayout;
signals:
    void ShowChat();

private slots:
    void clickedShowChat();
};

#endif // MYCHATBAR_H
