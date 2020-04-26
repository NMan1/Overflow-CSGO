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

	void __stdcall setup_resent(IDirect3DDevice9* device) 
	{
		ImGui_ImplDX9_Init(hooks::hCSGOWindow, device);

		ImGuiStyle& style = ImGui::GetStyle();
		style.Alpha = 1.0f;
		style.WindowPadding = ImVec2(0, 0);
		style.WindowMinSize = ImVec2(32, 32);
		style.WindowRounding = 0.0f;
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.ChildWindowRounding = 0.0f;
		style.FramePadding = ImVec2(4, 3);
		style.FrameRounding = 0.0f;
		style.ItemSpacing = ImVec2(8, 8);
		style.ItemInnerSpacing = ImVec2(8, 8);
		style.TouchExtraPadding = ImVec2(0, 0);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 0.0f;
		style.ScrollbarSize = 6.0f;
		style.ScrollbarRounding = 0.0f;
		style.GrabMinSize = 5.0f;
		style.GrabRounding = 0.0f;
		style.ButtonTextAlign = ImVec2(0.0f, 0.5f);
		style.DisplayWindowPadding = ImVec2(22, 22);
		style.DisplaySafeAreaPadding = ImVec2(4, 4);
		style.AntiAliasedLines = true;
		style.AntiAliasedShapes = false;
		style.CurveTessellationTol = 1.f;
		
		ImVec4* colors = ImGui::GetStyle().Colors;
		//colors[ImGuiCol_Text] = ImVec4(0.929, 0.290, 0.290, 1.00f); red
		colors[ImGuiCol_Text] = ImVec4(.6f, .6f, .6f, 1.00f); // grey
		colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_ChildWindowBg] = ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
		colors[ImGuiCol_Border] = ImVec4(0.654, 0.094, 0.278, .7);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.f, 0, 0, 1.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.101, 0.101, 0.101, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(.6f, .6f, .6f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.25f, 0.30f, 1.0f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.654, 0.094, 0.278, .25f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.654, 0.094, 0.278, .70f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.654, 0.094, 0.278, .70f);
		colors[ImGuiCol_ComboBg]              = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
		colors[ImGuiCol_Separator] = ImVec4(0.654, 0.094, 0.278, 1.f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.929, 0.290, 0.290, 0.50f);
		colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); 
		colors[ImGuiCol_ButtonActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(237 / 255.f, 74 / 255.f, 74 / 255.f, .5f); //multicombo, combo selected item color.
		colors[ImGuiCol_HeaderHovered] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
		colors[ImGuiCol_HeaderActive] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		colors[ImGuiCol_CloseButton]          = ImVec4(0.10f, 0.10f, 0.10f, 0.50f);
		colors[ImGuiCol_CloseButtonHovered]   = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_CloseButtonActive]    = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
		colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

		create_objects(device);
	}

	void apply_fonts() {
		ImGui::CreateContext();

		font_menu = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 20);
		font_title = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 24);
		font_child_title = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 24);
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
		int smoothing;
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
		bool skins;

		// add at end
		float f_chams_clr[4] = { 1.f, 0.411f, 0.705f, 1.f };
		color chams_clr = color(255, 105, 180);
		int chams_type = 0;
		bool local_chams;
		bool wireframe;
	};
	Config config;


private:
	ImFont* font_title = nullptr;
	ImFont* font_child_title = nullptr;
	ImFont* font_menu = nullptr;

	void change_log();

	void legit_tab();

	void rage_tab();

	void visuals_tab();

	void misc_tab();

	void skins_tab();

	void settings_tab();
};

extern Menu menu;