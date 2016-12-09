#pragma once

#include "MyController.h"
#include <iostream>
using namespace std;

MyController* controller = 0;

void split(std::string &str, char flag, std::vector<std::string> &v) {
	int begin = 0;
	for (int i = 0; i < str.size(); ++i) {
		if (str.at(i) == flag && i != begin) {
			v.push_back(str.substr(begin, i - begin));
			begin = i + 1;
		}
	}
	if (begin < str.size()) {
		v.push_back(str.substr(begin, v.size() - begin));
	}
}

void test(){
	using std::cin;
	using std::cout;
	using std::endl;

	controller = MyController::Instance();
	if (!controller->Init()) {
		cout << "controller init failed\n";
		return;
	}

	bool finish = false;
	std::string choise;
	std::vector<std::string> param;

	while (!finish) {
		cout << "input your choise:" << endl;
		getline(cin, choise);
		param.clear();
		
		split(choise, ' ', param);
		if (param.size() == 0) {
			std::cout << "wrong input, size = " << param.size() << std::endl;
			continue;
		}

		else if (param[0] == "ex") {
			finish = true;
			continue;
		}

		if (!controller->GetControl()->GetUser()->IsLogIn()) {
			cout << "you are not log in\n";
			if (param[0] == "log") {
				if (param.size() == 3) {
					controller->PushLogInCommand(std::string("forec@bupt.edu.cn"), std::string("TESTTHISPASSWORD"));
				}
			}
			continue;
		}

		if (param[0] == "get") {
			if (param.size() == 2) {
				controller->PushGetCommand(param[1]);//uID
			}	
			else {
				std::cout << "wrong input\n";
			}
		}else if (param[0] == "put") {
			if (param.size() == 4) {
//				controller->PushPutCommand(param[1], param[2], param[3]);	//文件名,文件路径，上传到的位置
				controller->PushPutCommand("tttt.rar", "D:\\", "/");
			}
			else {
				std::cout << "wrong input\n";
			}
		}else if(param[0] == "del") {				//√
			if (param.size() == 3) {
				controller->PushDeleteCommand(param[1], param[2]);
			}
			else {
				std::cout << "wrong input\n";
			}
		}else if(param[0] == "fork") {				//未完全实现
			if (param.size() == 4) {
				controller->PushForkCommand(param[1], param[2], param[3]);
			}
			else {
				std::cout << "wrong input\n";
			}
		}else if(param[0] == "ls") {
			if (param.size() >= 4) {
				std::vector<std::string> args(param.begin() + 4, param.end());
				controller->PushLsCommand(param[1], param[2], param[3], args);
			}
			else {
				std::cout << "wrong input\n";
			}
		}else if(param[0] == "mkdir") {
			if (param.size() == 3) {
				controller->PushMkdirCommand(param[1], param[2]);
			}
			else {
				std::cout << "wrong input\n";
			}
		}else if(param[0] == "cpy"){
			if (param.size() == 3) {
				controller->PushCopyCommand(param[1], param[2]);
			}
			else {
				std::cout << "wrong input\n";
			}
		}else if(param[0] == "mv"){
			if (param.size() == 4) {
				controller->PushMoveCommand(param[1], param[2], param[3]);
			}
			else {
				std::cout << "wrong input\n";
			}
		}else if (param[0] == "touch") {
			if (param.size() == 3) {
				controller->PushTouchCommand(param[1], param[2]);
			}
			else {
				std::cout << "wrong input\n";
			}
		}else if (param[0] == "rn") {
			if (param.size() == 5) {
				controller->PushRenameCommand(param[1], param[2], param[3], param[4]);
			}
			else {
				std::cout << "wrong input\n";
			}
		}else if(param[0] == "shr"){
			if (param.size() == 3) {
				controller->PushShareCommand(param[1], param[2]);
			}
			else {
				std::cout << "wrong input\n";
			}
		}else if(param[0] == "show") {
			std::cout << controller->GetControl()->GetManager()->ToString() << endl;
		}else {
			cout << "wrong input\n";
		}
	}

	controller->Release();
}