#include "Menu.h"
#include "cfg.h"
#include "../../source-sdk/math/vector3d.hpp"
#include "../../source-sdk/misc/color.hpp"
#include "../../dependencies/interfaces/i_surface.hpp"
#include "../../dependencies/utilities/xorstr.h"
#include "../../dependencies/interfaces/interfaces.hpp"
#include <sstream>
#include "../../dependencies/utilities/csgo.hpp"
#include "imgui/imgui_internal.h"
#include "../features/features.hpp"
Menu menu;

IDirect3DStateBlock9* state_block;
ImVec4 main_red = { 0.929, 0.290, 0.290, 1 }; // 237, 74, 74
ImVec4 main_pink_red = { 0.654, 0.094, 0.278, 1.f }; // 237, 74, 74
ImVec4 main_grey = { .6f, .6f, .6f, 1 }; // 237, 74, 74
int page = -1;

float one = 0.f;
float two = 0.f;
float version_f = 1.5;


const char* legit_select[] =
{
	"Closest To Crosshair",
	"Closest Distance",
	"Lowest Health"
};

const char* legit_bone[] =
{
	"Head",
	"Neck",
	"Chest",

};

const char* trigger_bone[] =
{
	"All Bones",
	"Head",
	"Chest",
	"Stomach",
	"Chest + Stomach"
};

const char* chams_type[] =
{
	"Textured",
	"Metallic",
	"Flat",
	"Dark Chrome",
	"Glow",
	"Crystal"
	"Wildfire"
};

int active = -1;

struct weapon_type_t
{
	const char* name;
	int type;
};

struct statrack_setting
{
	int definition_index = 1;
	struct
	{
		int counter = 0;
	}statrack_new;
};

struct weapon_kit_t
{
	int index;
	std::string rarity;
};

struct paint_kit_t
{
	int id;
	std::string english;
	std::vector<weapon_kit_t> weapons;
};

std::map<int, weapon_type_t> get_weapons(bool need_knife)
{
	std::map<int, weapon_type_t> k_item_names =
	{
		{ 63,{ "CZ75 Auto", WEAPONTYPE_PISTOL } },
		{ 1,{ "Desert Eagle", WEAPONTYPE_PISTOL } },
		{ 2,{ "Dual Berettas", WEAPONTYPE_PISTOL } },
		{ 3,{ "Five-SeveN", WEAPONTYPE_PISTOL } },
		{ 32,{ "P2000", WEAPONTYPE_PISTOL } },
		{ 36,{ "P250", WEAPONTYPE_PISTOL } },
		{ 61,{ "USP-S", WEAPONTYPE_PISTOL } },
		{ 30,{ "Tec-9", WEAPONTYPE_PISTOL } },
		{ 64,{ "R8 Revolver", WEAPONTYPE_PISTOL } },
		{ 4,{ "Glock-18", WEAPONTYPE_PISTOL } },

		{ 27,{ "MAG-7", WEAPONTYPE_SHOTGUN } },
		{ 35,{ "Nova", WEAPONTYPE_SHOTGUN } },
		{ 29,{ "Sawed-Off", WEAPONTYPE_SHOTGUN } },
		{ 25,{ "XM1014", WEAPONTYPE_SHOTGUN } },

		{ 24,{ "UMP-45", WEAPONTYPE_SUBMACHINEGUN } },
		{ 19,{ "P90", WEAPONTYPE_SUBMACHINEGUN } },
		{ 26,{ "PP-Bizon", WEAPONTYPE_SUBMACHINEGUN } },
		{ 17,{ "MAC-10", WEAPONTYPE_SUBMACHINEGUN } },
		{ 33,{ "MP7", WEAPONTYPE_SUBMACHINEGUN } },
		{ 34,{ "MP9", WEAPONTYPE_SUBMACHINEGUN } },
		{ WEAPON_MP5SD,{ "MP5-SD", WEAPONTYPE_SUBMACHINEGUN } },

		{ 14,{ "M249", WEAPONTYPE_MACHINEGUN } },
		{ 28,{ "Negev", WEAPONTYPE_MACHINEGUN } },

		{ 7,{ "AK-47", WEAPONTYPE_RIFLE } },
		{ 8,{ "AUG", WEAPONTYPE_RIFLE } },
		{ 13,{ "Galil AR", WEAPONTYPE_RIFLE } },
		{ 60,{ "M4A1-S", WEAPONTYPE_RIFLE } },
		{ 16,{ "M4A4", WEAPONTYPE_RIFLE } },
		{ 39,{ "SG 553", WEAPONTYPE_RIFLE } },
		{ 10,{ "FAMAS", WEAPONTYPE_RIFLE } },

		{ 9,{ "AWP", WEAPONTYPE_SNIPER_RIFLE } },
		{ 11,{ "G3SG1", WEAPONTYPE_SNIPER_RIFLE } },
		{ 38,{ "SCAR-20", WEAPONTYPE_SNIPER_RIFLE } },
		{ 40,{ "SSG 08", WEAPONTYPE_SNIPER_RIFLE } },
	};

	if (need_knife)
	{
		k_item_names[WEAPON_KNIFE_T] = { "T", -1 };
		k_item_names[WEAPON_KNIFE] = { "CT", -1 };

		k_item_names[GLOVE_T_SIDE] = { "T", -2 };
		k_item_names[GLOVE_CT_SIDE] = { "CT", -2 };
	}

	return k_item_names;
}

