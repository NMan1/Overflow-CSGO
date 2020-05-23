#pragma once
#include "../../dependencies/common_includes.hpp"

namespace fs = std::experimental::filesystem;
extern IDirect3DStateBlock9* state_block;

class Menu
{
public:
	bool menu_opened = true;
	void render();
	int screen_x, screen_y;

	void __stdcall create_objects(IDirect3DDevice9* device) 
	{
		if (hooks::hCSGOWindow)
			ImGui_ImplDX9_CreateDeviceObjects();
	}

	void __stdcall invalidate_objects() 
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void __stdcall setup_resent(IDirect3DDevice9* device);

	void apply_fonts() {
		ImGui::CreateContext();

		font_menu = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 20);
		font_spectators = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 17.5);
		font_title = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 24);
		font_child_title = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 24);
		font_weapon = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\icon font.ttf", 28);
	}

	float clip(float n, float lower, float upper)
	{
		n = (n > lower) * n + !(n > lower) * lower;
		return (n < upper) * n + !(n < upper) * upper;
	}

	void __stdcall end_present(IDirect3DDevice9* device) 
	{
		if (menu_opened) 
		{
			if (ImGui::GetStyle().Alpha > 1.f)
				ImGui::GetStyle().Alpha = 1.f;
			else if (ImGui::GetStyle().Alpha != 1.f)
				ImGui::GetStyle().Alpha += 0.005f;
		}

		ImGui::Render();

		state_block->Apply();
		state_block->Release();
	}

	void __stdcall pre_render(IDirect3DDevice9* device) 
	{
		D3DVIEWPORT9 d3d_viewport;
		device->GetViewport(&d3d_viewport);

		device->CreateStateBlock(D3DSBT_ALL, &state_block);
		state_block->Capture();

		device->SetVertexShader(nullptr);
		device->SetPixelShader(nullptr);
		device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

		device->SetRenderState(D3DRS_LIGHTING, FALSE);
		device->SetRenderState(D3DRS_FOGENABLE, FALSE);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
		device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

		device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	}

	void __stdcall post_render()
	{
		ImGui_ImplDX9_NewFrame();
	}

	std::filesystem::path path;
	std::vector<std::string> configs{};

	void run(const char*) noexcept;
	void load(size_t) noexcept;
	void save(size_t) const noexcept;
	bool add(const char*) noexcept;
	void remove(size_t) noexcept;
	void rename(size_t, const char*) noexcept;
	void reset() noexcept;

	constexpr auto& get_configs() noexcept {
		return configs;
	}

	struct Config
	{
		// genereal
		int menu_key = 0x2D;
		int cheat_end_key = 0x23;
		std::pair<bool, bool> show_console_pair;

		// Rage
		bool aimbot_rage;
		
		// Legit
		bool aimbot_legit;
		int legit_fov;
		float smoothing;
		std::pair<int, bool> legit_pair;
		int legit_selection;
		int legit_bone;
		bool legit_silent;
		bool legit_always_on;
		bool simple_rcs;
		bool only_visible;
		bool standalone_rcs;
		float standalone_rcs_x;
		float standalone_rcs_y;

		// triggerbot
		std::pair<int, bool> trigger_bot_pair;
		bool trigger_bot;
		bool trigger_bot_always_on;
		int trigger_bone;
		float trigger_delay;

		// Visuals
		bool visuals;
		bool team_check;
		bool box;
		float f_box_clr[4] = { 1.f, 0.411f, 0.705f, 1.f };
		color box_clr	= color(255, 105, 180);
		bool box_outline;
		float f_box_outline_clr[4] = { 0, 0, 0, 1.f };
		color box_outline_clr = color(0, 0, 0);
		bool corner_box;
		float f_corner_box_clr[4] = { 1.f, 0.411f, 0.705f, 1.f };
		color corner_box_clr = color(255, 105, 180);
		bool corner_box_outline;
		float f_corner_outline_clr[4] = { 0, 0, 0, 1.f };
		color corner_outline_clr = color(0, 0, 0);
		bool health;
		float f_health_clr[4] = { 1.f, 0.411f, 0.705f, 1.f };
		color health_clr = color(255, 105, 180);
		bool animated_health;
		bool health_text;
		bool name;
		bool gun;
		bool show_fov;
		float f_fov_clr[4] = { 0.f, 0.f, 0.f, 1.f };
		color fov_clr = { 0, 0, 0 };
		bool show_spread;
		float f_spread_clr[4] = { 0.f, 0.f, 0.f, 1.f };
		color spread_clr = { 0, 0, 0 };
		bool night_mode;
		bool disbale_post_processing;
		bool flashed;
		bool scoped;
		bool armor;
		bool money;
		bool bomb;
		bool defuse_kit;
		bool defusing;

		//chams
		bool chams;
		bool team_check_chams;

		// Misc
		bool misc;
		bool bhop;
		bool force_crosshair;
		bool anti_screenshot;
		bool no_flash;
		float wepaon_fov = 68.f;
		float character_fov = 90;
		float actual_fov = 90;
		bool third_person;
		int third_person_distance = 150;
		bool third_disable_on_weapon;
		bool third_disable_on_throwable;
		std::pair<int, bool> thirdperson_pair;
		bool auto_pistol;
		bool no_scope;
		std::pair<int, bool> quick_peak_pair;
		bool do_quick_peek;
		char clan_tag[16] = { "Clantag Changer" };
		bool animated_clan_tag;
		float clan_tag_delay;

		// skins
		bool skins_enable;

		// add at end
		float f_chams_clr[4] = { 1.f, 0.411f, 0.705f, 1.f };
		color chams_clr = color(255, 105, 180);
		int chams_type = 0;
		bool local_chams;
		bool wireframe;
		float f_chams_local_clr[4] = { 1.f, 0.411f, 0.705f, 1.f };
		color chams_local_clr = color(255, 105, 180);		
		float f_chams_team_clr[4] = { 1.f, 0.411f, 0.705f, 1.f };
		color chams_team_clr = color(255, 105, 180);
		bool chams_xqz = true;
		bool weapon_icon;
		int skin_type = 0;
		bool spectator_list;
		float f_wep_name[4] = { 1.f, 1.f, 1.f, 1.f };
		color wep_name_clr = color(255, 105, 255);
		float f_wep_icon[4] = { 1.f, 1.f, 1.f, 1.f };
		color wep_icon_clr = color(255, 105, 180);		
		float f_name_clr[4] = { 1.f, 1.f, 1.f, 1.f };
		color name_clr = color(255, 105, 255);		
		float f_xqz_clr[4] = { 1.f, 1.f, 1.f, 1.f };
		color xqz_clr = color(255, 105, 255);
		bool auto_strafer = false;
		bool disable_occulusion = true;
		bool sleeve_chams = false;
		float f_sleeve_chams[4] = { 1.f, 1.f, 1.f, 1.f };
		color sleeve_chams_clr = color(255, 105, 255);		
		bool arms_chams = false;
		float f_arms_chams[4] = { 1.f, 1.f, 1.f, 1.f };
		color arms_chams_clr = color(255, 105, 255);
		bool rank_revealer = false;
		int filler____;
		bool remove_stamina;
		int bhop_chance = 100;
		int filler_;
		int filler__;
		int filler;
		bool bomb_esp;
		std::pair<int, bool> movement_blocker;
		std::pair<int, bool> crouch_blocker;
		std::pair<int, bool> fake_duck;
	};
	Config config;

	bool force_update = false;
	int weapon_index;
	int skin_index;
	int knife_model = 0;

	std::vector <std::string> spectators = {};
	int vec_size = 0;
	int new_vec_size = 0;
	int spec_height = 30;
private:
	ImFont* font_title = nullptr;
	ImFont* font_child_title = nullptr;
	ImFont* font_menu = nullptr;
	ImFont* font_spectators = nullptr;
	ImFont* font_weapon = nullptr;

	void change_log();

	void legit_tab();

	void rage_tab();

	void visuals_tab();

	void misc_tab();

	void skins_tab();

	void settings_tab();

public:
	void spectator_list();
};

extern Menu menu;