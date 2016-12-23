#pragma once

#ifndef SPLIT_H_
#define SPLIT_H_

#include <string>
#include <vector>

//以flag字符将字符串划分
namespace split {
	void split(std::string &str, char flag, std::vector<std::string> &v) {
		unsigned int begin = 0;
		for (unsigned int i = 0; i < str.size(); ++i) {
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
};

#endif // !SPLIT_H_
