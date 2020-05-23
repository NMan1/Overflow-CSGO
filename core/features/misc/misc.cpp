#include "../features.hpp"
#include "thirdperson.h"

void features::misc::bunny_hop(c_usercmd* cmd) 
{
	static bool last_jumped = false, should_fake = false;

	if (!last_jumped && should_fake) 
	{
		should_fake = false;
		cmd->buttons |= in_jump;
	}
	else if (cmd->buttons & in_jump) 
	{
		std::random_device randomDevice;
		std::mt19937 generate(randomDevice());
		std::uniform_int_distribution<> chance(0, 100);

		if (chance(generate) > menu.config.bhop_chance)
			return;

		if (csgo::local_player->flags() & fl_onground) 
		{
			last_jumped = true;
			should_fake = true;
		}
		else 
		{
			cmd->buttons &= ~in_jump;
			last_jumped = false;
		}
	}
	else 
	{
		last_jumped = false;
		should_fake = false;
	}
}

void features::misc::auto_strafer(c_usercmd* cmd)
{
	static bool flip = true;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (csgo::local_player->flags() & fl_onground)
		return;
	
	cmd->forwardmove = 0.0f;
	cmd->sidemove = 0.0f;

	if (cmd->mousedx < 0)
	{
		cmd->sidemove = -450.0f;
	}
	else if (cmd->mousedx > 0)
	{
		cmd->sidemove = 450.0f;
	}
	else
	{
		if (flip)
		{
			cmd->viewangles = math::normalize(vec3_t(cmd->viewangles.x, cmd->viewangles.y - 1.0f, 0.0f));
			cmd->sidemove = -450.0f;
			flip = false;
		}
		else
		{
			cmd->viewangles = math::normalize(vec3_t(cmd->viewangles.x, cmd->viewangles.y + 1.0f, 0.0f));
			cmd->sidemove = 450.0f;
			flip = true;
		}
	}
}

void features::misc::movement_blocker(c_usercmd* cmd)
{
	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	auto best_dist = 900.f;
	player_t* best_entity = nullptr;
	for (int i = 0; i < interfaces::globals->max_clients; i++)
	{
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity)
			continue;

		if (entity == csgo::local_player)
			continue;

		if (!entity->is_alive() || entity->dormant())
			continue;

		auto distance = csgo::local_player->origin().distance_to(entity->origin());
		if (distance < best_dist)
		{
			best_dist = distance;
			best_entity = entity;
		}
	}

	if (!best_entity || !csgo::local_player)
		return;

	vec3_t angle;
	auto delta = csgo::local_player->origin() - best_entity->origin();
	math::vector_angles(delta, angle);

	angle.y -= csgo::local_player->eye_angles().y;
	angle = math::normalize(angle);

	if (angle.y < 0.0f)
		cmd->sidemove = 450.f;
	else if (angle.y > 0.0f)
		cmd->sidemove = -450.f;
}

