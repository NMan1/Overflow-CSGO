#include "../../../dependencies/common_includes.hpp"
#include "../features.hpp"


void custom_materials() 
{
	std::ofstream("csgo\\materials\\overflow_reflective.vmt") << R"#("VertexLitGeneric"
	{
		"$basetexture" "vgui/white_additive"
		"$ignorez" "0"
		"$envmap" "env_cubemap"
		"$normalmapalphaenvmapmask" "1"
		"$envmapcontrast"  "1"
		"$nofog" "1"
		"$model" "1"
		"$nocull" "0"
		"$selfillum" "1"
		"$halflambert" "1"
		"$znearer" "0"
		"$flat" "1"
		"$reflectivity" "1"
		"$phong" "1"
	}
	)#";	
	std::ofstream("csgo\\materials\\overflow_reflective_xqz.vmt") << R"#("VertexLitGeneric"
	{
		"$basetexture" "vgui/white_additive"
		"$ignorez" "1"
		"$envmap" "env_cubemap"
		"$normalmapalphaenvmapmask" "1"
		"$envmapcontrast"  "1"
		"$nofog" "1"
		"$model" "1"
		"$nocull" "0"
		"$selfillum" "1"
		"$halflambert" "1"
		"$znearer" "0"
		"$flat" "1"
		"$reflectivity" "1"
		"$phong" "1"
		"$rimlight" "1"
	}
	)#";

	std::ofstream("csgo\\materials\\overflow_flat.vmt") << R"#("UnlitGeneric"
	{
	  "$basetexture" "vgui/white"
	  "$ignorez"      "0"
	  "$envmap"       ""
	  "$nofog"        "1"
	  "$model"        "1"
	  "$nocull"       "0"
	  "$selfillum"    "1"
	  "$halflambert"  "1"
	  "$znearer"      "0"
	  "$flat"         "1"
	}
	)#";

	std::ofstream("csgo\\materials\\overflow_flat_xqz.vmt") << R"#("UnlitGeneric"
	{
	  "$basetexture" "vgui/white"
	  "$ignorez"      "1"
	  "$envmap"       ""
	  "$nofog"        "1"
	  "$model"        "1"
	  "$nocull"       "0"
	  "$selfillum"    "1"
	  "$halflambert"  "1"
	  "$znearer"      "0"
	  "$flat"         "1"
	}
	)#";

	std::ofstream("csgo\\materials\\overflow_regular.vmt") << R"#("VertexLitGeneric"
	{
	  "$basetexture" "vgui/white"
	  "$ignorez"      "0"
	  "$envmap"       ""
	  "$nofog"        "1"
	  "$model"        "1"
	  "$nocull"       "0"
	  "$selfillum"    "1"
	  "$halflambert"  "1"
	  "$znearer"      "0"
	  "$flat"         "1"
	}
	)#";	

	std::ofstream("csgo\\materials\\overflow_regular_xqz.vmt") << R"#("VertexLitGeneric"
	{
		"$basetexture" "vgui/white"
		"$ignorez"      "1"
		"$envmap"       ""
		"$nofog"        "1"
		"$model"        "1"
		"$nocull"       "0"
		"$selfillum"    "1"
		"$halflambert"  "1"
		"$znearer"      "0"
		"$flat"         "1"
	}
	)#";
}

