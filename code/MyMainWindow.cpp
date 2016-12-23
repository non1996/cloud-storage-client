# pragma execution_character_set("utf-8")
#include "MyMainWindow.h"
#include "MyTitleBar.h"
#include "MyFilePropertyHint.h"
#include "MyMenuBar/MyMenuBar.h"
#include "MyPage/MyPage.h"
#include "MyPasteErrorHint.h"
#include "MyBase/MyController.h"
#include "MyBase/MyController.h"
#include "MyChat/MyChatBar.h"
#include "MyChat/MyMessageDialog.h"
#include "MyLogInDialog.h"

#include <QTimer>
#include <QPainter>
#include <QBitmap>
#include <fstream>
#include <QDebug>
#include <QDialog>
#include <QProcess>
#include <QDir>

MyMainWindow::MyMainWindow()
    :QFrame()
{
    isMoving = false;

    InitWidget();

    InitLayout();

    setThisStyle();

    InitSlot();
}

bool MyMainWindow::SetPath(std::string & path)
{
	lpPage->SetPath(QString::fromStdString(path));
	return true;
}

bool MyMainWindow::CleanBrowser()
{
	lpPage->CleanBrowser();
	return true;
}

void MyMainWindow::AddFile(std::string & name)
{
	lpPage->AddFile(QString::fromStdString(name));
}

void MyMainWindow::Rename(std::string & oldName, std::string & newName)
{
	lpPage->RenameFile(QString::fromStdString(oldName), QString::fromStdString(newName));
}

void MyMainWindow::DeleteNetFile(std::string & name)
{
	lpPage->DeleteNetFile(QString::fromStdString(name));
}

void MyMainWindow::AddUploadMission(std::string &s)
{
	lpPage->AddUploadMission(QString::fromStdString(s));
}

void MyMainWindow::SetUploadProgress(int i, float pro, unsigned int speed)
{
	emit SetUploadProgress_signal(i, pro, speed);
}

void MyMainWindow::SetDownloadProgress(int i, float pro, unsigned int speed)
{
	emit SetDownloadProgress_signal(i, pro, speed);
}

void MyMainWindow::DownloadComplete(int i)
{
	emit DownloadComplete_signal(i);
}

void MyMainWindow::UploadComplete(int i)
{
	emit UploadComplete_signal(i);
}

void MyMainWindow::showMessage(std::string c, std::string m)
{
	emit ShowMessage(QString::fromStdString(c), QString::fromStdString(m));
}

void MyMainWindow::sendSuccess(std::string c, std::string m)
{
	emit ShowSendMessage(QString::fromStdString(c), QString::fromStdString(m));
}

void MyMainWindow::SetUserInfo(std::string & name, std::string & url, unsigned long long currentV, unsigned long long totalV)
{
	emit SetUserInfo(	QString::fromStdString(name),
						QString::fromStdString(url),
						currentV,
						totalV);
}

