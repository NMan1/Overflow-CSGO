#include "../../../dependencies/common_includes.hpp"
#include "../features.hpp"
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF

struct knife_glove_info_t
{
	constexpr knife_glove_info_t(int index, const char* model, const char* icon = nullptr) :
		index(index),
		model(model),
		icon(icon)
	{}
	int index;
	const char* model;
	const char* icon;
};

std::map<int, knife_glove_info_t> knife_glove_info =
{
		{ WEAPON_KNIFE,{ WEAPON_KNIFE, "models/weapons/v_knife_default_ct.mdl", "knife_default_ct" } },
		{ WEAPON_KNIFE_T,{ WEAPON_KNIFE_T, "models/weapons/v_knife_default_t.mdl", "knife_t" } },
		{ WEAPON_BAYONET,{ WEAPON_BAYONET, "models/weapons/v_knife_bayonet.mdl", "bayonet" } },
		{ WEAPON_KNIFE_FLIP,{WEAPON_KNIFE_FLIP, "models/weapons/v_knife_flip.mdl", "knife_flip" } },
		{ WEAPON_KNIFE_GUT,{ WEAPON_KNIFE_GUT,"models/weapons/v_knife_gut.mdl", "knife_gut" } },
		{ WEAPON_KNIFE_KARAMBIT,{ WEAPON_KNIFE_KARAMBIT,"models/weapons/v_knife_karam.mdl", "knife_karambit" } },
		{ WEAPON_KNIFE_M9_BAYONET,{ WEAPON_KNIFE_M9_BAYONET,"models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet" } },
		{ WEAPON_KNIFE_TACTICAL,{ WEAPON_KNIFE_TACTICAL,"models/weapons/v_knife_tactical.mdl", "knife_tactical" } },
		{ WEAPON_KNIFE_FALCHION,{ WEAPON_KNIFE_FALCHION,"models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion" } },
		{ WEAPON_KNIFE_SURVIVAL_BOWIE,{ WEAPON_KNIFE_SURVIVAL_BOWIE,"models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie" } },
		{ WEAPON_KNIFE_BUTTERFLY,{WEAPON_KNIFE_BUTTERFLY ,"models/weapons/v_knife_butterfly.mdl", "knife_butterfly" } },
		{ WEAPON_KNIFE_PUSH,{WEAPON_KNIFE_PUSH ,"models/weapons/v_knife_push.mdl", "knife_push" } },
		{ WEAPON_KNIFE_URSUS,{WEAPON_KNIFE_URSUS, "models/weapons/v_knife_ursus.mdl", "knife_ursus" } },
		{ WEAPON_KNIFE_GYPSY_JACKKNIFE,{WEAPON_KNIFE_GYPSY_JACKKNIFE,"models/weapons/v_knife_gypsy_jackknife.mdl", "knife_gypsy_jackknife" } },
		{ WEAPON_KNIFE_STILETTO,{WEAPON_KNIFE_STILETTO, "models/weapons/v_knife_stiletto.mdl", "knife_stiletto" } },
		{ WEAPON_KNIFE_WIDOWMAKER,{WEAPON_KNIFE_WIDOWMAKER ,"models/weapons/v_knife_widowmaker.mdl", "knife_widowmaker" } },
		{ WEAPON_KNIFE_CSS,{WEAPON_KNIFE_CSS,"models/weapons/v_knife_css.mdl", "knife_css" } },
		{ WEAPON_KNIFE_CORD,{WEAPON_KNIFE_CORD, "models/weapons/v_knife_cord.mdl", "knife_cord" } },
		{ WEAPON_KNIFE_SURV,{WEAPON_KNIFE_SURV ,"models/weapons/v_knife_canis.mdl", "knife_canis" } },
		{ WEAPON_KNIFE_GYPSY_NOMAD,{WEAPON_KNIFE_GYPSY_NOMAD ,"models/weapons/v_knife_outdoor.mdl", "knife_outdoor" } },
		{ WEAPON_KNIFE_SKELETON,{WEAPON_KNIFE_SKELETON ,"models/weapons/v_knife_skeleton.mdl", "knife_skeleton" } },
		{ GLOVE_STUDDED_BLOODHOUND,{GLOVE_STUDDED_BLOODHOUND ,"models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl" } },
		{ GLOVE_T_SIDE,{GLOVE_T_SIDE ,"models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl" } },
		{ GLOVE_CT_SIDE,{GLOVE_CT_SIDE ,"models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl" } },
		{ GLOVE_SPORTY,{GLOVE_SPORTY ,"models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl" } },
		{ GLOVE_SLICK,{GLOVE_SLICK, "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl" } },
		{ GLOVE_LEATHER_WRAP,{GLOVE_LEATHER_WRAP, "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl" } },
		{ GLOVE_MOTORCYCLE,{GLOVE_MOTORCYCLE, "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl" } },
		{ GLOVE_SPECIALIST,{GLOVE_SPECIALIST, "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl" } },
		{ GLOVE_HYDRA,{GLOVE_HYDRA, "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl" } }
};

const knife_glove_info_t* get_weapon_info(const int& defindex)
{
	const auto entry = knife_glove_info.find(defindex);

	return entry == end(knife_glove_info) ? nullptr : &entry->second;
}

bool is_knife(int index)
{
	return index > 500 || index == WEAPON_KNIFE || index == WEAPON_KNIFE_T;
}