void features::misc::crouch_blocker(c_usercmd* cmd)
{
	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	player_t* ground_entity = (player_t*)interfaces::entity_list->get_client_entity_handle(csgo::local_player->ground_entity());

	// Check if there's a player under us.
	if (ground_entity && ground_entity->client_class()->class_id == class_ids::ccsplayer)
	{
		// Get the target's speed.
		vec3_t Velocity = ground_entity->velocity();
		const auto Speed = Velocity.Length2D();

		if (Speed > 0.0f)
		{
			// Get the angles direction based on the target's speed.
			vec3_t Direction;
			math::vector_angles(Velocity, Direction);
		
			vec3_t ViewAngles;
			interfaces::engine->get_view_angles(ViewAngles);

			// Cut down on our viewangles.
			Direction.y = ViewAngles.y - Direction.y;

			// Transform into vector again.
			vec3_t Forward;
			math::angle_vector(Direction, Forward);

			// Calculate the new direction based on the target's speed.
			vec3_t NewDirection = Forward * -Speed;

			// Move accordingly.
			cmd->forwardmove = NewDirection.x;
			cmd->sidemove = NewDirection.y;
		}
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

float best_cam_dist() 
{
	ray_t ray;
	trace_t tr;
	trace_world_only filter;
	vec3_t forward, backward, end;
	filter.pSkip1 = csgo::local_player;
	vec3_t va;
	interfaces::engine->get_view_angles(va);

	math::angle_vectors_alternative(va, &forward);
	backward = forward * -1;
	backward *= menu.config.third_person_distance;
	end = csgo::local_player->get_eye_pos() + backward;

	ray.initialize(csgo::local_player->get_eye_pos(), end);

	interfaces::trace_ray->trace_ray(ray, MASK_SHOT_HULL, &filter, &tr);

	if (!tr.entity)
		return menu.config.third_person_distance;

	return (tr.end - csgo::local_player->get_eye_pos()).length() - 20.f;
}

void features::misc::thirdperson()
{
	if (!csgo::local_player)
		return;

	interfaces::input->m_fCameraInThirdPerson = menu.config.third_person && menu.config.thirdperson_pair.second && csgo::local_player->is_alive();
	if (!interfaces::input->m_fCameraInThirdPerson)
		return;

	auto weapon = csgo::local_player->active_weapon();

	const auto weapon_type = weapon->get_weapon_data()->m_iWeaponType;

	if ((weapon_type == WEAPONTYPE_GRENADE || weapon_type == WEAPONTYPE_C4))
		interfaces::input->m_fCameraInThirdPerson = false;

	if ((weapon_type == WEAPONTYPE_PISTOL || weapon_type == WEAPONTYPE_MACHINEGUN || weapon_type == WEAPONTYPE_RIFLE || weapon_type == WEAPONTYPE_SHOTGUN || weapon_type == WEAPONTYPE_SNIPER_RIFLE || weapon_type == WEAPONTYPE_SUBMACHINEGUN) && menu.config.third_disable_on_weapon)
		interfaces::input->m_fCameraInThirdPerson = false;

	vec3_t angles;
	interfaces::engine->get_view_angles(angles);

	vec3_t backward(angles.x, angles.y + 180.f, angles.z);
	backward = backward.clamp();
	backward.normalize();

	vec3_t range;
	math::angle_vectors_alternative(backward, &range);
	range *= 8192.f;

	auto start = csgo::local_player->get_eye_pos();

	trace_filter filter;
	filter.skip = csgo::local_player;

	ray_t ray;
	ray.initialize(start, start + range);

	trace_t tr;
	interfaces::trace_ray->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);
	angles.z = std::min<int>(start.distance_to(tr.end), best_cam_dist()); // 150 is better distance

	interfaces::input->m_vecCameraOffset = angles;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void features::misc::auto_pistol(c_usercmd* cmd)
{
	if (!csgo::local_player)
		return;

	if (!csgo::local_player->is_alive())
		return;

	auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	static bool fire = false;
	if (weapon->get_weapon_data()->m_iWeaponType == WEAPONTYPE_PISTOL && weapon->item_definition_index() != WEAPON_REVOLVER && cmd->buttons & in_attack)
	{
		if (fire)
			cmd->buttons &= ~in_attack;
		fire = cmd->buttons & in_attack ? true : false;
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

vec3_t quick_peck_start_pos{};
bool has_shot;

void goto_start(c_usercmd* cmd) 
{
	if (!csgo::local_player || csgo::local_player->dormant() || !csgo::local_player->is_alive()) 
		return;

	vec3_t playerLoc = csgo::local_player->abs_origin();

	float yaw = cmd->viewangles.y;
	vec3_t VecForward = playerLoc - quick_peck_start_pos;
	vec3_t translatedVelocity = vec3_t{
		(float)(VecForward.x * cos(yaw / 180 * (float)M_PI) + VecForward.y * sin(yaw / 180 * (float)M_PI)),
		(float)(VecForward.y * cos(yaw / 180 * (float)M_PI) - VecForward.x * sin(yaw / 180 * (float)M_PI)),
		VecForward.z
	};

	cmd->forwardmove = -translatedVelocity.x * 20.f;
	cmd->sidemove = translatedVelocity.y * 20.f;

	if (translatedVelocity.x == 0)
		menu.config.do_quick_peek = false;
}

void features::misc::quick_peak(c_usercmd* cmd)
{
	if (!csgo::local_player || csgo::local_player->dormant() || !csgo::local_player->is_alive()) 
		return;

	if (menu.config.do_quick_peek)
	{
		if (quick_peck_start_pos.x == NULL && quick_peck_start_pos.y == NULL && quick_peck_start_pos.z == NULL)
			quick_peck_start_pos = csgo::local_player->abs_origin();
		else 
		{
			if (cmd->buttons & in_attack) 
				has_shot = true;
			if (has_shot)
				goto_start(cmd);
		}
	}
	else 
	{
		has_shot = false;
		quick_peck_start_pos = vec3_t{ 0, 0, 0 };
	}
}

void features::misc::shoot_gun(c_usercmd* cmd)
{
	player_t* best_entity = nullptr;
	for (int i = 1; i < interfaces::entity_list->get_highest_index(); i++)
	{
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		
		if (!entity)
			continue;

		if (entity->is_player())
			continue;

		if (!entity->client_class()->class_id)
			continue;

		auto model_name = interfaces::model_info->get_model_name(entity->model());
		if (strstr(model_name, "models/weapons/w_") && strstr(model_name, "_dropped.mdl"))
		{
			float distance;
			float best_distance = FLT_MAX;

			auto delta = math::calculate_angle(csgo::local_player->get_eye_pos(), entity->get_eye_pos(), cmd->viewangles).length();
			distance = entity->abs_origin().distance_to(csgo::local_player->abs_origin());
			if (distance < best_distance && delta < 40)
			{
				best_entity = entity;
				best_distance = distance;
			}
		}
	}

	if (best_entity)
	{
		auto pos = best_entity->abs_origin();
		vec3_t pos_2d = {};
		auto angle = math::calculate_angle(csgo::local_player->get_eye_pos(), pos, cmd->viewangles);
		angle.normalize();

		cmd->viewangles += angle;
	}
}
 
void features::misc::draw_start_pos()
{
	if (quick_peck_start_pos.x != NULL && quick_peck_start_pos.y != NULL && quick_peck_start_pos.z != NULL)
	{
		vec3_t out;
		if (interfaces::debug_overlay->world_to_screen(quick_peck_start_pos, out))
			render::draw_circle(out.x, out.y, 30, 60, color(0, 0, 0));
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DWORD* offset = NULL;
typedef int(__fastcall* set_clan_tag_fn)(const char*, const char*);
size_t pos = 0;
std::string clantag;
float last_time = 0;

void features::misc::set_clan_tag(const char* tag)
{
	if (!offset)
		offset = (DWORD*)utilities::pattern_scan(GetModuleHandleA("engine.dll"), "53 56 57 8B DA 8B F9 FF 15");
	reinterpret_cast<set_clan_tag_fn>(offset)(tag, "Overflow");
}

void features::misc::animated_clan_tag(const char* tag)
{
	if (clantag != menu.config.clan_tag || clantag.length() < pos)
	{
		clantag = menu.config.clan_tag;
		pos = 0;
	}

	if (last_time + menu.config.clan_tag_delay > interfaces::globals->realtime)
		return;

	last_time = interfaces::globals->realtime;

	reinterpret_cast<set_clan_tag_fn>(offset)(clantag.substr(0, pos).c_str(), "Overflow");
	pos++;
}