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

	template <size_t index, typename ret, class ... args_>
	__forceinline ret get_vfunc(void* thisptr, args_... args) 
	{
		using fn = ret(__thiscall*)(void*, args_...);

		auto fn_ptr = (fn)(*(uintptr_t**)thisptr)[index];
		return fn_ptr(thisptr, args...);
	}

	std::uint8_t* pattern_scan(void* module, const char* signature);
	void force_update();
}