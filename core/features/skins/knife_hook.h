#pragma once
#include "../../../dependencies/common_includes.hpp"
#include "../../../source-sdk/sdk.hpp"
#include <Windows.h>
#include <vector>
class c_knifehook {
public:
	void knife_animation() noexcept;
};

extern c_knifehook knife_hook;