void features::visuals::dme_chams_run(hooks::dme::fn original, void* thisptr, void* context, void* state, const model_render_info_t& info, matrix_t* custom_bone_to_world)
{
	if (!menu.config.chams)
		return original(thisptr, context, state, info, custom_bone_to_world);

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return original(thisptr, context, state, info, custom_bone_to_world);

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return original(thisptr, context, state, info, custom_bone_to_world);

	if (!info.model)
		return original(thisptr, context, state, info, custom_bone_to_world);

	// get our entity
	auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
	if (!entity || !entity->is_alive() || entity->dormant())
		return original(thisptr, context, state, info, custom_bone_to_world);

	static bool create = true;
	if (create)
	{
		custom_materials();
		create = false;
	}

	if (strstr(interfaces::model_info->get_model_name(info.model), "models/player"))
	{
		static int index;
		static i_material* mat = nullptr;
		static i_material* mat_xqz = nullptr;
		if (!mat || !mat_xqz || index != menu.config.chams_type)
			switch (menu.config.chams_type)
			{
			case 0:
				mat = interfaces::material_system->find_material("overflow_regular", TEXTURE_GROUP_MODEL); index = menu.config.chams_type;
				mat_xqz = interfaces::material_system->find_material("/overflow_regular_xqz", TEXTURE_GROUP_MODEL); index = menu.config.chams_type; break;
			case 1:
				mat = interfaces::material_system->find_material("overflow_reflective", TEXTURE_GROUP_MODEL, true, nullptr); index = menu.config.chams_type;
				mat_xqz = interfaces::material_system->find_material("overflow_reflective_xqz", TEXTURE_GROUP_MODEL); index = menu.config.chams_type; break;
			case 2:
				mat = interfaces::material_system->find_material("overflow_flat", TEXTURE_GROUP_MODEL); index = menu.config.chams_type; 
				mat_xqz = interfaces::material_system->find_material("overflow_flat_xqz", TEXTURE_GROUP_MODEL); index = menu.config.chams_type; break;
			case 3:
				mat = interfaces::material_system->find_material("models/gibs/glass/glass", TEXTURE_GROUP_OTHER); index = menu.config.chams_type;
				mat_xqz = interfaces::material_system->find_material("models/gibs/glass/glass", TEXTURE_GROUP_MODEL); index = menu.config.chams_type; break;
			case 4:
				mat = interfaces::material_system->find_material("dev/glow_armsrace.vmt", TEXTURE_GROUP_OTHER); index = menu.config.chams_type; 
				mat_xqz = interfaces::material_system->find_material("dev/glow_armsrace.vmt", TEXTURE_GROUP_MODEL); index = menu.config.chams_type; break;
			case 5:
				mat = interfaces::material_system->find_material("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER); index = menu.config.chams_type;
				mat_xqz = interfaces::material_system->find_material("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_MODEL); index = menu.config.chams_type; break;
			case 6:
				mat = interfaces::material_system->find_material("models/inventory_items/wildfire_gold/wildfire_gold_detail", TEXTURE_GROUP_OTHER); index = menu.config.chams_type; 
				mat_xqz = interfaces::material_system->find_material("models/inventory_items/wildfire_gold/wildfire_gold_detail", TEXTURE_GROUP_MODEL); index = menu.config.chams_type; break;
			default:
				index = menu.config.chams_type; break;
			}

		mat->increment_reference_count();
		mat->set_material_var_flag(material_var_wireframe, menu.config.wireframe);

		// team
		if (entity->team() == local_player->team() && menu.config.team_check_chams)
		{
			if (menu.config.chams_xqz)
			{
				interfaces::render_view->set_blend(menu.config.f_xqz_clr[3]);
				interfaces::render_view->modulate_color(menu.config.f_xqz_clr);
				interfaces::model_render->override_material(mat_xqz);
				original(thisptr, context, state, info, custom_bone_to_world);
			}

			interfaces::render_view->set_blend(menu.config.f_chams_clr[3]);
			interfaces::render_view->modulate_color(menu.config.f_chams_clr);
			interfaces::model_render->override_material(mat);
		}	

		// enemy
		if (entity->team() != local_player->team())
		{
			if (menu.config.chams_xqz)
			{
				interfaces::render_view->set_blend(menu.config.f_xqz_clr[3]);
				interfaces::render_view->modulate_color(menu.config.f_xqz_clr);
				interfaces::model_render->override_material(mat_xqz);
				original(thisptr, context, state, info, custom_bone_to_world);
			}

			interfaces::render_view->set_blend(menu.config.f_chams_clr[3]);
			interfaces::render_view->modulate_color(menu.config.f_chams_clr);
			interfaces::model_render->override_material(mat);
		}

		// local
		if (menu.config.local_chams && entity == local_player)
		{
			if (menu.config.chams_xqz)
			{
				interfaces::render_view->set_blend(menu.config.f_xqz_clr[3]);
				interfaces::render_view->modulate_color(menu.config.f_xqz_clr);
				interfaces::model_render->override_material(mat_xqz);
				original(thisptr, context, state, info, custom_bone_to_world);
			}

			interfaces::render_view->set_blend(menu.config.f_chams_local_clr[3]);
			interfaces::render_view->modulate_color(menu.config.f_chams_local_clr);
			interfaces::model_render->override_material(mat);
		}

		// glow
		if (menu.config.chams_type == 4)
		{
			original(thisptr, context, state, info, custom_bone_to_world);
			interfaces::model_render->override_material(mat);
			original(thisptr, context, state, info, custom_bone_to_world);
		}
	}

	original(thisptr, context, state, info, custom_bone_to_world);
	interfaces::model_render->override_material(nullptr);
}

void features::visuals::dme_misc(const model_render_info_t& info)
{
	if (!menu.config.chams)
		return;

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player || !local_player->is_alive())
		return;

	auto model_name = interfaces::model_info->get_model_name((model_t*)info.model);
	if (!model_name)
		return;

	static i_material* mat = nullptr;
	if (!mat)
		mat = interfaces::material_system->find_material("overflow_regular", TEXTURE_GROUP_MODEL);

	if (strstr(model_name, "sleeve") && menu.config.sleeve_chams)
	{
		interfaces::render_view->set_blend(menu.config.f_sleeve_chams[3]);
		interfaces::render_view->modulate_color(menu.config.f_sleeve_chams);
		mat->set_material_var_flag(material_var_ignorez, false);
		interfaces::model_render->override_material(mat);
	}	
	else if (strstr(model_name, "arms") && menu.config.arms_chams)
	{
		interfaces::render_view->set_blend(menu.config.f_arms_chams[3]);
		interfaces::render_view->modulate_color(menu.config.f_arms_chams);
		mat->set_material_var_flag(material_var_ignorez, false);
		interfaces::model_render->override_material(mat);
	}
}

void features::visuals::scene_chams_run()
{

}

