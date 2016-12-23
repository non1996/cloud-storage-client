#pragma once

#ifndef CLEANER_H_
#define CLEANER_H_

//-------------------------------------
//	模板函数，用于安全释放资源
//-------------------------------------
namespace Cleaner {
	template<typename T>
	void Release(T* ptr) {
		if ((*ptr) != 0) {
			(*ptr)->Release();
			(*ptr) = 0;
		}
	}

	template<typename T>
	void Delete(T* ptr) {
		if ((*ptr) != 0) {
			delete (*ptr);
			*ptr = 0;
		}
	}
};

#endif

