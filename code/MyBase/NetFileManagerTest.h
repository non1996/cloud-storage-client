#pragma once

#include "MyNetFileManager.h"
#include <iostream>
using namespace std;

MyNetFileManager* manager = 0;
std::vector<std::string> contentOfMyPicture = {"file", "old driver1.jpg", "", "2016/1/1", "123456",
											   "file", "old driver2.jpg", "", "2016/1/1", "654321",
											   "file", "old driver3.jpg", "", "2016/1/1", "456456"};
std::vector<std::string> contentOfRoot = {	"dir", "MyPicture", "", "2016/11/9", "0",
											"dir", "MyFile", "", "2016/11/9", "0",
											"dir", "MySource", "", "2016/11/9", "0" };

void Test() {
	manager = new MyNetFileManager;

	manager->SetRootDir(std::string("MyDocument"));
	cout << manager->ToString();

	cout << "is current path root: " << manager->IsRootDir() << endl;

	manager->AddDirector(std::string("MyPicture"), std::string("2016/11/9"));
	manager->AddDirector(std::string("MyFile"), std::string("2016/11/9"));
	manager->AddDirector(std::string("MySource"), std::string("2016/11/9"));
	manager->AddNormFile(std::string("fuck you.cpp"), std::string("2016/11/9"), 1024);
	cout << endl;
	cout << manager->ToString();

	manager->DeleteNetFile(std::string("fuck you.cpp"));
	cout << endl;
	cout << manager->ToString();

	manager->Rename(std::string("MyPicture"), std::string("MyNetPicture"));
	cout << endl;
	cout << manager->ToString();

	if (manager->EnterDir(std::string("MyNetPicture"))) {
		manager->ReplaceContent(contentOfMyPicture);
	}

	cout << endl;
	cout << manager->ToString();

	if (manager->ToParentDir()) {
		manager->ReplaceContent(contentOfRoot);
	}
	cout << endl;
	cout << manager->ToString();
}