#include "../../../dependencies/common_includes.hpp"
#include "../features.hpp"
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF

bool is_knife(const int& i)
{
	return (i >= WEAPON_BAYONET && i < GLOVE_STUDDED_BLOODHOUND) || i == WEAPON_KNIFE_T || i == WEAPON_KNIFE;
}

std::string default_skins{"63,0:1,0:2,0:3,0:32,0:36,0:61,0:30,0:64,0:4,0:27,0:35,0:29,0:25,0:24,0:19,0:26,0:17,0:33,0:34,0:14,0:28,0:7,0:8,0:13,0:60,0:16,0:39,0:10,0:9,0:11,0:38,0:40,0:59,0:42,0:"};
std::string features::skins::file_skins{};

void features::skins::create()
{
	PWSTR pathToDocuments;
	std::filesystem::path path;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &pathToDocuments))) 
	{
		path = pathToDocuments;
		path /= "overflow";
		path /= "skins";
		CoTaskMemFree(pathToDocuments);
	}

	std::ofstream out{ path / "skins" };
	out << default_skins;
	out.close();
}

void features::skins::load()
{
	PWSTR pathToDocuments;
	std::filesystem::path path;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &pathToDocuments)))
	{
		path = pathToDocuments;
		path /= "overflow";
		path /= "skins";
		CoTaskMemFree(pathToDocuments);
	}

	std::ifstream in{ path / "skins" };
	file_skins.clear();
	file_skins.append((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
	in.close();
	file_skins.erase(std::remove(file_skins.begin(), file_skins.end(), '\n'), file_skins.end());
}

void features::skins::save()
{
	PWSTR pathToDocuments;
	std::filesystem::path path;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &pathToDocuments)))
	{
		path = pathToDocuments;
		path /= "overflow";
		path /= "skins";
		CoTaskMemFree(pathToDocuments);
	}

	std::ofstream out{ path / "skins", std::ofstream::out | std::ofstream::trunc };
	out << file_skins;
	out.close();
}

int features::skins::find_paint_kit(int item_definition_index)
{
	auto pos_wep = file_skins.find(":"+std::to_string(item_definition_index) + ",");
	auto pos_mid = file_skins.find(",", pos_wep+1);
	auto pos_skin = file_skins.find(":", pos_wep+1);
	if (pos_wep != std::string::npos)
		if (pos_mid != std::string::npos)
			if (pos_skin != std::string::npos)
				return std::stoi(file_skins.substr(pos_mid + 1, pos_skin - pos_mid - 1));
}

void features::skins::replace_paint_kit(int item_definition_index, int paint_kit)
{
	auto pos_wep = file_skins.find(":"+std::to_string(item_definition_index) + ",");
	auto pos_mid = file_skins.find(",", pos_wep+1);
	auto pos_skin = file_skins.find(":", pos_wep+1);
	if (pos_wep != std::string::npos)
		if (pos_mid != std::string::npos)
			if (pos_skin != std::string::npos)
				file_skins.replace(pos_mid + 1, pos_skin - pos_mid - 1, std::to_string(paint_kit));
}

void features::skins::run()
{
	std::cout << "2" << std::endl;
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	auto active_weapon = local_player->active_weapon();
	if (!active_weapon)
		return;

	auto my_weapons = local_player->get_weapons();
	for (int i = 0; my_weapons[i] != INVALID_EHANDLE_INDEX; i++)
	{
		auto weapon = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(my_weapons[i]));
		if (!weapon)
			continue;
		
		if (weapon->item_definition_index() == WEAPON_FRAG_GRENADE || weapon->item_definition_index() == WEAPON_FLASHBANG || weapon->item_definition_index() == WEAPON_C4 || weapon->item_definition_index() == WEAPON_MOLOTOV || weapon->item_definition_index() == WEAPON_DECOY || weapon->item_definition_index() == WEAPON_SMOKEGRENADE)
			continue;
		
		const auto knife_index = local_player->team() == team::team_ct ? WEAPON_KNIFE : WEAPON_KNIFE_T;
		const auto index = is_knife(active_weapon->item_definition_index()) ? knife_index : weapon->item_definition_index();
		const auto paint_kit = find_paint_kit(index);
		std::cout << "1: " << paint_kit << std::endl;
		if (paint_kit)
		{
			weapon->fallback_paint_kit() = index;
			weapon->original_owner_xuid_low() = 0;
			weapon->original_owner_xuid_high() = 0;
			weapon->fallback_seed() = 661;
			weapon->item_id_high() = -1;
		}
		//else if (is_knife(active_weapon->item_definition_index()) && is_knife(weapon->item_definition_index()))
		//{
		//	auto view_model = reinterpret_cast<base_view_model_t*>(interfaces::entity_list->get_client_entity_handle(localplayer->view_model()));
		//	weapon->fallback_paint_kit() = 253;
		//	view_model->model_index() = interfaces::model_info->get_model_index("models/weapons/v_knife_karam.mdl");
		//	weapon->model_index() = interfaces::model_info->get_model_index("models/weapons/v_knife_karam.mdl");
		//	weapon->item_definition_index() = WEAPON_KNIFE_KARAMBIT;
		//	weapon->entity_quality() = 3;
		//}
	
	}
}