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
hooks::list_leaves::fn		list_leaves_original		= nullptr;
hooks::file_check::fn		file_check_original         = nullptr;
hooks::file_system::fn		file_system_original		= nullptr;
hooks::send_net_msg::fn		send_net_msg_original       = nullptr;

using fn_org = void(__fastcall*)(void*, void*);
fn_org hook_org = nullptr;

uint8_t*					present_address				= nullptr;
uint8_t*					reset_address				= nullptr;
i_net_channel*				old_net_channel				= nullptr;
void*						send_net_msg_target;

HWND						hooks::hCSGOWindow			= nullptr; 
WNDPROC						hooks::pOriginalWNDProc		= nullptr;
bool						hooks::initialized_drawManager = false;
color						watermark					= (255, 59, 59);
uintptr_t					p_list_leaves;

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
	auto list_leaves_target = reinterpret_cast<void*>(get_virtual(interfaces::engine->get_bsp_tree_query(), 6));
	auto file_check_target = reinterpret_cast<void*>(utilities::pattern_scan(GetModuleHandleW(L"engine.dll"), "55 8B EC 81 EC ? ? ? ? 53 8B D9 89 5D F8 80"));
	auto file_system_target = reinterpret_cast<void*>(get_virtual(**reinterpret_cast<void***>(utilities::pattern_scan(GetModuleHandleW(L"engine.dll"), "8B 0D ? ? ? ? 8D 95 ? ? ? ? 6A 00 C6") + 0x2), 128));
	auto present_address = utilities::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 DB") + 0x2;
	auto reset_address = utilities::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 FF 78 18") + 0x2;
	interfaces::engine->is_in_game() ? send_net_msg_target = reinterpret_cast<void*>(get_virtual(old_net_channel = (i_net_channel*)interfaces::clientstate->net_channel, 40)) : send_net_msg_target = nullptr;

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

	if (MH_CreateHook(list_leaves_target, &list_leaves::hook, reinterpret_cast<void**>(&list_leaves_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize list_leaves. (outdated index?)");
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

	if (MH_CreateHook(file_check_target, &file_check::hook, reinterpret_cast<void**>(&file_check_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize file_check_target. (outdated index?)");
		return false;
	}		

	if (MH_CreateHook(file_system_target, &file_system::hook, reinterpret_cast<void**>(&file_system_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize file_check_target. (outdated index?)");
		return false;
	}	

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		throw std::runtime_error("failed to enable hooks.");
		return false;
	}

	interfaces::engine->get_screen_size(menu.screen_x, menu.screen_y);
	p_list_leaves = (uintptr_t)utilities::pattern_scan(GetModuleHandleW(L"client_panorama"), "56 52 FF 50 18") + 5;
	interfaces::console->get_convar("sv_cheats")->set_value(1);
	interfaces::console->get_convar("sv_pure_allow_loose_file_loads")->set_value(0);

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

	if (!csgo::local_player->is_alive())
		return create_move_original(input_sample_frametime, cmd);

	//if (!interfaces::game_rules)
	//	interfaces::game_rules = **reinterpret_cast<c_csgamerulesproxy***>(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 74") + 0x1);

	//if (interfaces::game_rules)
	//	csgo::rounds = interfaces::game_rules->round_count();

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C);

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	if (menu.config.rank_revealer && (cmd->buttons & in_score) != 0)
		interfaces::client->dispatch_user_message(cs_um_serverrankrevealall, 0, 0, nullptr);
	
	if (menu.config.fake_duck.second)
		interfaces::engine->get_net_channel_info()->choked_packets <= 7 ? cmd->buttons &= ~in_duck : cmd->buttons |= in_duck;
	std::cout << interfaces::engine->get_net_channel_info()->choked_packets << std::endl;
	if (menu.config.misc)
	{
		if (menu.config.bhop)
			features::misc::bunny_hop(cmd);

		if (menu.config.auto_pistol)
			features::misc::auto_pistol(cmd);

		if (menu.config.no_flash)
			if (csgo::local_player->flash_duration() > 1.f)
				csgo::local_player->flash_duration() = 0.f;

		if (menu.config.remove_stamina)
			cmd->buttons |= in_bullrush;

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

	// movement features
	features::misc::quick_peak(cmd);

	if (menu.config.auto_strafer)
		features::misc::auto_strafer(cmd);	

	if (!menu.menu_opened)
	{
		if (menu.config.movement_blocker.second)
			features::misc::movement_blocker(cmd);

		if (menu.config.crouch_blocker.second)
			features::misc::crouch_blocker(cmd);
	}

	if (!old_net_channel && !send_net_msg_target)
	{
		send_net_msg_target = reinterpret_cast<void*>(get_virtual(old_net_channel = (i_net_channel*)interfaces::clientstate->net_channel, 40));
		MH_CreateHook(send_net_msg_target, &send_net_msg::hook, reinterpret_cast<void**>(&send_net_msg_original));
		MH_EnableHook(send_net_msg_target);
	}
	else if (old_net_channel != (i_net_channel*)interfaces::clientstate->net_channel)
	{
		old_net_channel = (i_net_channel*)interfaces::clientstate->net_channel;
		if (old_net_channel)
		{
			send_net_msg_target = reinterpret_cast<void*>(get_virtual(old_net_channel, 40));
			MH_CreateHook(send_net_msg_target, &send_net_msg::hook, reinterpret_cast<void**>(&send_net_msg_original));
			MH_EnableHook(send_net_msg_target);
		}
	}

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

// pasted
int __fastcall hooks::list_leaves::hook(void* bsp, void* edx, vec3_t& mins, vec3_t& maxs, unsigned short* list, int listMax)
{
	if (std::uintptr_t(_ReturnAddress()) == p_list_leaves)
	{
		if (const auto info = *reinterpret_cast<RenderableInfo_t**>(std::uintptr_t(_AddressOfReturnAddress()) + 0x14); info && info->renderable) 
		{
			auto base_entity = info->renderable->GetIClientUnknown()->GetBaseEntity();
			if (base_entity)
			{
				if (base_entity->is_player())
				{
					if (menu.config.disable_occulusion)
					{
						// FIXME: sometimes players are rendered above smoke, maybe sort render list?
						info->flags &= ~0x100;
						info->flags2 |= 0xC0;

						constexpr float maxCoord = 16384.0f;
						constexpr float minCoord = -maxCoord;
						static vec3_t min{ minCoord, minCoord, minCoord };
						static vec3_t max{ maxCoord, maxCoord, maxCoord };
						return list_leaves_original(bsp, min, max, list, listMax);
					}
				}
			}
		}
	}
	return list_leaves_original(bsp, mins, maxs, list, listMax);
}

void __fastcall hooks::dme::hook(void* thisptr, void* edx, void* ctx, void* state, const model_render_info_t& info, matrix3x4_t* custom_bone_to_world)
{
	features::visuals::dme_misc(info);
	features::visuals::dme_chams_run(dme_original, thisptr, ctx, state, info, custom_bone_to_world);
}

void __stdcall hooks::sceneend::hook()
{
	//features::visuals::scene_chams_run();
	sceneend_original(interfaces::render_view);
}

void __stdcall hooks::fsn::hook(int frame_stage)
{
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		fsn_original(interfaces::client, frame_stage);

	if (menu.force_update && interfaces::engine->is_connected() && interfaces::engine->is_in_game())
	{
		//utilities::force_update();
		interfaces::clientstate->full_update();
		menu.force_update = !menu.force_update;
	}

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
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return 68.f;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player || !local_player->is_alive())
		return 68.f;

	if (!local_player->is_scoped())
	{
		if (menu.config.wepaon_fov > 0)
			return menu.config.wepaon_fov;
	}
	return 68.f;		
}

// fuck valve
bool __fastcall hooks::send_net_msg::hook(i_net_channel* thisptr, int edx, i_net_message* pMessage, bool bForceReliable, bool bVoice)
{
	if (pMessage->GetType() == 14)
		return false;

	if (pMessage->GetGroup() == 9)
		bVoice = true;

	return send_net_msg_original(thisptr, edx, pMessage, bForceReliable, bVoice);
}

void __fastcall hooks::file_check::hook(void* ecx, void* edx)
{
	return;
}

bool __fastcall hooks::file_system::hook(void* ecx, void* edx) 
{
	return true;
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

	if (GetAsyncKeyState(menu.config.movement_blocker.first) & 1)
		menu.config.movement_blocker.second = !menu.config.movement_blocker.second;	

	if (GetAsyncKeyState(menu.config.crouch_blocker.first) & 1)
		menu.config.crouch_blocker.second = !menu.config.crouch_blocker.second;

	if (GetAsyncKeyState(menu.config.fake_duck.first) & 1)
		menu.config.fake_duck.second = !menu.config.fake_duck.second;

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
