#include "../../../dependencies/common_includes.hpp"
#include "../features.hpp"

int trigger_bone[] = { 0, hitgroup_head, hitgroup_chest, hitgroup_stomach };

void features::aimbot::run_trigger(c_usercmd* user_cmd)
{
	if (menu.menu_opened)
		return;

	if (!csgo::local_player)
		return;

	auto wep = csgo::local_player->active_weapon();
	if (!wep)
		return;

	if (!wep->clip1_count())
		return;

	if (wep->get_weapon_data()->m_iWeaponType == WEAPONTYPE_GRENADE || wep->get_weapon_data()->m_iWeaponType == WEAPONTYPE_KNIFE)
		return;

	if (menu.config.trigger_bot_pair.second || menu.config.trigger_bot_always_on)
	{
		ray_t ray;
		trace_filter filter;
		trace_t trace;
		vec3_t start, end, forward;

		math::angle_vectors_alternative(user_cmd->viewangles, &forward);

		forward *= csgo::local_player->active_weapon()->get_weapon_data()->m_flWeaponRange;
		start = csgo::local_player->get_eye_pos();
		end = start + forward;
		filter.skip = csgo::local_player;
		ray.initialize(start, end);

		interfaces::trace_ray->trace_ray(ray, 0x46004003, &filter, &trace);

		auto player = trace.entity;
		if (!player)
			return;

		if (player->client_class()->class_id != ccsplayer)
			return;

		if (trace.entity->team() == csgo::local_player->team())
			return;

		if (menu.config.trigger_bone == 0)
			user_cmd->buttons |= in_attack;
		else if (menu.config.trigger_bone == 4 && (trace.hitGroup == hitgroup_chest || trace.hitGroup == hitbox_stomach))
			user_cmd->buttons |= in_attack;
		else if (trace.hitGroup == trigger_bone[menu.config.trigger_bone])
			user_cmd->buttons |= in_attack;
		else
			return;
	}
}