std::map<int, const char*> get_groups(bool need_knife = false, bool need_groups = false)
{
	std::map<int, const char*> groups =
	{
		{ WEAPONTYPE_PISTOL, ("Pistols") },
		{ WEAPONTYPE_SHOTGUN,("Shotguns") },
		{ WEAPONTYPE_SUBMACHINEGUN, ("Submachineguns") },
		{ WEAPONTYPE_MACHINEGUN, ("Machineguns") },
		{ WEAPONTYPE_RIFLE, ("Rifles") },
		{ WEAPONTYPE_SNIPER_RIFLE, ("Snipers") },
	};

	if (need_knife)
	{
		groups[-1] = { ("Knives") };
		groups[-2] = { ("Gloves") };
	}

	if (need_groups)
	{
		groups[201] = "Desert Eagle";
		groups[240] = "SSG08";
		groups[209] = "AWP";
	}

	return groups;
}

ImVec2 get_listbox_size(float x, float y_offset)
{
	return ImVec2(x, (ImGui::GetCurrentWindow()->Size.y - ImGui::GetStyle().WindowPadding.y * 2) - ImGui::GetCursorPosY() - y_offset);
}

bool selectable(const char* label, bool selected)
{
	ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyleColorVec4(ImGuiCol_Header));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetStyleColorVec4(ImGuiCol_HeaderActive));

	const auto state = ImGui::Selectable(label, selected);

	ImGui::PopStyleColor(3);

	return state;
}

bool listbox_group_weapons(
	int& selected_item,
	std::map<int, const char*> groups,
	std::map<int, weapon_type_t> items,
	ImVec2 listbox_size,
	bool show_only_selected = false,
	std::vector<int> selected_weapons = { }
	)
{
	auto current_value = selected_item;

	ImGui::ListBoxHeader("##items", listbox_size);
	{
		for (auto& group : groups)
		{
			ImGui::TextDisabled(group.second);
			ImGui::Separator();

			auto has_items = false;

			for (auto& item : items)
			{
				if (item.second.type == group.first)
				{
					if (show_only_selected)
						if (std::find(selected_weapons.begin(), selected_weapons.end(), item.first) == selected_weapons.end())
							continue;

					has_items = true;

					char name[128];
					sprintf(name, "%s##%d", item.second.name, item.first);
					if (selectable(name, item.first == selected_item))
						selected_item = item.first;
				}
			}

			if (has_items)
				ImGui::Separator();
		}
	}
	ImGui::ListBoxFooter();

	return current_value != selected_item;
}

bool listbox_group_paints(
	int& selected_item,
	std::vector<paint_kit> paint_kits,
	ImVec2 listbox_size
	)
{
	auto current_value = selected_item;

	ImGui::ListBoxHeader("##items", listbox_size);
	{
		ImGui::Separator();

		auto has_items = false;
		auto index = 0;
		for (auto& item : paint_kits)
		{
			has_items = true;

			if (selectable(item.name.c_str(), index == selected_item))
				selected_item = index;
			index = item.id;
		}

		if (has_items)
			ImGui::Separator();
	}
	ImGui::ListBoxFooter();

	return current_value != selected_item;
}


namespace ImGui
{
	namespace Custom
	{
	
		void ChildSettings(int postion, int& active)
		{
			PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 24));
			//if (active == postion)
			//	PushStyleColor(ImGuiCol_Border, ImVec4(0.929f, 0.290f, 0.290f, 1.f));
			//else
				//PushStyleColor(ImGuiCol_Border, ImVec4(0.929f, 0.290f, 0.290f, .4f));
				PushStyleColor(ImGuiCol_Border, main_pink_red);
		}

		void ChildSettingsInside(int postion, int& active)
		{
			if (ImGui::IsWindowFocused())
				active = postion;
		}

		void ChildSettingsEnd()
		{
			PopStyleColor();
			PopStyleVar();
		}
	}
}

void __stdcall Menu::setup_resent(IDirect3DDevice9* device)
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
	colors[ImGuiCol_Border] = ImVec4(0.654, 0.094, 0.278, 1.f);
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
	colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
	colors[ImGuiCol_Separator] = ImVec4(0.654, 0.094, 0.278, 1.f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.929, 0.290, 0.290, 0.50f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	//colors[ImGuiCol_Header] = ImVec4(237 / 255.f, 74 / 255.f, 74 / 255.f, .5f); //multicombo, combo selected item color.
	//colors[ImGuiCol_HeaderHovered] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
	//colors[ImGuiCol_HeaderActive] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);	
	colors[ImGuiCol_Header] = ImVec4(0.1f, 0.1f, 0.1f, 1.); //multicombo, combo selected item color.
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	colors[ImGuiCol_CloseButton] = ImVec4(0.10f, 0.10f, 0.10f, 0.50f);
	colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	create_objects(device);
}

