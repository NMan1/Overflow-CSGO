#include "../../../dependencies/common_includes.hpp"
#include "../features.hpp"


std::vector<std::string> mat_names_model = {
	"debug/debugambientcube", // Normal Material            
	"metallic_chams",                       // Metallic Material
	"cherry_flat",                                  // Red Material
};

std::vector<std::string> mat_names_other = {
	"models/inventory_items/trophy_majors/crystal_clear",               // Crystal Clear Material
	"models/inventory_items/trophy_majors/crystal_blue",                // Crystal Blue Matrial
	"models/gibs/glass/glass",
	"models/inventory_items/trophy_majors/gold",
	"models/inventory_items/cologne_prediction/cologne_prediction_glass",
	"models/inventory_items/wildfire_gold/wildfire_gold_detail",
	"models/inventory_items/trophy_majors/velvet",
	"models/inventory_items/music_kit/darude_01/mp3_detail",
	"models/extras/speech_info",
	"models/weapons/customization/stickers/cologne2014/esl_c",
	"models/inventory_items/contributor_map_tokens/contributor_charset_color",
	"models/inventory_items/dreamhack_trophies/dreamhack_star_blur",
	"models/player/t_guerilla/t_guerilla",
	"models/inventory_items/trophy_majors/silver_winners",
	"models/props_shacks/fishing_net01",
	"models/player/ct_fbi/ct_fbi_glass",                                                    // Platinum Material
	"models/inventory_items/trophy_majors/gloss",                           // Gloss Material
	"vgui/achievements/glow",                                                                   // Glow Material
	"models/inventory_items/dogtags/dogtags_outline",                   // Dogtags Outline Material
	"models/inventory_items/dogtags/dogtags_lightray",              // Dogtags Lightray Material
	"models/inventory_items/dogtags/dogtags",                               // Dogtags Material
	"models/inventory_items/hydra_crystal/hydra_crystal",               // Hydra Crystal Material
	"models/inventory_items/hydra_crystal/hydra_crystal_detail", // Hydra Crystal Detail Material

};

void features::visuals::chams_run(hooks::dme::fn original, void* thisptr, void* context, void* state, const model_render_info_t& info, matrix3x4_t* custom_bone_to_world)
{
	if (!menu.config.chams)
		return;
	else
		return original(thisptr, context, state, info, custom_bone_to_world);

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	if (!info.model)
		return;

	// get our entity
	auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
	if (!entity || !entity->is_alive() || entity->dormant())
		return;
	
	if (strstr(interfaces::model_info->get_model_name(info.model), "models/player"))
	{
		static i_material* mat = nullptr;
		if (!mat)
			mat = interfaces::material_system->find_material("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER); //you can create your own material to really make it shiny, but i like this subtle one
		mat->increment_reference_count();

		if (entity->team() != local_player->team() && entity != local_player)
		{
			interfaces::render_view->set_blend(menu.config.chams_clr.a);
			interfaces::render_view->modulate_color(menu.config.f_chams_clr);
			interfaces::model_render->override_material(mat);
		}
	}
	original(thisptr, context, state, info, custom_bone_to_world);
	interfaces::model_render->override_material(nullptr);
}

