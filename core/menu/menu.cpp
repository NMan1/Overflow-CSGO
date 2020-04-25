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
ImVec4 main_pink_red = { 0.654, 0.094, 0.278, 1 }; // 237, 74, 74
ImVec4 main_grey = { .6f, .6f, .6f, 1 }; // 237, 74, 74
int page = -1;

float one = 0.f;
float two = 0.f;
float version_f = 1.5;

const char* legit_select[] =
{
	"Closest To Crosshair",
	"Distance",
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

int active = -1;

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
				PushStyleColor(ImGuiCol_Border, ImVec4(0.929f, 0.290f, 0.290f, .4f));
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
}

void Menu::render()
{
	//0.568, 0.533, 0.533
	// Black overlay
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
		ImGui::SetCursorPosX((826 - 14) - 400);
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

		ImGui::SetCursorPosX((399 - 275) * .5);
		ImGui::Text("Selection Type");
		ImGui::SetCursorPosX((399 - 275) * .5);
		ImGui::Combo("##Selection Type", &menu.config.legit_selection, legit_select, IM_ARRAYSIZE(legit_select));
		ImGui::Spacing();

		ImGui::SetCursorPosX((399 - 275) * .5);
		ImGui::Text("Bone");
		ImGui::SetCursorPosX((399 - 275) * .5);
		ImGui::Combo("##Bone", &menu.config.legit_bone, legit_bone, IM_ARRAYSIZE(legit_bone));
		ImGui::Spacing();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SetCursorPosX((399 - 275) * .5);
		ImGui::SliderInt("FOV", &menu.config.legit_fov, 0, 180);	

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SetCursorPosX((399 - 275) * .5);
		ImGui::SliderInt("Smoothing", &menu.config.smoothing, 0, 25);
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
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		ImGui::Checkbox("Show Weapon", &menu.config.gun);

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
		ImGui::Checkbox("Show Team", &menu.config.team_check_chams);

	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();


	ImGui::Dummy(ImVec2(0, 6));
	ImGui::Dummy(ImVec2(6, 0)); ImGui::SameLine();
	ImGui::SetCursorPosY(273);
	ImGui::SetCursorPosX(427);

	ImGui::Custom::ChildSettings(8, active);
	ImGui::BeginChild("##World", ImVec2(399, 213), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{		
		ImGui::Custom::ChildSettingsInside(8, active);

		ImGui::Checkbox("Disable Post Processing", &menu.config.disbale_post_processing);
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
		ImGui::Checkbox("Auto Pistol", &menu.config.auto_pistol);
		ImGui::Checkbox("No Scope", &menu.config.no_scope);
		ImGui::SameLine();
		ImGui::SetCursorPosX(399 - 175);
		ImGui::Checkbox("Anti Screenshot", &menu.config.anti_screenshot);
		ImGui::Checkbox("Quick Peak", &menu.config.quick_peak_pair.second);
		ImGui::Hotkey("##QuickPeakKey", &menu.config.quick_peak_pair.first, ImVec2(150, 30));
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
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

		ImGui::SliderFloat("Character FOV", &menu.config.character_fov, 10, 120);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SliderFloat("ViewModel FOV", &menu.config.wepaon_fov, 10, 120);
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
		ImGui::SliderInt("Thirdperson Distance", &menu.config.third_person_distance, 50, 200);
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

	} ImGui::EndChild(true, menu.font_child_title, main_red);
	ImGui::Custom::ChildSettingsEnd();
}

void Menu::skins_tab()
{

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

