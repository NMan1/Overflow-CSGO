#pragma once
#include "../../dependencies/common_includes.hpp"

class c_view_setup;

namespace hooks {
	bool initialize();
	void release();

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		bool __fastcall hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}	

	namespace present {
		using fn = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
		long __stdcall hook(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region);
	}		

	namespace dme {
		using fn = void(__thiscall*)(void*, void*, void*, const model_render_info_t&, matrix3x4_t*);
		void __fastcall hook(void* thisptr, void* edx, void* ctx, void* state, const model_render_info_t& info, matrix3x4_t* custom_bone_to_world);
	}	

	namespace sceneend {
		using fn = void(__thiscall*)(void*);
		void __stdcall hook();
	}	

	namespace reset {
		using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters);
	}

	namespace lock_cursor {
		using fn = void(__thiscall*)(void*);
		void __stdcall hook();
	}

	namespace get_view_model {
		using fn = float(__thiscall*)();
		float __stdcall hook();
	}

	namespace overide_view {
		using fn = void(__thiscall*)(void* _this, c_view_setup* setup);
		void __stdcall hook(c_view_setup* vsView);
	}	
	
	namespace fsn {
		using fn = void(__thiscall*)(void* _this, int);
		void __stdcall hook(int frame_stage);
	}

	LRESULT  __stdcall					  WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	extern HWND                           hCSGOWindow; // CSGO window handle
	extern WNDPROC                        pOriginalWNDProc; // Original CSGO window proc
	extern bool							  initialized_drawManager;
}