void update_colors()
{
	menu.config.box_clr = color(menu.config.f_box_clr[0] * 255, menu.config.f_box_clr[1] * 255, menu.config.f_box_clr[2] * 255, menu.config.f_box_clr[3] * 255);
	menu.config.box_outline_clr = color(menu.config.f_box_outline_clr[0] * 255, menu.config.f_box_outline_clr[1] * 255, menu.config.f_box_outline_clr[2] * 255, menu.config.f_box_outline_clr[3] * 255);
	menu.config.corner_box_clr = color(menu.config.f_corner_box_clr[0] * 255, menu.config.f_corner_box_clr[1] * 255, menu.config.f_corner_box_clr[2] * 255, menu.config.f_corner_box_clr[3] * 255);
	menu.config.corner_outline_clr = color(menu.config.f_corner_outline_clr[0] * 255, menu.config.f_corner_outline_clr[1] * 255, menu.config.f_corner_outline_clr[2] * 255, menu.config.f_corner_outline_clr[3] * 255);
	menu.config.health_clr = color(menu.config.f_health_clr[0] * 255, menu.config.f_health_clr[1] * 255, menu.config.f_health_clr[2] * 255, menu.config.f_health_clr[3] * 255);
	menu.config.fov_clr = color(menu.config.f_fov_clr[0] * 255, menu.config.f_fov_clr[1] * 255, menu.config.f_fov_clr[2] * 255, menu.config.f_fov_clr[3] * 255);
	menu.config.spread_clr = color(menu.config.f_spread_clr[0] * 255, menu.config.f_spread_clr[1] * 255, menu.config.f_spread_clr[2] * 255, menu.config.f_spread_clr[3] * 255);
	menu.config.chams_clr = color(menu.config.f_chams_clr[0] * 255, menu.config.f_chams_clr[1] * 255, menu.config.f_chams_clr[2] * 255, menu.config.f_chams_clr[3] * 255);
	menu.config.chams_local_clr = color(menu.config.f_chams_local_clr[0] * 255, menu.config.f_chams_local_clr[1] * 255, menu.config.f_chams_local_clr[2] * 255, menu.config.f_chams_local_clr[3] * 255);
	menu.config.chams_team_clr = color(menu.config.f_chams_team_clr[0] * 255, menu.config.f_chams_team_clr[1] * 255, menu.config.f_chams_team_clr[2] * 255, menu.config.f_chams_team_clr[3] * 255);
	menu.config.name_clr = color(menu.config.f_name_clr[0] * 255, menu.config.f_name_clr[1] * 255, menu.config.f_name_clr[2] * 255, menu.config.f_name_clr[3] * 255);
	menu.config.wep_name_clr = color(menu.config.f_wep_name[0] * 255, menu.config.f_wep_name[1] * 255, menu.config.f_wep_name[2] * 255, menu.config.f_wep_name[3] * 255);
	menu.config.wep_icon_clr = color(menu.config.f_wep_icon[0] * 255, menu.config.f_wep_icon[1] * 255, menu.config.f_wep_icon[2] * 255, menu.config.f_wep_icon[3] * 255);
	menu.config.xqz_clr = color(menu.config.f_xqz_clr[0] * 255, menu.config.f_xqz_clr[1] * 255, menu.config.f_xqz_clr[2] * 255, menu.config.f_xqz_clr[3] * 255);
	menu.config.sleeve_chams_clr = color(menu.config.f_sleeve_chams[0] * 255, menu.config.f_sleeve_chams[1] * 255, menu.config.f_sleeve_chams[2] * 255, menu.config.f_sleeve_chams[3] * 255);
}

