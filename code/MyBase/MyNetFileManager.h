#pragma once

#ifndef MYNETFILEMANAGER_H_
#define MYNETFILEMANAGER_H_

#include "Cleaner.h"
#include "MyNetFileBase.h"
#include <map>
#include <vector>

class MyNetFileManager
{
private:
	void split(std::string &str, char flag, std::vector<std::string> &v);
private:
	static MyNetFileManager* instance;
	std::map<std::string, MyNetFileBase*> files;
	std::vector<std::string> path;					//the path of current director
	std::vector<std::string> prePath;

	bool isRoot;									//judge whether the path is root path
	bool hasPre;
	bool isCopy;

	MyNetFileBase* copyBuf;
public:
	MyNetFileManager();
	~MyNetFileManager();

	bool GetFileUID(std::string &name, std::string &uID);

	bool IsRootDir() {
		return path.size() == 1;
	}

	bool hasPreDir() {
		return hasPre;
	}

	#pragma region Copy manage
	std::string GetCopyFileUID() {
		if (copyBuf) {
			return copyBuf->GetUid();
		}
		return std::string("");
	}

	std::string GetCopyFileName() {
		if (copyBuf) {
			return copyBuf->GetName();
		}
		return std::string("");
	}

	bool Exist();

	bool IsCopy() {
		return isCopy;
	}

	bool HasCopyFile() {
		return copyBuf != 0;
	}

	//put specified file into copy buffer; 
	bool Copy(std::string &fileName);

	bool Cut(std::string &fileName);

	bool Paste();
	
	#pragma endregion

	bool AddDirector(std::string &uId, std::string &path, std::string &name, std::string &date, unsigned long long size, bool shared);
	
	bool AddNormFile(std::string &uId, std::string &path, std::string &name, std::string &date, unsigned long long size, bool shared);

	bool AddFile(MyNetFileBase*);

	bool DeleteNetFile(std::string &name);

	bool Rename(std::string &oldName, std::string &newName);	

	bool EnterDir(std::string &dirName);

	bool SetPath(std::vector<std::string> &path);

	//Clean the content
	void Fresh();

	//method used while back to the parent directory or enter an director,
	//method supose that path of this class has been changed
	bool ReplaceContent(std::vector<std::string> &fileInfo);	

	std::string GetCurrentDir();

	std::string GetUpperDir();

	std::string GetPreDir();

	std::string GetFilePath(std::string &name);

	void SaveCurrentPath();

	bool GetInfo(std::string &name, std::string &info);

	static MyNetFileManager* Instance();

	static void Release();
	//use for test
	std::string ToString();
};

#endif // !MYNETFILEMANAGER_h_
