#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"

namespace utilities {
	template<typename FuncType>
	__forceinline static FuncType call_virtual(void* ppClass, int index) {
		int* pVTable = *(int**)ppClass;
		int dwAddress = pVTable[index];
		return (FuncType)(dwAddress);
	}

	template <typename t>
	static t get_virtual(void* class_pointer, size_t index) {
		return (*(t**)class_pointer)[index];
	}


	std::uint8_t* pattern_scan(void* module, const char* signature);
}