#include "../../../dependencies/common_includes.hpp"
#include "../features.hpp"
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF


void features::skins::run()
{
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	auto localplayer = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!localplayer)
		return;

	auto my_weapons = localplayer->get_weapons();
	for (int i = 0; my_weapons[i] != INVALID_EHANDLE_INDEX; i++)
	{
		auto weapon = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(my_weapons[i]));
		if (!weapon)
			continue;
		
		if (weapon->client_class()->class_id == class_ids::cak47)
			weapon->fallback_paint_kit() = 12;		
		else if (weapon->client_class()->class_id == class_ids::cweaponm4a1)
			weapon->fallback_paint_kit() = 2;

		weapon->original_owner_xuid_low() = 0;
		weapon->original_owner_xuid_high() = 0;
		weapon->fallback_seed() = 661;
		weapon->item_id_high() = -1;
	}
}