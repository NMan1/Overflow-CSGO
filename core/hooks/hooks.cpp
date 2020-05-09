#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../../dependencies/utilities/settings.h"

hooks::present::fn			present_original		    = nullptr;
hooks::reset::fn			reset_original				= nullptr;
hooks::create_move::fn		create_move_original		= nullptr;
hooks::paint_traverse::fn	paint_traverse_original		= nullptr;
hooks::dme::fn				dme_original				= nullptr;
hooks::sceneend::fn			sceneend_original			= nullptr;
hooks::lock_cursor::fn		lock_cursor_original		= nullptr;
hooks::get_view_model::fn	get_view_model_original		= nullptr;
hooks::overide_view::fn		overide_view_original		= nullptr;
hooks::fsn::fn				fsn_original				= nullptr;

uint8_t*					present_address				= nullptr;
uint8_t*					reset_address				= nullptr;

HWND						hooks::hCSGOWindow			= nullptr; 
WNDPROC						hooks::pOriginalWNDProc		= nullptr;
bool						hooks::initialized_drawManager = false;
color						watermark					= (255, 59, 59);

unsigned int get_virtual(void* class_, unsigned int index) { return (unsigned int)(*(int**)class_)[index]; }
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

bool hooks::initialize() 
{
	// Get window handle
	while (!(hooks::hCSGOWindow = FindWindowA("Valve001", nullptr)))
	{
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(50ms);
	}

	if (hooks::hCSGOWindow)        // Hook WNDProc to capture mouse / keyboard input
		hooks::pOriginalWNDProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hooks::hCSGOWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::WndProc)));
	else
	{
		throw std::runtime_error("failed to hook wndproc.");
		return false;
	}

	auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	auto get_view_model_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 35));
	auto get_overide_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 18));
	auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	auto dme_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));
	auto sceneend_target = reinterpret_cast<void*>(get_virtual(interfaces::render_view, 9));
	auto lock_cursor_target = reinterpret_cast<void*>(get_virtual(interfaces::surface, 67));
	auto in_key_event_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 21));
	auto fsn_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	auto present_address = utilities::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 DB") + 0x2;
	auto reset_address = utilities::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 FF 78 18") + 0x2;

	if (!present_address || !reset_address)
	{
		throw std::runtime_error("failed to aquire present address");
		return false;
	}

	if (MH_Initialize() != MH_OK) {
		throw std::runtime_error("failed to initialize MH_Initialize.");
		return false;
	}

	if (MH_CreateHook(**reinterpret_cast<void***>(present_address), &present::hook, reinterpret_cast<void**>(&present_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize present_address. (outdated index?)");
		return false;
	}		
	
	if (MH_CreateHook(**reinterpret_cast<void***>(reset_address), &reset::hook, reinterpret_cast<void**>(&reset_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize reset_address. (outdated index?)");
		return false;
	}	

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");
		return false;
	}	

	if (MH_CreateHook(dme_target, &dme::hook, reinterpret_cast<void**>(&dme_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize dme. (outdated index?)");
		return false;
	}	

	if (MH_CreateHook(sceneend_target, &sceneend::hook, reinterpret_cast<void**>(&sceneend_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize sceneend. (outdated index?)");
		return false;
	}	

	if (MH_CreateHook(fsn_target, &fsn::hook, reinterpret_cast<void**>(&fsn_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize fsn. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(get_view_model_target, &get_view_model::hook, reinterpret_cast<void**>(&get_view_model_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize get_view_model. (outdated index?)");
		return false;
	}	

	if (MH_CreateHook(get_overide_target, &overide_view::hook, reinterpret_cast<void**>(&overide_view_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize overide_view. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");
		return false;
	}	

	if (MH_CreateHook(lock_cursor_target, &lock_cursor::hook, reinterpret_cast<void**>(&lock_cursor_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize lock_cursor. (outdated index?)");
		return false;
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		throw std::runtime_error("failed to enable hooks.");
		return false;
	}

	interfaces::engine->get_screen_size(menu.screen_x, menu.screen_y);

	console::log("[setup] hooks initialized!\n");
	return true;
}

void hooks::release() 
{
	menu.menu_opened = false;
	interfaces::input->m_fCameraInThirdPerson = false;
	Sleep(100);

	MH_Uninitialize();
	MH_DisableHook(MH_ALL_HOOKS);

	SetWindowLongPtr(hooks::hCSGOWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::pOriginalWNDProc));
}

bool __fastcall hooks::create_move::hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd) 
{
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return create_move_original(input_sample_frametime, cmd);

	csgo::local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!csgo::local_player)
		return create_move_original(input_sample_frametime, cmd);

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C);

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	if (menu.config.misc && csgo::local_player->is_alive())
	{
		if (menu.config.bhop)
			features::misc::bunny_hop(cmd);

		if (menu.config.auto_pistol)
			features::misc::auto_pistol(cmd);

		if (menu.config.no_flash)
			if (csgo::local_player->flash_duration() > 1.f)
				csgo::local_player->flash_duration() = 0.f;

		if (menu.config.force_crosshair && !csgo::local_player->is_scoped())
			interfaces::console->get_convar("weapon_debug_spread_show")->set_value(3);
		else
			interfaces::console->get_convar("weapon_debug_spread_show")->set_value(0);
	}

	if (menu.config.misc)
		if (menu.config.animated_clan_tag)
			features::misc::animated_clan_tag(menu.config.clan_tag);

	prediction::start(cmd); 
	{
		if (menu.config.aimbot_legit)
			features::aimbot::legit::run(cmd);

		if (menu.config.trigger_bot)
			features::aimbot::run_trigger(cmd);
	} prediction::end();

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	//clamping movement
	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -450.0f, 450.0f);

	// clamping angles
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	features::misc::quick_peak(cmd);

	if (menu.config.auto_strafer)
		features::misc::auto_strafer(cmd);

	return false;
}

long __stdcall hooks::present::hook(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region) 
{
	if (!hooks::initialized_drawManager) 
	{
		menu.apply_fonts();
		menu.setup_resent(device);
		hooks::initialized_drawManager = true;
	}

	if (hooks::initialized_drawManager)
	{
		menu.pre_render(device);
		menu.post_render();
		if (menu.config.spectator_list)
			menu.spectator_list();
		if (menu.menu_opened)
			menu.render();
		menu.end_present(device);
	}

	return present_original(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

long __stdcall hooks::reset::hook(IDirect3DDevice9 * device, D3DPRESENT_PARAMETERS * present_parameters) 
{
	if (!hooks::initialized_drawManager)
		reset_original(device, present_parameters);

	menu.invalidate_objects();
	long hr = reset_original(device, present_parameters);
	menu.create_objects(device);

	return hr;
}

void __fastcall hooks::dme::hook(void* thisptr, void* edx, void* ctx, void* state, const model_render_info_t& info, matrix3x4_t* custom_bone_to_world)
{
	features::visuals::dme_chams_run(dme_original, thisptr, ctx, state, info, custom_bone_to_world);
}

void __stdcall hooks::sceneend::hook()
{
	//features::visuals::scene_chams_run();
	sceneend_original(interfaces::render_view);
}

void __stdcall hooks::fsn::hook(int frame_stage)
{
	if (frame_stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		if (menu.config.skins_enable)
			features::skins::run();
	fsn_original(interfaces::client, frame_stage);
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) 
	{
	case fnv::hash("HudZoom"):
		if (interfaces::engine->is_connected() && interfaces::engine->is_in_game() && menu.config.no_scope)
		{
			if (!csgo::local_player->is_alive())
				return;
			if (csgo::local_player->is_scoped())
			{
				int x, y;
				interfaces::engine->get_screen_size(x, y);
				render::draw_line(x / 2, 0, x / 2, y, color(0, 0, 0));
				render::draw_line(0, y / 2, x, y / 2, color(0, 0, 0));
			}
			return;
		}
	case fnv::hash("MatSystemTopPanel"):
		if (csgo::local_player)
			watermark.random_color((int)(csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick * 2.f));

		render::draw_text_string(1795, 5, render::fonts::watermark_font, "overflow", false, color(watermark.r, watermark.g, watermark.b));

		if (interfaces::engine->is_connected() && interfaces::engine->is_in_game())
		{
			if (menu.config.visuals)
			{
				if (!csgo::local_player)
					break;

				if (menu.config.visuals)
				{
					features::visuals::render_esp();
					features::visuals::render_visuals();

					if (menu.config.disbale_post_processing)
						interfaces::console->get_convar("mat_postprocess_enable")->set_value(0);
					else
						interfaces::console->get_convar("mat_postprocess_enable")->set_value(1);
				}
			}

			if (menu.config.do_quick_peek)
				features::misc::draw_start_pos();
		}

		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

void __stdcall hooks::lock_cursor::hook()
{
	if (menu.menu_opened)
	{
		interfaces::surface->unlock_cursor();
		return;
	}
	return lock_cursor_original(interfaces::surface);
}      

void __stdcall hooks::overide_view::hook(c_view_setup* vsView)
{
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return overide_view_original(interfaces::clientmode, vsView);

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return overide_view_original(interfaces::clientmode, vsView);

	if (!local_player->is_alive())
		return overide_view_original(interfaces::clientmode, vsView);

	// thirdperson
	features::misc::thirdperson();
	if (interfaces::input->m_fCameraInThirdPerson)
		interfaces::console->get_convar("crosshair")->set_value(0);
	else
		interfaces::console->get_convar("crosshair")->set_value(1);

	menu.config.actual_fov = vsView->fov;
	if (menu.config.character_fov > 0 && menu.config.no_scope ? true : !local_player->is_scoped())
		vsView->fov = menu.config.character_fov;

	overide_view_original(interfaces::clientmode, vsView);
}

float __stdcall hooks::get_view_model::hook()
{
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return 68.f;

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return 68.f;

	if (!local_player->is_scoped())
	{
		if (menu.config.wepaon_fov > 0)
			return menu.config.wepaon_fov;
	}
	return 68.f;		
}

LRESULT __stdcall hooks::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	if (GetAsyncKeyState(menu.config.menu_key) & 1)
		menu.menu_opened = !menu.menu_opened;

	if (GetAsyncKeyState(menu.config.legit_pair.first))
		menu.config.legit_pair.second = true;
	else
		menu.config.legit_pair.second = false;

	if (GetAsyncKeyState(menu.config.trigger_bot_pair.first))
		menu.config.trigger_bot_pair.second = true;
	else
		menu.config.trigger_bot_pair.second = false;

	if (GetAsyncKeyState(menu.config.thirdperson_pair.first) & 1)
		menu.config.thirdperson_pair.second = !menu.config.thirdperson_pair.second;

	if (GetAsyncKeyState(menu.config.quick_peak_pair.first) & 1)
		menu.config.do_quick_peek = !menu.config.do_quick_peek;

	if (menu.menu_opened) 
		interfaces::inputsystem->enable_input(false);
	else if (!menu.menu_opened) 
		interfaces::inputsystem->enable_input(true);

	if (menu.menu_opened && ImGui_ImplDX9_WndProcHandler(hwnd, message, wparam, lparam))
		return true;

	return CallWindowProcA(hooks::pOriginalWNDProc, hwnd, message, wparam, lparam);
}