std::string default_skins{"63,0:1,0:2,0:3,0:32,0:36,0:61,0:30,0:64,0:4,0:27,0:35,0:29,0:25,0:24,0:19,0:26,0:17,0:33,0:34,0:14,0:28,0:7,0:8,0:13,0:60,0:16,0:39,0:10,0:9,0:11,0:38,0:40,0:59,0,0:42,0,0:"};
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

int features::skins::find_paint_kit(int item_definition_index, bool is_knife=false)
{
	auto pos_wep = file_skins.find(":" + std::to_string(item_definition_index) + ",");
	auto pos_first_mid = file_skins.find(",", pos_wep + 1);
	auto pos_secound_mid = file_skins.find(",", pos_first_mid + 1);
	auto pos_skin = file_skins.find(":", pos_secound_mid + 1);
	if (pos_wep != std::string::npos)
		if (pos_first_mid != std::string::npos && is_knife ? pos_secound_mid != std::string::npos : true)
			if (pos_skin != std::string::npos)
				if (is_knife)
					return std::stoi(file_skins.substr(pos_secound_mid + 1, pos_skin - pos_secound_mid - 1)); // paint kit
				else
					return std::stoi(file_skins.substr(pos_first_mid + 1, pos_skin - pos_first_mid - 1));
}

int features::skins::find_knife_model(int item_definition_index)
{
	auto pos_wep = file_skins.find(":" + std::to_string(item_definition_index) + ",");
	auto pos_first_mid = file_skins.find(",", pos_wep + 1);
	auto pos_secound_mid = file_skins.find(",", pos_first_mid + 1);
	auto pos_skin = file_skins.find(":", pos_secound_mid + 1);
	if (pos_wep != std::string::npos)
		if (pos_first_mid != std::string::npos && pos_secound_mid != std::string::npos)
			if (pos_skin != std::string::npos)
				return std::stoi(file_skins.substr(pos_first_mid + 1, pos_secound_mid - pos_first_mid - 1));
}

void features::skins::replace_paint_kit(int item_definition_index, int paint_kit, int knife_index)
{
	auto pos_wep = file_skins.find(":" + std::to_string(item_definition_index) + ",");
	auto pos_first_mid = file_skins.find(",", pos_wep + 1);
	auto pos_secound_mid = file_skins.find(",", pos_first_mid + 1);
	auto pos_skin = file_skins.find(":", pos_secound_mid + 1);
	if (pos_wep != std::string::npos)
		if (pos_first_mid != std::string::npos && (knife_index != -1) ? pos_secound_mid != std::string::npos : true)
			if (pos_skin != std::string::npos)
				if (knife_index != -1)
				{
					// replace model and paint kit
					file_skins.replace(pos_first_mid + 1, pos_secound_mid - pos_first_mid - 1, std::to_string(knife_index)); // model
					// re update since positions changed
					auto pos_secound_mid = file_skins.find(",", pos_first_mid + 1);
					auto pos_skin = file_skins.find(":", pos_secound_mid + 1);
					if (pos_secound_mid != std::string::npos && pos_skin != std::string::npos)
						file_skins.replace(pos_secound_mid + 1, pos_skin - pos_secound_mid - 1, std::to_string(paint_kit)); // paint kit
				}
				else
					file_skins.replace(pos_first_mid + 1, pos_skin - pos_first_mid - 1, std::to_string(paint_kit));
}

void features::skins::run()
{
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
		const auto index = is_knife(weapon->item_definition_index()) ? knife_index : weapon->item_definition_index();
		const auto paint_kit = find_paint_kit(index, index == knife_index);
		static auto old_state = -1;
		static auto update_knife = false;

		// fuck me, knife skins wont apply without forceupdating
		if (is_knife(weapon->item_definition_index()) && !is_knife(active_weapon->item_definition_index()))
		{
			if (old_state != csgo::local_player->life_state())
			{
				update_knife = true;
				old_state = csgo::local_player->life_state();
			}
		}
		
		if (paint_kit)
		{
			weapon->fallback_paint_kit() = paint_kit;
			if (is_knife(active_weapon->item_definition_index()))
			{
				auto knife_model = find_knife_model(knife_index);
				if (!knife_model)
					continue;

				auto info = get_weapon_info(knife_model);
				if (!info)
					continue;
				
				auto view_model = reinterpret_cast<base_view_model_t*>(interfaces::entity_list->get_client_entity_handle(local_player->view_model()));
				if (!view_model)
					continue;

				auto view_model_weapon_h = view_model->m_hweapon();
				if (!view_model_weapon_h)
					continue;

				auto view_model_weapon = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(view_model_weapon_h));
				if (view_model_weapon != weapon)
					continue;

				weapon->model_index() = interfaces::model_info->get_model_index(info->model);
				view_model->model_index() = interfaces::model_info->get_model_index(info->model);
				view_model->view_model_index() = interfaces::model_info->get_model_index(info->model);
				weapon->world_model_handle() = interfaces::model_info->get_model_index(info->model) + 1;
				weapon->item_definition_index() = info->index;
				weapon->entity_quality() = 3;
			}

			weapon->original_owner_xuid_low() = 0;
			weapon->original_owner_xuid_high() = 0;
			weapon->fallback_seed() = 661;
			weapon->item_id_high() = -1;
		}

		if (update_knife)
		{
			if (is_knife(active_weapon->item_definition_index()))
			{
				//interfaces::engine->execute_cmd("cl_fullupdate 1");
				menu.force_update = true;
				//interfaces::clientstate->full_update();
				//interfaces::console->get_convar("sv_cheats")->set_value(1);
				update_knife = false;
			}
		}
	}
}