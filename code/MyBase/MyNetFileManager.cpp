#include "MyNetFileManager.h"
#include <sstream>
#include <iostream>
//#include "Split.h"

MyNetFileManager* MyNetFileManager::instance = 0;

void MyNetFileManager::split(std::string & str, char flag, std::vector<std::string>& v)
{
	int begin = 0;
	for (int i = 0; i < str.size(); ++i) {
		if (str.at(i) == flag && i != begin) {
			v.push_back(str.substr(begin, i - begin));
			begin = i + 1;
		}
		else if (str.at(i) == flag && i == begin) {
			begin = i + 1;
		}
	}
	if (begin < str.size()) {
		v.push_back(str.substr(begin, str.size() - begin));
	}
}

MyNetFileManager::MyNetFileManager()
{
	copyBuf = 0;
	isRoot = true;
	hasPre = false;
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

bool MyNetFileManager::GetFileUID(std::string & name, std::string & uID)
{
	std::map<std::string, MyNetFileBase*>::iterator hresult;
	if ((hresult = files.find(name)) == files.end()) {
		return false;
	}
	uID = hresult->second->GetUid();
	return true;
}

bool MyNetFileManager::Exist()
{
	if (!HasCopyFile()) {
		return false;
	}
	if (files.find(copyBuf->GetName()) == files.end()) {
		return false;
	}
	return true;
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
//	copyBuf = new MyNetFileBase(*(hresult->second));
	copyBuf = hresult->second->Copy();
	isCopy = true;
	return true;
}

bool MyNetFileManager::Cut(std::string & fileName)
{
	std::map<std::string, MyNetFileBase*>::iterator hresult;
	if ((hresult = files.find(fileName)) == files.end()) {
		return false;
	}
	if (HasCopyFile()) {
		Cleaner::Delete<MyNetFileBase*>(&copyBuf);
	}
//	copyBuf = new MyNetFileBase(*(hresult->second));
	copyBuf = hresult->second->Copy();
	isCopy = false;
	return true;
}

bool MyNetFileManager::Paste()
{
	if (!HasCopyFile()) {
		return false;
	}
//	MyNetFileBase* temp = new MyNetFileBase(*(copyBuf));
	MyNetFileBase* temp = copyBuf->Copy();
	temp->SetPath(GetCurrentDir());
	std::pair<std::map<std::string, MyNetFileBase*>::iterator, bool> ret;
	ret = files.insert(std::make_pair(temp->GetName(), temp));
	if (ret.second == false) {
		Cleaner::Delete<MyNetFileBase*>(&temp);
		return false;
	}
	return true;
}

bool MyNetFileManager::AddDirector(std::string &uId, std::string &path, std::string & name, std::string & date, unsigned long long size, bool shared)
{
	MyNetFileDir* dir = new MyNetFileDir(uId, path, name, date, size, shared);
	if (dir == 0) {
		return false;
	}
	return AddFile(dir);
}

bool MyNetFileManager::AddNormFile(std::string &uId, std::string &path, std::string & name, std::string & date, unsigned long long size, bool shared)
{
	MyNetFileNorm* file = new MyNetFileNorm(uId, path, name, date, size, shared);
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
}
/*
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
}*/

bool MyNetFileManager::EnterDir(std::string &dirName)
{
	if (files.find(dirName) == files.end()) {
		return false;
	}
	path.push_back(dirName);
	return true;
}

bool MyNetFileManager::SetPath(std::vector<std::string>& path)
{
	this->path = path;
	return true;
}

void MyNetFileManager::Fresh()
{
	if (files.empty()) {
		return;
	}
	for (std::map<std::string, MyNetFileBase*>::iterator iter = files.begin();
		iter != files.end();
		++iter) {
		Cleaner::Delete<MyNetFileBase*>(&(iter->second));
	}
	files.erase(files.begin(), files.end());
	files.clear();
}

bool MyNetFileManager::ReplaceContent(std::vector<std::string>& fileInfo)
{
	Fresh();
	for (int i = 1; i < fileInfo.size(); ++i) {
		std::vector<std::string> temp;
		bool share = false;
		split(fileInfo[i], '+', temp);

		if (temp.size() != 7) {
			std::cout << "文件信息数量不为7\n";
			continue;
		}
		if (temp[5] == "1") {
			share = true;
		}
		if (temp[6] == "DIR") {		
			AddDirector(temp[0], temp[1], temp[2], temp[3], std::stoull(temp[4]), share);
		}
		else {
			AddNormFile(temp[0], temp[1], temp[2], temp[3], std::stoull(temp[4]), share);
		}
	}
	return true;
}

std::string MyNetFileManager::GetCurrentDir()
{
	std::stringstream ss;
	for (int i = 0; i < path.size(); ++i) {
		ss << path.at(i) << "/";
	}
	return ss.str();
}

std::string MyNetFileManager::GetUpperDir()
{
	std::stringstream ss;
	for (int i = 0; i < path.size() - 1; ++i) {
		ss << path.at(i) << "/";
	}
	return ss.str();
}

std::string MyNetFileManager::GetPreDir()
{
	std::stringstream ss;
	for (int i = 0; i < prePath.size(); ++i) {
		ss << prePath.at(i) << "/";
	}
	return ss.str();
}

std::string MyNetFileManager::GetFilePath(std::string & name)
{
	std::map<std::string, MyNetFileBase*>::iterator hresult = files.find(name);
	if (hresult == files.end()) {
		return "";
	}
	return hresult->second->GetPath();
}

void MyNetFileManager::SaveCurrentPath()
{
	prePath = path;
	hasPre = true;
}

bool MyNetFileManager::GetInfo(std::string & name, std::string &info)
{
	std::map<std::string, MyNetFileBase*>::iterator hresult;
	hresult = files.find(name);
	if (hresult == files.end()) {
		return false;
	}
	info = hresult->second->toString();
	return true;
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
		ss << path.at(i) << "/";
	}
	ss << "\n";

	for (std::map<std::string, MyNetFileBase*>::iterator iter = files.begin();
		iter != files.end();
		++iter) {
		ss	<< iter->second->GetUid() << "\t"
			<< iter->second->GetPath() << "\t"
			<< iter->second->GetName() << "\t"
			<< iter->second->GetDate() << "\t"
			<< iter->second->GetSize() << "\t"
			<< iter->second->GetType() << "\n";
	}
	return ss.str();
}
