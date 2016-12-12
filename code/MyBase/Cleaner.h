#pragma once

#ifndef CLEANER_H_
#define CLEANER_H_

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

