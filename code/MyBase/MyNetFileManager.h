#pragma once

#ifndef MYNETFILEMANAGER_H_
#define MYNETFILEMANAGER_H_

#include "Cleaner.h"
#include "MyNetFileBase.h"
#include <map>
#include <vector>

//----------------------------------------------------------
//	文件管理器类，存储及管理网络文件信息
//----------------------------------------------------------
class MyNetFileManager
{
private:
	void split(std::string &str, char flag, std::vector<std::string> &v);

private:
	static MyNetFileManager* instance;				//单例模式
	std::map<std::string, MyNetFileBase*> files;	//以名字为键值存储文件信息
	std::vector<std::string> path;					//当前所处的网盘路径
	std::vector<std::string> prePath;				//上一个访问的路径

	bool isRoot;									//判断当前目录是否是根目录
	bool hasPre;									//判断之前是否访问过目录
	bool isCopy;									//判断是复制还是剪切

	MyNetFileBase* copyBuf;							//复制缓冲区

public:
	MyNetFileManager();
	~MyNetFileManager();

	bool IsRootDir() {
		return path.size() == 1;
	}

	bool hasPreDir() {
		return hasPre;
	}

	#pragma region Copy manage
	//获取复制文件的id
	std::string GetCopyFileUID() {
		if (copyBuf) {
			return copyBuf->GetUid();
		}
		return std::string("");
	}

	//获得复制的文件的名字
	std::string GetCopyFileName() {
		if (copyBuf) {
			return copyBuf->GetName();
		}
		return std::string("");
	}

	//判断是否有复制的文件
	bool Exist();

	bool IsCopy() {
		return isCopy;
	}

	bool HasCopyFile() {
		return copyBuf != 0;
	}

	//将文件以复制的形式存入复制缓冲
	bool Copy(std::string &fileName);

	//将文件以剪切的形式存入复制缓冲
	bool Cut(std::string &fileName);

	//粘贴，如果是剪切形式，则在粘贴后变为复制
	bool Paste();
	
	#pragma endregion

	//在当前路径新建文件夹
	bool AddDirector(std::string &uId, std::string &path, std::string &name, std::string &date, unsigned long long size, bool shared);
	
	//在当前路径新建文件
	bool AddNormFile(std::string &uId, std::string &path, std::string &name, std::string &date, unsigned long long size, bool shared);

	bool AddFile(MyNetFileBase*);

	//根据名字删除文件
	bool DeleteNetFile(std::string &name);

	//给文件重命名
	bool Rename(std::string &oldName, std::string &newName);	

	bool EnterDir(std::string &dirName);

	bool SetPath(std::vector<std::string> &path);

	//将当前文件信息全部清空，切换目录时使用
	void Fresh();

	//将新目录下的文件信息存入
	bool ReplaceContent(std::vector<std::string> &fileInfo);	

	//根据文件名获取文件ID
	bool GetFileUID(std::string &name, std::string &uID);

	//获取当前目录的路径，以‘/’为分隔符
	std::string GetCurrentDir();

	//获取上级目录的路径
	std::string GetUpperDir();

	//获取前一次访问的目录路径，调用前先判断
	std::string GetPreDir();

	//获取某个文件的文件目录
	std::string GetFilePath(std::string &name);

	void SaveCurrentPath();

	//获取某个文件全部信息
	bool GetInfo(std::string &name, std::string &info);

	static MyNetFileManager* Instance();

	static void Release();
};

#endif // !MYNETFILEMANAGER_h_