void Menu::render()
{
	ImGui::SetNextWindowSize(ImVec2(menu.screen_x, menu.screen_y));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Background", &menu.menu_opened, ImVec2(menu.screen_x, menu.screen_y), .45, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove);
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.568, 0.533, 0.533, .2f));
		ImGui::PopStyleColor();
		ImGui::End();
	}

	ImGui::SetNextWindowSize(ImVec2(840, 500));
	ImGui::SetNextWindowPos(ImVec2((menu.screen_x - 840) * .5, (menu.screen_y - 500) * .5), ImGuiCond_Once);
	ImGui::Begin("Overflow", &menu.menu_opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |  ImGuiWindowFlags_NoScrollbar);
	{
		ImGui::SetScrollY(0);
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImColor c = ImColor(32, 114, 247);

		// Nav Bar
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 30), ImVec2(p.x + ImGui::GetWindowWidth(), p.y - 3), ImColor(30, 30, 39));
		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x, p.y + 32), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + +30), ImColor(167, 24, 71, 255), ImColor(58, 31, 87, 255), ImColor(58, 31, 87, 255), ImColor(167, 24, 71, 255));

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Text, main_red);
		ImGui::PushFont(font_title);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6);
		ImGui::Text("O V E R F L O W");

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.117f, 0.117f, 0.152f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.117, 0.117, 0.152, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.117, 0.117, 0.152, 1));
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));

		ImGui::SameLine(ImGui::GetWindowWidth() - 110);
		ImGui::SetCursorPosY(-1);
		if (ImGui::ButtonT("SETTINGS", ImVec2(100, 25), page, 0, false, true)) { page = 5; }
		ImGui::SameLine(ImGui::GetWindowWidth() - 203);
		ImGui::SetCursorPosY(-1);
		if (ImGui::ButtonT("SKINS", ImVec2(90, 25), page, 0, false, true)) { page = 4; }
		ImGui::SameLine(ImGui::GetWindowWidth() - 290);
		ImGui::SetCursorPosY(-1);
		if (ImGui::ButtonT("MISC", ImVec2(90, 25), page, 0, false, true)) { page = 3; }
		ImGui::SameLine(ImGui::GetWindowWidth() - 390);
		ImGui::SetCursorPosY(-1);
		if (ImGui::ButtonT("VISUALS", ImVec2(90, 25), page, 0, false, true)) { page = 2; }
		ImGui::SameLine(ImGui::GetWindowWidth() - 485);
		ImGui::SetCursorPosY(-1);
		if (ImGui::ButtonT("RAGE", ImVec2(90, 25), page, 0, false, true)) { page = 1; }
		ImGui::SameLine(ImGui::GetWindowWidth() - 570);
		ImGui::SetCursorPosY(-1);
		if (ImGui::ButtonT("LEGIT", ImVec2(90, 25), page, 0, false, true)) { page = 0; }

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopFont();

		switch (page)
		{
		case -1:
			change_log(); break;
		case 0:
			legit_tab(); break;
		case 1:
			rage_tab(); break;
		case 2:
			visuals_tab(); break;
		case 3:
			misc_tab(); break;
		case 4:
			skins_tab(); break;
		case 5:
			settings_tab(); break;
		}


		ImGui::End();
	}
	update_colors();
}

void Menu::change_log()
{
	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14, 10));
	ImGui::BeginChild("Changes", ImVec2(826 - 14, 454 - 14), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		ImGui::Spacing();
		char buffer[UNLEN + 1];
		DWORD size;
		size = sizeof(buffer);
		GetUserName(buffer, &size);
		char title[UNLEN];
		char ch1[25] = "Welcome, ";
		char* ch = strcat(ch1, buffer);

		ImGui::Text(ch);
		std::string version("Version: beta ");
		version.append(std::to_string(version_f));
		version.erase(version.find_last_not_of('0') + 1, std::string::npos);

		ImGui::Text(version.c_str());
		ImGui::SameLine();
		ImGui::SetCursorPosX((826 - 14) - 325);
		ImGui::Text("Build Date: " __DATE__ " / " __TIME__);

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text(xorstr_("Added corner box to esp"));
		ImGui::Text(xorstr_("Added flags selection to esp"));
		ImGui::Text(xorstr_("Changed esp weapon name"));
		ImGui::Text(xorstr_("Made esp health lower so its more visible and not blocked by name esp"));
		ImGui::Text(xorstr_("Added recoil stand alone - applys rcs even when not aimbotting"));
		ImGui::Text(xorstr_("Added color picker"));
		ImGui::Text(xorstr_("Menu style changes"));
	}
	ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::PopStyleVar();
}

