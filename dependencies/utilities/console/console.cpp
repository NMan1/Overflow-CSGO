#include "console.hpp"

void console::initialize(const char* title) {
	AllocConsole();

	freopen_s((_iobuf * *)__acrt_iob_func(0), "conin$", "r", (_iobuf*)__acrt_iob_func(0));
	freopen_s((_iobuf * *)__acrt_iob_func(1), "conout$", "w", (_iobuf*)__acrt_iob_func(1));
	freopen_s((_iobuf * *)__acrt_iob_func(2), "conout$", "w", (_iobuf*)__acrt_iob_func(2));

	SetConsoleTitleA(title);
}

void console::release() {
	fclose((_iobuf*)__acrt_iob_func(0));
	fclose((_iobuf*)__acrt_iob_func(1));
	fclose((_iobuf*)__acrt_iob_func(2));

	HWND hw_ConsoleHwnd = GetConsoleWindow();     //Get the HWND of the console.
	FreeConsole();                                //Detach the console.
	PostMessageW(hw_ConsoleHwnd, WM_CLOSE, 0, 0); //Destroys the window.
}