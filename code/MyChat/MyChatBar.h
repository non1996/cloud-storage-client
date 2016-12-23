#ifndef MYCHATBAR_H
#define MYCHATBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

//------------------------------------------------
//	���칤����
//------------------------------------------------
class MyChatBar: public QWidget
{
    Q_OBJECT
public:
    explicit MyChatBar(QWidget* parent = 0);
    void SetText();
    void CleanText();
private:
    QPushButton* lpShowChat;			//�򿪻��������������İ�ť
    QLabel* lpInfo;						//��ʾδ������Ϣ����
    QHBoxLayout* lpLayout;
signals:
    void ShowChat();

private slots:
    void clickedShowChat();
};

#endif // MYCHATBAR_H
