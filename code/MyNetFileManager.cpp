#include "MyNetFileManager.h"
#include <sstream>

MyNetFileManager* MyNetFileManager::instance = 0;

MyNetFileManager::MyNetFileManager()
{
	copyBuf = 0;
	isRoot = true;
}


MyNetFileManager::~MyNetFileManager()
{
	std::map<std::string, MyNetFileBase*>::iterator iter;
	for (iter = files.begin(); iter != files.end(); ++iter) {
		Cleaner::Delete<MyNetFileBase*>(&(iter->second));
	}
	Cleaner::Delete<MyNetFileBase*>(&copyBuf);
	files.clear();
}

bool MyNetFileManager::Copy(std::string & fileName)
{
	std::map<std::string, MyNetFileBase*>::iterator hresult;
	if ((hresult = files.find(fileName)) == files.end()) {
		return false;
	}
	if (HasCopyFile()) {
		Cleaner::Delete<MyNetFileBase*>(&copyBuf);
	}
	copyBuf = new MyNetFileBase(*(hresult->second));
	return true;
}

bool MyNetFileManager::Paste()
{
	if (!HasCopyFile()) {
		return false;
	}
	MyNetFileBase* temp = new MyNetFileBase(*(copyBuf));
	temp->SetPath(GetCurrentDir());
	std::pair<std::map<std::string, MyNetFileBase*>::iterator, bool> ret;
	ret = files.insert(std::make_pair(temp->GetName(), temp));
	if (ret.second == false) {
		Cleaner::Delete<MyNetFileBase*>(&temp);
		return false;
	}
	return true;
}

bool MyNetFileManager::AddDirector(std::string &uId, std::string & name, std::string & date, unsigned long long size)
{
	MyNetFileDir* dir = new MyNetFileDir(uId, name, GetCurrentDir(), date, 0);
	if (dir == 0) {
		return false;
	}
	return AddFile(dir);
}

bool MyNetFileManager::AddNormFile(std::string &uId, std::string & name, std::string & date, unsigned long long size)
{
/*	std::string temp;
	for (int i = 0; i < path.size(); ++i) {
		temp += path.at(i) + "/";
	}
	temp.erase(temp.end() - 1);*/

	MyNetFileNorm* file = new MyNetFileNorm(uId, name, GetCurrentDir(), date, size);
	if (file == 0) {
		return false;
	}

	return AddFile(file);
}

bool MyNetFileManager::AddFile(MyNetFileBase *c)
{
	std::pair<std::map<std::string, MyNetFileBase*>::iterator, bool> Insert_Pair;
	Insert_Pair = files.insert(std::make_pair(c->GetName(), c));
	return Insert_Pair.second;
}

bool MyNetFileManager::DeleteNetFile(std::string & name)
{
	std::map<std::string, MyNetFileBase*>::iterator hresult;
	hresult = files.find(name);
	if (hresult == files.end()) {
		return false;
	}
	Cleaner::Delete<MyNetFileBase*>(&(hresult->second));
	files.erase(hresult);
	return true;
/*	MyNetFileBase* temp = 0;
	for (int i = 0; i < files.size(); ++i) {
		if (files.at(i)->GetName() == name) {
			temp = files.at(i);
			files.erase(files.begin() + i);
			Cleaner::Delete<MyNetFileBase*>(&temp);
			return true;
		}
	}
	return false;*/
}

bool MyNetFileManager::Rename(std::string & oldName, std::string & newName)
{
	std::map<std::string, MyNetFileBase*>::iterator hresult;
	hresult = files.find(oldName);
	if (hresult == files.end()) {
		return false;
	}
	hresult->second->SetName(newName);
	files.insert(std::make_pair(newName, hresult->second));
	files.erase(hresult);
	return true;
/*	for (int i = 0; i < files.size(); ++i) {
		if (files.at(i)->GetName() == oldName) {
			files.at(i)->SetName(newName);
			return true;
		}
	}
	return false;*/
}

bool MyNetFileManager::ToParentDir()
{
	if (IsRootDir()){
		return false;
	}
	path.erase(path.end() - 1);
	if (path.size() == 1) {
		GetToRoot();
	}
	return true;
}

bool MyNetFileManager::EnterDir(std::string &dirName)
{
	if (files.find(dirName) == files.end()) {
		return false;
	}
	path.push_back(dirName);
	if (IsRootDir()) {
		LeaveRoot();
	}
	return true;
}

void MyNetFileManager::Fresh()
{
	for (std::map<std::string, MyNetFileBase*>::iterator iter = files.begin();
		iter != files.end();
		++iter) {
		Cleaner::Delete<MyNetFileBase*>(&(iter->second));
	}
	files.clear();
}

//------------------------------------------------------------------
//		content of params:
//			0			1		2			3			4
//		-------------------------------------------------------
//		| file type | uID | file name | file date | file size |	......
//		-------------------------------------------------------
//------------------------------------------------------------------
bool MyNetFileManager::ReplaceContent(std::vector<std::string>& fileInfo)
{
	if (fileInfo.size() % 5 != 0) {
		return false;
	}

	Fresh();
	for (int i = 0; i < fileInfo.size() / 4; ++i) {
		if (fileInfo.at(i * 5) == "0") {
			AddNormFile(fileInfo.at(i * 5 + 1),							//uid
						fileInfo.at(i * 5 + 2),							//name
						fileInfo.at(i * 5 + 3),							//date
						stoull(fileInfo.at(i * 5 + 4)));				//size
		} else{
			AddDirector(fileInfo.at(i * 5 + 1),							//uid
						fileInfo.at(i * 5 + 2),							//name
						fileInfo.at(i * 5 + 3),							//date
						stoull(fileInfo.at(i * 5 + 4)));				//size
		}
	}
	return true;
}

std::string MyNetFileManager::GetCurrentDir()
{
	std::stringstream ss;
	
	for (int i = 0; i < path.size(); ++i) {
		ss << path.at(i) << "\\";
	}
	return ss.str();
}

MyNetFileManager * MyNetFileManager::Instance()
{
	if (instance == 0) {
		instance = new MyNetFileManager();
	}
	return instance;
}

void MyNetFileManager::Release()
{
	Cleaner::Delete<MyNetFileManager*>(&instance);
}

std::string MyNetFileManager::ToString()
{
	std::stringstream ss;
	ss << "path:\t";
	for (int i = 0; i < path.size(); ++i) {
		ss << path.at(i) << "\\";
	}
	ss << "\n";

	for (std::map<std::string, MyNetFileBase*>::iterator iter = files.begin();
		iter != files.end();
		++iter) {
		ss	<< iter->second->GetName() << "\t"
			<< iter->second->GetSize() << "\t"
			<< iter->second->GetDate() << "\t"
			<< iter->second->GetType() << "\t"
			<< iter->second->GetPath() << "\n";
	}
	return ss.str();
}
