#include "../../../../dependencies/common_includes.hpp"
#include "../../features.hpp"

int aim_bone[] = { 8, 7 , 6 };

void smooth(vec3_t& ViewAngle, vec3_t& Angle, float SmoothValue)
{
	Angle = (ViewAngle + (Angle - ViewAngle).clamp() / SmoothValue).clamp();
}

void simple_rcs(c_usercmd* cmd, vec3_t& angles)
{
	static vec3_t old_punch{ 0, 0, 0 };
	auto scale = interfaces::console->get_convar("weapon_recoil_scale");
	auto punch = csgo::local_player->aim_punch_angle() * 2;

	punch.x *= menu.config.standalone_rcs_x;
	punch.y *= menu.config.standalone_rcs_y;

	auto rcs_angle = cmd->viewangles += (old_punch - punch);
	interfaces::engine->set_view_angles(rcs_angle);

	old_punch = punch;
}

player_t* closest_to_crosshair(c_usercmd* user_cmd)
{
	player_t* best_entity = nullptr;
	float delta;
	float best_delta = FLT_MAX;

	for (int i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto view_angles = user_cmd->viewangles;
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity == csgo::local_player)
			continue;

		if (entity->has_gun_game_immunity() || entity->dormant() || !entity->is_alive() || !entity->is_player() || entity->health() <= 0)
			continue;

		if (entity->team() == csgo::local_player->team() && !menu.config.team_check)
			continue;

		if (menu.config.only_visible)
			if (!entity->is_visible(entity))
				continue;

		delta = math::calculate_angle(csgo::local_player->get_eye_pos(), entity->get_eye_pos(), view_angles).length();
		if (delta < best_delta && delta < menu.config.legit_fov)
		{
			best_entity = entity;
			best_delta = delta;
		}
	}
	return best_entity;
}

player_t* closest_distance(c_usercmd* user_cmd)
{
	player_t* best_entity = nullptr;
	float distance;
	float best_distance = FLT_MAX;

	for (int i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto view_angles = user_cmd->viewangles;
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity == csgo::local_player)
			continue;

		if (entity->has_gun_game_immunity() || entity->dormant() || !entity->is_alive() || !entity->is_player() || entity->health() <= 0)
			continue;

		if (entity->team() == csgo::local_player->team() && !menu.config.team_check)
			continue;

		if (menu.config.only_visible)
			if (!entity->is_visible(entity))
				continue;
		
		auto delta = math::calculate_angle(csgo::local_player->get_eye_pos(), entity->get_eye_pos(), view_angles).length();
		distance = entity->abs_origin().distance_to(csgo::local_player->abs_origin());
		if (distance < best_distance && delta < menu.config.legit_fov)
		{
			best_entity = entity;
			best_distance = distance;
		}
	}
	return best_entity;
}

player_t* lowest_health(c_usercmd* user_cmd)
{
	player_t* best_entity = nullptr;
	int health;
	int best_health = 1000.f;

	for (int i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto view_angles = user_cmd->viewangles;
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity == csgo::local_player)
			continue;

		if (entity->has_gun_game_immunity() || entity->dormant() || !entity->is_alive() || !entity->is_player() || entity->health() <= 0)
			continue;

		if (entity->team() == csgo::local_player->team() && !menu.config.team_check)
			continue;

		if (menu.config.only_visible)
			if (!entity->is_visible(entity))
				continue;

		auto delta = math::calculate_angle(csgo::local_player->get_eye_pos(), entity->get_eye_pos(), view_angles).length();
		health = entity->health();
		if (health < best_health && delta < menu.config.legit_fov)
		{
			best_entity = entity;
			best_health = health;
		}
	}
	return best_entity;
}

void features::aimbot::legit::run(c_usercmd* user_cmd)
{
	if (!menu.menu_opened)
	{
		if (menu.config.standalone_rcs)
			simple_rcs(user_cmd, user_cmd->viewangles);

		if (menu.config.legit_always_on
			|| menu.config.legit_pair.second)
		{
			if (!csgo::local_player->is_alive())
				return;

			const auto weapon_type = csgo::local_player->active_weapon()->get_weapon_data()->m_iWeaponType;
			if (weapon_type == WEAPONTYPE_GRENADE || weapon_type == WEAPONTYPE_C4 || weapon_type == WEAPONTYPE_KNIFE)
				return;

			player_t* entity = nullptr;
			int bone = 0;
			switch (menu.config.legit_selection)
			{
			case 0:
				entity = closest_to_crosshair(user_cmd); break;
			case 1: 
				entity = closest_distance(user_cmd); break;
			case 2:
				entity = lowest_health(user_cmd); break;
			default:
				break;
			}

			if (!entity)
				return;

			bone = aim_bone[menu.config.legit_bone];

			auto angle = math::calculate_angle(csgo::local_player->get_eye_pos(), entity->get_bone_position(bone), user_cmd->viewangles);
			if (menu.config.simple_rcs)
				angle -= csgo::local_player->aim_punch_angle() * 2;

			angle = angle.clamp();

			if (menu.config.smoothing > 0)
				angle /= (menu.config.smoothing * 5);

			angle = math::normalize(angle);

			user_cmd->viewangles += angle;

			if (!menu.config.legit_silent)
				interfaces::engine->set_view_angles(user_cmd->viewangles);
		}
	}
}