void Menu::legit_tab()
{
	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();


	ImGui::Custom::ChildSettings(3, active);
	ImGui::BeginChild("Settings", ImVec2(399, 440), true);
	{
		ImGui::Custom::ChildSettingsInside(3, active);

		ImGui::Checkbox("Enable Legit", &menu.config.aimbot_legit); 
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
		ImGui::SetCursorPosX(399 - 163);
		ImGui::Hotkey("##Key", &menu.config.legit_pair.first, ImVec2(150, 30));

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Checkbox("Silent", &menu.config.legit_silent);
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 150);
		ImGui::Checkbox("Always On", &menu.config.legit_always_on);

		ImGui::Checkbox("Visible Only", &menu.config.only_visible);
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 150);
		ImGui::Checkbox("Recoil Control", &menu.config.simple_rcs);

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Checkbox("Standalone RCS", &menu.config.standalone_rcs);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SliderFloat("X", &menu.config.standalone_rcs_x, 0, 1, "%.1f", 1, 171);
		ImGui::SameLine();
		ImGui::SliderFloat("Y", &menu.config.standalone_rcs_y, 0, 1, "%.1f", 1, 171);

		ImGui::Separator();

		ImGui::Spacing();

		ImGui::Text("Selection Type");
		ImGui::SameLine();
		ImGui::SetCursorPosX(210+39);
		ImGui::Text("Bone");
		ImGui::PushItemWidth(210);
		ImGui::Combo("##Selection Type", &menu.config.legit_selection, legit_select, IM_ARRAYSIZE(legit_select));
		ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX()+20);
		ImGui::Combo("##Bone", &menu.config.legit_bone, legit_bone, IM_ARRAYSIZE(legit_bone));
		ImGui::Spacing();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SliderInt("FOV", &menu.config.legit_fov, 0, 90, "%.0f", 171);
		ImGui::SameLine();
		ImGui::SetCursorPosX(37 + 171);
		ImGui::SliderFloat("Smoothing", &menu.config.smoothing, 0, 10, "%.1f", 1, 171);
	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();


	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();
	ImGui::SetCursorPosY(46);
	ImGui::SetCursorPosX(427);

	ImGui::Custom::ChildSettings(4, active);
	ImGui::BeginChild("#Trigger Bot", ImVec2(399, 213), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		ImGui::Custom::ChildSettingsInside(4, active);

		ImGui::Checkbox("Enable Trigger", &menu.config.trigger_bot);
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
		ImGui::SetCursorPosX(399 - 163);
		ImGui::Hotkey("##Key", &menu.config.trigger_bot_pair.first, ImVec2(150, 30));
		ImGui::Checkbox("Always On", &menu.config.trigger_bot_always_on);
		ImGui::Spacing();
		ImGui::Text("Trigger Bot Bone");
		ImGui::Combo("##Selection Type", &menu.config.trigger_bone, trigger_bone, IM_ARRAYSIZE(trigger_bone));
	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();

	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();
	ImGui::SetCursorPosY(273);
	ImGui::SetCursorPosX(427);

	ImGui::Custom::ChildSettings(5, active);
	ImGui::BeginChild("##Backtrack", ImVec2(399, 213), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		ImGui::Custom::ChildSettingsInside(5, active);

	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();
}

void Menu::rage_tab()
{
	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();


	ImGui::Custom::ChildSettings(0, active);
	ImGui::BeginChild("Settings", ImVec2(399, 440), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		ImGui::Custom::ChildSettingsInside(0, active);

		ImGui::Checkbox("Enable Rage", &menu.config.aimbot_rage);
		ImGui::Separator();
		ImGui::Spacing();
	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();


	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();
	ImGui::SetCursorPosY(46);
	ImGui::SetCursorPosX(427);

	ImGui::Custom::ChildSettings(1, active);
	ImGui::BeginChild("#Anti Aim", ImVec2(399, 213), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		ImGui::Custom::ChildSettingsInside(1, active);

	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();


	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();
	ImGui::SetCursorPosY(273);
	ImGui::SetCursorPosX(427);

	ImGui::Custom::ChildSettings(2, active);
	ImGui::BeginChild("##Backtrack", ImVec2(399, 213), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		ImGui::Custom::ChildSettingsInside(2, active);

	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();
}

void Menu::visuals_tab()
{
	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();


	ImGui::Custom::ChildSettings(6, active);
	ImGui::BeginChild("General", ImVec2(399, 440), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		ImGui::Custom::ChildSettingsInside(6, active);

		ImGui::Checkbox("Enable Visuals", &menu.config.visuals);
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Checkbox("Show team", &menu.config.team_check);
		if (ImGui::Checkbox("Box", &menu.config.box)) 
		{ 
			if (menu.config.corner_box) menu.config.corner_box = !menu.config.corner_box; 
			if (menu.config.corner_box_outline) menu.config.corner_box_outline = !menu.config.corner_box_outline;
		}
		ImGui::ColorEdit4("box color", menu.config.f_box_clr, ImGuiColorEditFlags_NoInputs);

		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);

		if (ImGui::Checkbox("Box Outline", &menu.config.box_outline)) 
		{ 
			if (!menu.config.box) menu.config.box = !menu.config.box;
			if (menu.config.corner_box) menu.config.corner_box = !menu.config.corner_box; 
			if (menu.config.corner_box_outline) menu.config.corner_box_outline = !menu.config.corner_box_outline;
		}
		ImGui::ColorEdit4("box outline", menu.config.f_box_outline_clr, ImGuiColorEditFlags_NoInputs);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		if (ImGui::Checkbox("Corner Box", &menu.config.corner_box)) 
		{ 
			if (menu.config.box) menu.config.box = !menu.config.box; 
			if (menu.config.box_outline) menu.config.box_outline = !menu.config.box_outline;
		}
		ImGui::ColorEdit4("corner box", menu.config.f_corner_box_clr, ImGuiColorEditFlags_NoInputs);

		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		if (ImGui::Checkbox("Corner Outline", &menu.config.corner_box_outline)) 
		{ 
			if (!menu.config.corner_box) menu.config.corner_box = !menu.config.corner_box;
			if (menu.config.box) menu.config.box = !menu.config.box; 
			if (menu.config.box_outline) menu.config.box_outline = !menu.config.box_outline;
		}
		ImGui::ColorEdit4("corner box outline", menu.config.f_corner_outline_clr, ImGuiColorEditFlags_NoInputs);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		ImGui::Checkbox("Health Bar", &menu.config.health);
		ImGui::ColorEdit4("health color", menu.config.f_health_clr, ImGuiColorEditFlags_NoInputs);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		if (ImGui::Checkbox("Health Text", &menu.config.health_text)) { if (menu.config.animated_health) menu.config.animated_health = !menu.config.animated_health; }
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		if (ImGui::Checkbox("Animated Text", &menu.config.animated_health)) { if (menu.config.health_text) menu.config.health_text = !menu.config.health_text; }
		ImGui::Checkbox("Show Name", &menu.config.name);
		ImGui::ColorEdit4("name", menu.config.f_name_clr, ImGuiColorEditFlags_NoInputs);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		ImGui::Checkbox("Show Weapon", &menu.config.gun);
		ImGui::ColorEdit4("wep name", menu.config.f_wep_name, ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine(); 
		ImGui::SetCursorPosX(399 - 175);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		ImGui::Checkbox("Show Icon", &menu.config.weapon_icon);
		ImGui::ColorEdit4("wep icon", menu.config.f_wep_icon, ImGuiColorEditFlags_NoInputs);

		ImGui::Spacing();
		ImGui::Spacing();

		// yes I know this is aids
		static std::string selected_items{"none"};
		if (ImGui::BeginCombo("Flags", selected_items.c_str(), ImVec2(0, 105)))
		{
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
			ImGui::Selectable(("flashed"), &menu.config.flashed, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
			ImGui::Selectable(("money"), &menu.config.money, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
			ImGui::Selectable(("armor"), &menu.config.armor, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
			ImGui::Selectable(("bomb"), &menu.config.bomb, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
			ImGui::Selectable(("scoped"), &menu.config.scoped, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
			ImGui::Selectable(("defuse kit"), &menu.config.defuse_kit, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
			ImGui::Selectable(("defusing"), &menu.config.defusing, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::EndCombo();
		}

		ImGui::Separator();
		ImGui::Spacing();
		
		ImGui::Checkbox("Show Aimbot FOV", &menu.config.show_fov);
		ImGui::ColorEdit4("fov clr", menu.config.f_fov_clr, ImGuiColorEditFlags_NoInputs);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		ImGui::Checkbox("Show Spread", &menu.config.show_spread);
		ImGui::ColorEdit4("spread clr", menu.config.f_spread_clr, ImGuiColorEditFlags_NoInputs);

	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();


	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();
	ImGui::SetCursorPosY(46);
	ImGui::SetCursorPosX(427);

	ImGui::Custom::ChildSettings(7, active);
	ImGui::BeginChild("#Chams", ImVec2(399, 213), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		ImGui::Custom::ChildSettingsInside(7, active);
		ImGui::Checkbox("Enable Chams", &menu.config.chams);
		ImGui::ColorEdit4("chams color", menu.config.f_chams_clr, ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		ImGui::Checkbox("XQZ", &menu.config.chams_xqz);
		ImGui::ColorEdit4("xqz color", menu.config.f_xqz_clr, ImGuiColorEditFlags_NoInputs);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		ImGui::Text("Chams Material");
		ImGui::Combo("##Chams Material", &menu.config.chams_type, chams_type, IM_ARRAYSIZE(chams_type));	
		ImGui::Checkbox("Show Team", &menu.config.team_check_chams);
		ImGui::ColorEdit4("team chams coilor", menu.config.f_chams_team_clr, ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		ImGui::Checkbox("Local Chams", &menu.config.local_chams);
		ImGui::ColorEdit4("local chams clr", menu.config.f_chams_local_clr, ImGuiColorEditFlags_NoInputs);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		ImGui::Checkbox("Wireframe", &menu.config.wireframe);
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		ImGui::Checkbox("Sleeve Chams", &menu.config.sleeve_chams);
		ImGui::ColorEdit4("sleve clr", menu.config.f_sleeve_chams, ImGuiColorEditFlags_NoInputs);	
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		ImGui::Checkbox("Arms Chams", &menu.config.arms_chams);
		ImGui::ColorEdit4("arms clr", menu.config.f_arms_chams, ImGuiColorEditFlags_NoInputs);
	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();


	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();
	ImGui::SetCursorPosY(273);
	ImGui::SetCursorPosX(427);

	ImGui::Custom::ChildSettings(8, active);
	ImGui::BeginChild("##Misc", ImVec2(399, 109), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{		
		ImGui::Custom::ChildSettingsInside(8, active);
		ImGui::Checkbox("Bomb Esp", &menu.config.bomb_esp);
	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();	

	ImGui::SetCursorPosY(396);
	ImGui::SetCursorPosX(427);

	ImGui::Custom::ChildSettings(9, active);
	ImGui::BeginChild("##World", ImVec2(399, 90), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{		
		ImGui::Custom::ChildSettingsInside(9, active);

		ImGui::Checkbox("Disable Post Processing", &menu.config.disbale_post_processing);
		ImGui::Checkbox("Disable Occulusion", &menu.config.disable_occulusion);
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		ImGui::Checkbox("Night Mode", &menu.config.night_mode);
	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();
}

void Menu::misc_tab()
{
	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();

	ImGui::Custom::ChildSettings(9, active);
	ImGui::BeginChild("General", ImVec2(399, 440), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		ImGui::Custom::ChildSettingsInside(9, active);
		ImGui::Checkbox("Enable Misc", &menu.config.misc);
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Checkbox("Bunny Hop", &menu.config.bhop);
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
		ImGui::SliderInt("Chance (%)", &menu.config.bhop_chance, 0, 100, "%.0f", 150);
		ImGui::Checkbox("Auto Strafer", &menu.config.auto_strafer);
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		ImGui::Checkbox("Auto Pistol", &menu.config.auto_pistol);
		ImGui::Checkbox("Anti Screenshot", &menu.config.anti_screenshot);
		ImGui::Checkbox("Quick Peak", &menu.config.quick_peak_pair.second);
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		ImGui::Checkbox("Spectator List", &menu.config.spectator_list);
		ImGui::Hotkey("##QuickPeakKey", &menu.config.quick_peak_pair.first, ImVec2(150, 30));
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
		ImGui::Checkbox("No Scope", &menu.config.no_scope);
		ImGui::Checkbox("Movement Blocker", &menu.config.movement_blocker);
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		ImGui::Checkbox("Rank Revealer", &menu.config.rank_revealer);
	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();

	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();
	ImGui::SetCursorPosY(46);
	ImGui::SetCursorPosX(427);

	ImGui::Custom::ChildSettings(10, active);
	ImGui::BeginChild("Visuals", ImVec2(399, 213), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		ImGui::Custom::ChildSettingsInside(10, active);
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::SliderFloat("Character FOV", &menu.config.character_fov, 10, 120, "%.0f");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SliderFloat("ViewModel FOV", &menu.config.wepaon_fov, 10, 120, "%.0f");
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Checkbox("Thirdperson", &menu.config.third_person);
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 170);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6);
		ImGui::Hotkey("##ThirdpersonKey", &menu.config.thirdperson_pair.first, ImVec2(150, 30));
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SliderInt("Distance", &menu.config.third_person_distance, 50, 200);
		ImGui::Checkbox("Disable On Weapon", &menu.config.third_disable_on_weapon);


	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();


	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();
	ImGui::SetCursorPosY(273);
	ImGui::SetCursorPosX(427);

	ImGui::Custom::ChildSettings(11, active);
	ImGui::BeginChild("Automation", ImVec2(399, 213), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		ImGui::Custom::ChildSettingsInside(11, active);
		ImGui::PushItemWidth(150);
		ImGui::InputText("##Clantag", menu.config.clan_tag, sizeof(menu.config.clan_tag));
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 87 - 35);
		if (ImGui::Button("   Apply", ImVec2(87, 27)))
			features::misc::set_clan_tag(menu.config.clan_tag);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
		ImGui::Checkbox("Animated | Delay", &menu.config.animated_clan_tag);
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
		ImGui::SliderFloat("", &menu.config.clan_tag_delay, 0, 1, "%.1f", 1, 175);

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Checkbox("Force Crosshair", &menu.config.force_crosshair);
		ImGui::Checkbox("No Flash", &menu.config.no_flash);
		ImGui::Checkbox("Remove Stamina", &menu.config.remove_stamina);

	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();
}

void Menu::skins_tab()
{
	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 24));
	ImGui::BeginChild("Items", ImVec2(261, 440), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		ImGui::Checkbox("Enable Skins", &menu.config.skins_enable);
		ImGui::Separator();

		auto weapon_index = 0;
		const auto weapons = get_weapons(true);
		auto can_change_index = [weapons, &weapon_index]()
		{
			if (!csgo::local_player || !csgo::local_player->is_alive())
				return false;

			if (!csgo::local_player->active_weapon())
				return false;

			weapon_index = csgo::local_player->active_weapon()->item_definition_index();
			return weapons.count(weapon_index) > 0;
		};

		const auto state = can_change_index();

		ImGui::SetCursorPosX(-16);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY()-7);
		listbox_group_weapons(menu.config.weapon_index, get_groups(true, false), weapons, get_listbox_size(261, state ? 26.f : -5.f));

		ImGui::SetCursorPosX((261 - 148)*.5);
		if (ImGui::Button("Current Weapon"))
			menu.config.weapon_index = weapon_index;

	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::PopStyleVar();

	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();

	ImGui::SetCursorPosY(46);
	ImGui::SetCursorPosX(289);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 24));
	ImGui::BeginChild("Skin", ImVec2(261, 440), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		static auto show_all_kits = false;
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
		ImGui::Checkbox("All skins", &show_all_kits);

		ImGui::SetCursorPosX(-16);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7);
		listbox_group_paints(menu.config.skin_index, parser_skins, get_listbox_size(261, -5.f));

	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::PopStyleVar();

	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();

	ImGui::SetCursorPosY(46);
	ImGui::SetCursorPosX(564);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 24));
	ImGui::BeginChild("##Settings", ImVec2(261, 440), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		if (ImGui::Button("Update", ImVec2(70, 30)))
		{
			features::skins::replace_paint_kit(menu.config.weapon_index, menu.config.skin_index);
			force_update = true;
		}		

		if (ImGui::Button("Save", ImVec2(60, 30)))
			features::skins::save();
		
	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::PopStyleVar();
}

void Menu::settings_tab()
{
	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 24));
	ImGui::BeginChild("Settings", ImVec2(399, 440), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		char buffer[UNLEN + 1];
		DWORD size;
		size = sizeof(buffer);
		GetUserName(buffer, &size);
		char title[UNLEN];
		char ch1[25] = "Welcome, ";
		char* ch = strcat(ch1, buffer);

		ImGui::Text(ch);
		std::string version("Version: beta ");
		version.append(std::to_string(version_f));
		version.erase(version.find_last_not_of('0') + 1, std::string::npos);

		ImGui::Text(version.c_str());
		ImGui::Text("Build Date: " __DATE__ " / " __TIME__);

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 39.5);
		ImGui::Text("Menu Key");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 72);
		ImGui::Text("End Cheat Key");

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 29.5);
		ImGui::Hotkey("##Menu Key", &menu.config.menu_key, ImVec2(150, 30));
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40 - 38);
		ImGui::Hotkey("##EndCheat", &menu.config.cheat_end_key, ImVec2(150, 30));

		ImGui::SetCursorPosX((399 - 184) * .5);
		ImGui::SetCursorPosY((440 - 50));
		if (ImGui::Button("Show Debug Console"))
		{
			console::initialize("Overflow Console");
			menu.config.show_console_pair.first = !menu.config.show_console_pair.first;
			if (!menu.config.show_console_pair.second)
			{
				console::initialize("Overflow Console");
				menu.config.show_console_pair.second = true;
			}
			else if (menu.config.show_console_pair.first)
				::ShowWindow(::GetConsoleWindow(), SW_SHOW);
			else
				::ShowWindow(::GetConsoleWindow(), SW_HIDE);			
		}

	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::PopStyleVar();

	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();

	ImGui::SetCursorPosY(46);
	ImGui::SetCursorPosX(427);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 24));
	ImGui::BeginChild("##Configs", ImVec2(399, 440), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		constexpr auto& config_items = menu.get_configs();
		static int current_config = 0;

		if (static_cast<size_t>(current_config) >= config_items.size())
			current_config = -1;

		static char buffer[16];
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
		ImGui::PushItemWidth(180);
		if (ImGui::ListBox("", &current_config, [](void* data, int idx, const char** out_text)
			{
				auto& vector = *static_cast<std::vector<std::string>*>(data);
				*out_text = vector[idx].c_str();
				return true;
			}, &config_items, config_items.size(), 5) && current_config != -1)
			strcpy(buffer, config_items[current_config].c_str());
			ImGui::PopStyleColor();

			ImGui::SameLine();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY());
			ImGui::PushID(0);
			ImGui::PushItemWidth(180-26);
			if (ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) 
				if (current_config != -1)
					menu.rename(current_config, buffer);
			
			ImGui::PopID();
			ImGui::NextColumn();

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 204);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 115);
			if (ImGui::Button(("Create"), ImVec2(73.5, 25))) 
				menu.add(buffer);
			
			ImGui::SameLine();
			if (ImGui::Button((" Reset"), ImVec2(74, 25)))
				menu.reset();
			
			ImGui::SetCursorPosX(215);
			ImGui::SetCursorPosY(105);
			if (ImGui::Button(("  Load"), ImVec2(74, 25)))
			{
				menu.load(current_config);
				if (interfaces::engine->is_in_game() && interfaces::engine->is_connected())
				{
					interfaces::console->console_printf(config_items[current_config].c_str());
					interfaces::console->console_printf(" loaded. \n");
					std::string msg("\x02[overflow] \x01" + config_items[current_config] + " config loaded"" ");
					interfaces::chat_element->chat_printf(0, hud_chat::chat_filters::CHAT_FILTER_NONE, msg.c_str());
					std::stringstream ss;
					ss << config_items[current_config].c_str() << " loaded.";
				}
			}
			ImGui::SameLine();
			if (ImGui::Button(("  Save"), ImVec2(74, 25)))
			{
				menu.save(current_config);
				if (interfaces::engine->is_in_game() && interfaces::engine->is_connected())
				{
					interfaces::console->console_printf(config_items[current_config].c_str());
					interfaces::console->console_printf(" saved. \n");
					std::string msg("\x02[overflow] \x01" + config_items[current_config] + " config saved"" ");
					interfaces::chat_element->chat_printf(0, hud_chat::chat_filters::CHAT_FILTER_NONE, msg.c_str());
					std::stringstream ss;
					ss << config_items[current_config].c_str() << " saved.";
				}
			}
			ImGui::SetCursorPosX(215);
			ImGui::SetCursorPosY(142);
			if (ImGui::Button(("Remove"), ImVec2(74, 25)))
				menu.remove(current_config);
	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::PopStyleVar();
}

// convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}

void Menu::spectator_list()
{
	if (menu.spectators.size() > 0)
		menu.spec_height = 41 + ((ImGui::CalcTextSize(menu.spectators[0].c_str()).y + 5) * menu.spectators.size());
	else
		menu.spec_height = 30;

	ImGui::SetNextWindowSize(ImVec2(175, menu.spec_height));
	ImGui::SetNextWindowPos(ImVec2(1700, 260), ImGuiCond_Once);
	ImGui::Begin("Spectators", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::SetCursorPosX((175 - ImGui::CalcTextSize("Spectators").x)*.5);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.5);
		ImGui::Text("Spectators");
		ImGui::Separator();
		if (!menu.spectators.empty())
		{
			ImGui::PushFont(menu.font_spectators);
			for (int i = 0; i < menu.spectators.size(); i++)
			{
				ImGui::SetCursorPosX((175 - ImGui::CalcTextSize(menu.spectators[i].c_str()).x) * .5);
				ImGui::Text(menu.spectators[i].c_str());
			}
			ImGui::PopFont();
		}
	}
	ImGui::End();
}