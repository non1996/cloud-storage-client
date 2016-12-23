#pragma once

#ifndef MYNETFILEMANAGER_H_
#define MYNETFILEMANAGER_H_

#include "Cleaner.h"
#include "MyNetFileBase.h"
#include <map>
#include <vector>

//----------------------------------------------------------
//	�ļ��������࣬�洢�����������ļ���Ϣ
//----------------------------------------------------------
class MyNetFileManager
{
private:
	void split(std::string &str, char flag, std::vector<std::string> &v);

private:
	static MyNetFileManager* instance;				//����ģʽ
	std::map<std::string, MyNetFileBase*> files;	//������Ϊ��ֵ�洢�ļ���Ϣ
	std::vector<std::string> path;					//��ǰ����������·��
	std::vector<std::string> prePath;				//��һ�����ʵ�·��

	bool isRoot;									//�жϵ�ǰĿ¼�Ƿ��Ǹ�Ŀ¼
	bool hasPre;									//�ж�֮ǰ�Ƿ���ʹ�Ŀ¼
	bool isCopy;									//�ж��Ǹ��ƻ��Ǽ���

	MyNetFileBase* copyBuf;							//���ƻ�����

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
	//��ȡ�����ļ���id
	std::string GetCopyFileUID() {
		if (copyBuf) {
			return copyBuf->GetUid();
		}
		return std::string("");
	}

	//��ø��Ƶ��ļ�������
	std::string GetCopyFileName() {
		if (copyBuf) {
			return copyBuf->GetName();
		}
		return std::string("");
	}

	//�ж��Ƿ��и��Ƶ��ļ�
	bool Exist();

	bool IsCopy() {
		return isCopy;
	}

	bool HasCopyFile() {
		return copyBuf != 0;
	}

	//���ļ��Ը��Ƶ���ʽ���븴�ƻ���
	bool Copy(std::string &fileName);

	//���ļ��Լ��е���ʽ���븴�ƻ���
	bool Cut(std::string &fileName);

	//ճ��������Ǽ�����ʽ������ճ�����Ϊ����
	bool Paste();
	
	#pragma endregion

	//�ڵ�ǰ·���½��ļ���
	bool AddDirector(std::string &uId, std::string &path, std::string &name, std::string &date, unsigned long long size, bool shared);
	
	//�ڵ�ǰ·���½��ļ�
	bool AddNormFile(std::string &uId, std::string &path, std::string &name, std::string &date, unsigned long long size, bool shared);

	bool AddFile(MyNetFileBase*);

	//��������ɾ���ļ�
	bool DeleteNetFile(std::string &name);

	//���ļ�������
	bool Rename(std::string &oldName, std::string &newName);	

	bool EnterDir(std::string &dirName);

	bool SetPath(std::vector<std::string> &path);

	//����ǰ�ļ���Ϣȫ����գ��л�Ŀ¼ʱʹ��
	void Fresh();

	//����Ŀ¼�µ��ļ���Ϣ����
	bool ReplaceContent(std::vector<std::string> &fileInfo);	

	//�����ļ�����ȡ�ļ�ID
	bool GetFileUID(std::string &name, std::string &uID);

	//��ȡ��ǰĿ¼��·�����ԡ�/��Ϊ�ָ���
	std::string GetCurrentDir();

	//��ȡ�ϼ�Ŀ¼��·��
	std::string GetUpperDir();

	//��ȡǰһ�η��ʵ�Ŀ¼·��������ǰ���ж�
	std::string GetPreDir();

	//��ȡĳ���ļ����ļ�Ŀ¼
	std::string GetFilePath(std::string &name);

	void SaveCurrentPath();

	//��ȡĳ���ļ�ȫ����Ϣ
	bool GetInfo(std::string &name, std::string &info);

	static MyNetFileManager* Instance();

	static void Release();
};

#endif // !MYNETFILEMANAGER_h_
