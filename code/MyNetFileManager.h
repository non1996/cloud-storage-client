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
	static MyNetFileManager* instance;

//	std::vector<MyNetFileBase*> files;	//contain file item of current director
	std::map<std::string, MyNetFileBase*> files;
	std::vector<std::string> path;					//the path of current director

	bool isRoot;									//judge whether the path is root path

	MyNetFileBase* copyBuf;
public:
	MyNetFileManager();
	~MyNetFileManager();

	void SetRootDir(std::string &dirName) {
		if (path.size() == 0) {
			path.push_back(dirName);
		}
		path.at(0) = dirName;
	}

	bool IsRootDir() {
		return isRoot;
	}

	void GetToRoot() {
		isRoot = true;
	}

	void LeaveRoot() {
		isRoot = false;
	}

	#pragma region Copy manage

	bool HasCopyFile() {
		return copyBuf != 0;
	}

	//put specified file into copy buffer; 
	bool Copy(std::string &fileName);

	bool Paste();
	
	#pragma endregion

	bool AddDirector(std::string &uId, std::string &name, std::string &date, unsigned long long size);
	
	bool AddNormFile(std::string &uId, std::string &name, std::string &date, unsigned long long size);

	bool AddFile(MyNetFileBase*);

	bool DeleteNetFile(std::string &name);

	bool Rename(std::string &oldName, std::string &newName);

	bool ToParentDir();			

	bool EnterDir(std::string &dirName);

	//Clean the content
	void Fresh();

	//method used while back to the parent directory or enter an director,
	//method supose that path of this class has been changed
	bool ReplaceContent(std::vector<std::string> &fileInfo);	

	std::string GetCurrentDir();

	static MyNetFileManager* Instance();

	static void Release();

	//use for test
	std::string ToString();
};

#endif // !MYNETFILEMANAGER_h_