void MyMainWindow::paintEvent(QPaintEvent *event)
{
    QBitmap bitmap(size());
    QPainter painter(&bitmap);

    painter.fillRect(rect(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(this->rect(), 10, 10);
    setMask(bitmap);
}

void MyMainWindow::mousePressEvent(QMouseEvent *e){
    if(e->y() <= 22 && e->x() <= 850){
        isMoving = true;
        last = e->globalPos();
    }
}

void MyMainWindow::mouseMoveEvent(QMouseEvent *e){
    if(isMoving == true){
        int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        last = e->globalPos();
        move(x()+dx,y()+dy);
    }
}

void MyMainWindow::mouseReleaseEvent(QMouseEvent *e){
    if(e->y() <= 30){
        int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        move(x()+dx, y()+dy);
        isMoving = false;
    }
}

void MyMainWindow::InitWidget()
{
    lpTitleBar = new MyTitleBar(this);
    lpMenuBar = new MyMenuBar(this);
    lpPage = new MyPage(this);
    lpError = new MyPasteErrorHint(this);
	lpFileProperty = new MyFilePropertyHint(this);
	lpFileProperty->hide();
	lpChatBar = new MyChatBar(this);
	lpChat = new MyMessageDialog(this);
	lpChat->hide();
	lpChat->setGeometry(600, 30, 330, 600);
}

void MyMainWindow::InitLayout()
{
    lpMainLayout = new QVBoxLayout(this);
    lpMainLayout->setMargin(2);
    lpMainLayout->setSpacing(0);
    lpMainLayout->addWidget(lpTitleBar);
    lpMainLayout->addWidget(lpMenuBar);
    lpMainLayout->addWidget(lpPage);
	lpMainLayout->addWidget(lpChatBar);
    this->setLayout(lpMainLayout);
}

void MyMainWindow::InitSlot()
{
	connect(lpTitleBar, SIGNAL(CloseWindow()), this, SLOT(CleanAndClose()));

    connect(lpMenuBar, SIGNAL(SwitchPage(int)), lpPage, SLOT(SetCurrentPage(int)));
	connect(this, SIGNAL(SetUserInfo(QString, QString, unsigned long long, unsigned long long)),
			lpMenuBar, SLOT(SetUserInfo(QString, QString, unsigned long long, unsigned long long)));

    connect(lpPage, SIGNAL(Download(QString)), this, SLOT(startDownloadMission(QString)));
    connect(lpPage, SIGNAL(Upload(QString)), this, SLOT(startUploadMission(QString)));
    connect(lpPage, SIGNAL(Share(QString, QString)), this, SLOT(shareFile(QString, QString)));
    connect(lpPage, SIGNAL(Delete(QString)), this, SLOT(deleteFile(QString)));
    connect(lpPage, SIGNAL(NewDir(QString)), this, SLOT(makeNewDir(QString)));

    connect(lpPage, SIGNAL(Back()), this, SLOT(toUpperDir()));
    connect(lpPage, SIGNAL(Front()), this, SLOT(toBeforeDir()));
    connect(lpPage, SIGNAL(Refresh()), this, SLOT(refreshDir()));
	connect(lpPage, SIGNAL(Home()), this, SLOT(backToHome()));
    connect(lpPage, SIGNAL(Search(QString)), this, SLOT(searchFile(QString)));

    connect(lpPage, SIGNAL(EnterDir(QString)), this, SLOT(enterDir(QString)));
    connect(lpPage, SIGNAL(Rename(QString,QString)), this, SLOT(renameFile(QString,QString)));
    connect(lpPage, SIGNAL(Property(QString)), this, SLOT(getProperty(QString)));
    connect(lpPage, SIGNAL(Copy(QString)), this, SLOT(copy(QString)));
    connect(lpPage, SIGNAL(Cut(QString)), this, SLOT(cut(QString)));
    connect(lpPage, SIGNAL(Paste()), this, SLOT(paste()));

    connect(lpPage, SIGNAL(SuspendD(int)), this, SLOT(suspendD(int)));
    connect(lpPage, SIGNAL(ResumeD(int)), this, SLOT(resumeD(int)));
    connect(lpPage, SIGNAL(CancelD(int)), this, SLOT(cancelD(int)));
    connect(lpPage, SIGNAL(OpenDirD(int)), this, SLOT(openDirD(int)));
    connect(lpPage, SIGNAL(SuspendU(int)), this, SLOT(suspendU(int)));
    connect(lpPage, SIGNAL(ResumeU(int)), this, SLOT(resumeU(int)));
    connect(lpPage, SIGNAL(CancelU(int)), this, SLOT(cancelU(int)));
    connect(lpPage, SIGNAL(OpenDirU(int)), this, SLOT(openDirU(int)));

	connect(this, SIGNAL(ShareInfo(bool)), this, SLOT(ShowShare(bool)));

	//改变页面
	connect(this, SIGNAL(SetUploadProgress_signal(int, float, unsigned int)), lpPage, SLOT(SetUploadProgress(int, float, unsigned int)));
	connect(this, SIGNAL(SetDownloadProgress_signal(int, float, unsigned int)), lpPage, SLOT(SetDownloadProgress(int, float, unsigned int)));
	connect(this, SIGNAL(UploadComplete_signal(int)), lpPage, SLOT(UploadComplete(int)));
	connect(this, SIGNAL(DownloadComplete_signal(int)), lpPage, SLOT(DownloadComplete(int)));

	//聊天框信号槽
	connect(lpChatBar, SIGNAL(ShowChat()), this, SLOT(showChatDialog()));
	connect(lpChat, SIGNAL(SendMessage(QString, QString)), this, SLOT(send(QString, QString)));
	connect(this, SIGNAL(ShowSendMessage(QString, QString)), lpChat, SLOT(AddSendMessage(QString, QString)));
	connect(this, SIGNAL(ShowMessage(QString, QString)), lpChat, SLOT(AddRecvMessage(QString, QString)));
}

void MyMainWindow::setThisStyle()
{
    QPalette bgpal = palette();
    bgpal.setColor(QPalette::Background, QColor(57, 63, 63));
    setPalette(bgpal);

    setMinimumWidth(950);
    setMinimumHeight(600);
    setWindowFlags(Qt::FramelessWindowHint);
}

void MyMainWindow::showNetBrokenInfo() 
{
	lpError->NetBroken();
	lpError->show();
	QTimer::singleShot(2000, lpError, SLOT(hide()));
}

void MyMainWindow::showShareInfo(bool ok)
{
	emit ShareInfo(ok);
}

void MyMainWindow::showInfo()
{
	lpError->show();
	QTimer::singleShot(2000, lpError, SLOT(hide()));
}

void MyMainWindow::startDownloadMission(QString name)
{
	MyController* c = MyController::Instance();
	std::string uID;
	if (!c->GetControl()->GetManager()->GetFileUID(name.toStdString(), uID)) {
		return;
	}
	if (!c->GetControl()->IsConnect()) {
		showNetBrokenInfo();
	}
	else{
		c->PushGetCommand(uID);
	}
}

void MyMainWindow::startUploadMission(QString name)
{
	int pos = name.lastIndexOf('/');
	QString fileName = name.mid(pos + 1, name.length() - pos);
	QString filePath = name.mid(0, pos + 1);

	MyController* c = MyController::Instance();
	if (!c->GetControl()->IsConnect()) {
		showNetBrokenInfo();
	}
	else {
		c->PushPutCommand(fileName.toStdString(), filePath.toStdString(), c->GetControl()->GetManager()->GetCurrentDir());
	}
}

void MyMainWindow::shareFile(QString name, QString pass)
{
	MyController* c = MyController::Instance();
	std::string uID;
	if (false == c->GetControl()->GetManager()->GetFileUID(name.toStdString(), uID)) {
		return;
	}

	if (!c->GetControl()->IsConnect()) {
		showNetBrokenInfo();
	}
	else {
		c->PushShareCommand(uID, std::string("0"), pass.toStdString());
	}
}

void MyMainWindow::deleteFile(QString name)
{
	MyController* c = MyController::Instance();
	std::string uID;
	if (false == c->GetControl()->GetManager()->GetFileUID(name.toStdString(), uID)) {
		return;
	}

	if (!c->GetControl()->IsConnect()) {
		showNetBrokenInfo();
	}
	else {
		c->PushDeleteCommand(uID, name.toStdString());
	}
}

void MyMainWindow::makeNewDir(QString name)
{
	MyController* c = MyController::Instance();

	if (!c->GetControl()->IsConnect()) {
		showNetBrokenInfo();
	}
	else {
		c->PushMkdirCommand(name.toStdString(), c->GetControl()->GetManager()->GetCurrentDir());
	}
}

void MyMainWindow::toUpperDir()
{
	MyController* c = MyController::Instance();
	if (c->GetControl()->GetManager()->IsRootDir()) {
		return;
	}
	if (!c->GetControl()->IsConnect()) {
		showNetBrokenInfo();
	}
	else {
		c->GetControl()->GetManager()->SaveCurrentPath();
		c->PushLsCommand(std::string("0"),
			std::string(""),
			c->GetControl()->GetManager()->GetUpperDir(),
			std::vector<std::string>());
	}
}

void MyMainWindow::toBeforeDir()
{
	MyController* c = MyController::Instance();
	if (!c->GetControl()->IsConnect()) {
		showNetBrokenInfo();
	}
	else {
		if (c->GetControl()->GetManager()->hasPreDir()) {
			c->PushLsCommand(std::string("0"),
								std::string(""),
								c->GetControl()->GetManager()->GetPreDir(),
								std::vector<std::string>());
		}
	}
}

void MyMainWindow::refreshDir()
{
	MyController* c = MyController::Instance();
	if (!c->GetControl()->IsConnect()) {
		showNetBrokenInfo();
	}
	else {
		c->PushLsCommand(	std::string("0"),
							std::string(""),
							c->GetControl()->GetManager()->GetCurrentDir(),
							std::vector<std::string>());
	}
}

void MyMainWindow::searchFile(QString name)
{
	MyController* c = MyController::Instance();
	std::vector<std::string> argv;
	argv.push_back(name.toStdString());
	
	if (!c->GetControl()->IsConnect()) {
		showNetBrokenInfo();
	}
	else {
		c->GetControl()->GetManager()->SaveCurrentPath();
		c->PushLsCommand(	std::string("1"),
							std::string(""),
							c->GetControl()->GetManager()->GetCurrentDir(),
							argv);
	}
}

void MyMainWindow::backToHome()
{
	MyController* c = MyController::Instance();
	c->GetControl()->GetManager()->SaveCurrentPath();

	if (!c->GetControl()->IsConnect()) {
		showNetBrokenInfo();
	}
	else {
		c->PushLsCommand(std::string("0"),
			std::string(""),
			std::string("/"),
			std::vector<std::string>());
	}
}

void MyMainWindow::enterDir(QString name)
{
	MyController* c = MyController::Instance();
	c->GetControl()->GetManager()->SaveCurrentPath();

	std::ofstream fout("fault.txt", std::ios_base::app);
	fout << c->GetControl()->IsConnect() << std::endl;
	fout.flush();
	fout.close();

	if (!c->GetControl()->IsLogIn() || !c->GetControl()->IsConnect()) {
		c->GetControl()->Restart();
		MyLogInDialog* dialog = new MyLogInDialog(this);
		if (dialog->exec() != QDialog::Accepted) {
			CleanAndClose();
		}
	}
	else {
		if (name.split('.').size() == 1) {
			std::string path = c->GetControl()->GetPath(name.toStdString());
			if (path.size() == 0) {
				return;
			}
			c->PushLsCommand(std::string("0"),
				std::string(""),
				path + name.toStdString() + "/",
				std::vector<std::string>());
		}
	}
}

void MyMainWindow::renameFile(QString oldn, QString newn)
{
	MyController* c = MyController::Instance();
	std::string uID;
	if (false == c->GetControl()->GetManager()->GetFileUID(oldn.toStdString(), uID)) {
		return;
	}
	if (!c->GetControl()->IsConnect()) {
		showNetBrokenInfo();
	}
	else {
		c->PushRenameCommand(uID,
			oldn.toStdString(),
			newn.toStdString(),
			c->GetControl()->GetManager()->GetCurrentDir());
	}
}

void MyMainWindow::getProperty(QString name)
{
	MyController* c = MyController::Instance();
	std::string info;
	QString infoQ;
	QStringList param;
	if (false == c->GetControl()->GetManager()->GetInfo(name.toStdString(), info)) {
		return;
	}
	else {
		param = QString::fromStdString(info).split('+');
		if (param.size() != 5) {
			return;
		}
		showFileInfo(param[0], param[1], param[2], param[3], param[4]);
	}
}

void MyMainWindow::copy(QString name)
{
	MyController* c = MyController::Instance();
	c->GetControl()->GetManager()->Copy(name.toStdString());
}

void MyMainWindow::cut(QString name)
{
	MyController* c = MyController::Instance();
	c->GetControl()->GetManager()->Cut(name.toStdString());
}

void MyMainWindow::paste()
{
	MyController* c = MyController::Instance();

	if (!c->GetControl()->GetManager()->HasCopyFile()) {
		lpError->ExistHint();
		lpError->show();
		QTimer::singleShot(2000, lpError, SLOT(hide()));
		return;
	}
	if (c->GetControl()->GetManager()->Exist()) {
		lpError->ExistHint();
		lpError->show();
		QTimer::singleShot(2000, lpError, SLOT(hide()));
		return;
	}

	if (!c->GetControl()->IsConnect()) {
		showNetBrokenInfo();
		return;
	}
	if (c->GetControl()->GetManager()->IsCopy()) {
		c->PushCopyCommand(c->GetControl()->GetManager()->GetCopyFileUID(), c->GetControl()->GetManager()->GetCurrentDir());
	}
	else {
		c->PushMoveCommand(	c->GetControl()->GetManager()->GetCopyFileUID(), 
							c->GetControl()->GetManager()->GetCopyFileName(),
							c->GetControl()->GetManager()->GetCurrentDir());
	}
}

void MyMainWindow::send(QString cID, QString content)
{
	MyController* c = MyController::Instance();
	c->PushSendCommand(cID.toStdString(), content.toStdString());
	std::ofstream fout("test1.txt", std::ios_base::ate);
	fout << cID.toStdString() << std::endl << content.toStdString();
	fout.flush();
	fout.close();
}

void MyMainWindow::showFileInfo(QString & name, QString & path, QString & date, QString & size, QString & type)
{
	lpFileProperty->setGeometry(300, 200, 200, 300);
	lpFileProperty->SetText(name, path, date, size, type);
	lpFileProperty->show();
}

void MyMainWindow::suspendD(int n)
{
    qDebug() << "suspend download thread " << n << endl;
}

void MyMainWindow::resumeD(int n)
{
    qDebug() << "resume download thread " << n << endl;
}

void MyMainWindow::cancelD(int n)
{
    qDebug() << "cancel download thread " << n << endl;
}

void MyMainWindow::openDirD(int n)
{
	QString path = "myDownload\\";
	path.replace("/", "\\");
	QProcess::startDetached("explorer " + path);
}

void MyMainWindow::suspendU(int n)
{
    qDebug() << "suspend upload thread " << n << endl;
}

void MyMainWindow::resumeU(int n)
{
    qDebug() << "resume upload thread " << n << endl;
}

void MyMainWindow::cancelU(int n)
{
    qDebug() << "cancel upload thread " << n << endl;
}

void MyMainWindow::openDirU(int n)
{
//	MyController* c = MyController::Instance();
//	showInfo(QString("%1").arg(n));
}

void MyMainWindow::showChatDialog()
{
	if (lpChat->isHidden()) {
		lpChat->show();
	}
	else {
		lpChat->hide();
	}
}

void MyMainWindow::CleanAndClose()
{
	MyController* c = MyController::Instance();
	c->Close();
	c->Release();
	close();
}

void MyMainWindow::ShowShare(bool ok)
{
	if (ok) {
		lpError->ShareOK();
	}
	else {
		lpError->ShareFalled();
	}
	showInfo();
}
