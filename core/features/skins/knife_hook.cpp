#include "knife_hook.h"
c_knifehook knife_hook;

typedef void(*recv_var_proxy_fn)(const c_recv_proxy_data* p_data, void* p_struct, void* p_out);

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1

using namespace std;
inline int RandomSequence(int low, int high) {
	return (rand() % (high - low + 1) + low);
}

#define	LIFE_ALIVE 0
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
recv_var_proxy_fn sequence_proxy_fn = nullptr;
recv_var_proxy_fn recv_model_index;

void hooked_recvproxy_viewmodel(c_recv_proxy_data* p_data, void* p_struct, void* p_out)noexcept {

	int index_default_t = interfaces::model_info->get_model_index("models/weapons/v_knife_default_t.mdl");
	int index_default_ct = interfaces::model_info->get_model_index("models/weapons/v_knife_default_ct.mdl");
	int index_bayonet = interfaces::model_info->get_model_index("models/weapons/v_knife_bayonet.mdl");
	int index_m9 = interfaces::model_info->get_model_index("models/weapons/v_knife_m9_bay.mdl");
	int index_karambit = interfaces::model_info->get_model_index("models/weapons/v_knife_karam.mdl");
	int index_bowie = interfaces::model_info->get_model_index("models/weapons/v_knife_survival_bowie.mdl");
	int index_butterfly = interfaces::model_info->get_model_index("models/weapons/v_knife_butterfly.mdl");
	int index_falchion = interfaces::model_info->get_model_index("models/weapons/v_knife_falchion_advanced.mdl");
	int index_flip = interfaces::model_info->get_model_index("models/weapons/v_knife_flip.mdl");
	int index_gut = interfaces::model_info->get_model_index("models/weapons/v_knife_gut.mdl");
	int index_huntsman = interfaces::model_info->get_model_index("models/weapons/v_knife_tactical.mdl");
	int index_shadow_daggers = interfaces::model_info->get_model_index("models/weapons/v_knife_push.mdl");
	int index_navaja = interfaces::model_info->get_model_index("models/weapons/v_knife_gypsy_jackknife.mdl");
	int index_stiletto = interfaces::model_info->get_model_index("models/weapons/v_knife_stiletto.mdl");
	int index_talon = interfaces::model_info->get_model_index("models/weapons/v_knife_widowmaker.mdl");
	int index_ursus = interfaces::model_info->get_model_index("models/weapons/v_knife_ursus.mdl");

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if ((menu.config.skins_enable) && local_player) 
	{

		if (local_player->is_alive() && (
			p_data->value.m_int == index_bayonet ||
			p_data->value.m_int == index_m9 ||
			p_data->value.m_int == index_karambit ||
			p_data->value.m_int == index_bowie ||
			p_data->value.m_int == index_butterfly ||
			p_data->value.m_int == index_falchion ||
			p_data->value.m_int == index_flip ||
			p_data->value.m_int == index_gut ||
			p_data->value.m_int == index_huntsman ||
			p_data->value.m_int == index_shadow_daggers ||
			p_data->value.m_int == index_navaja ||
			p_data->value.m_int == index_stiletto ||
			p_data->value.m_int == index_talon ||
			p_data->value.m_int == index_ursus ||
			p_data->value.m_int == index_default_t ||
			p_data->value.m_int == index_default_ct))
		{
			if (menu.config.knife_model == 0)
				p_data->value.m_int = index_default_t;
			else if (menu.config.knife_model == 0)
				p_data->value.m_int = index_default_ct;
			else if (menu.config.knife_model == 1)
				p_data->value.m_int = index_bayonet;
			else if (menu.config.knife_model == 2)
				p_data->value.m_int = index_m9;
			else if (menu.config.knife_model == 3)
				p_data->value.m_int = index_karambit;
			else if (menu.config.knife_model == 4)
				p_data->value.m_int = index_bowie;
			else if (menu.config.knife_model == 5)
				p_data->value.m_int = index_butterfly;
			else if (menu.config.knife_model == 6)
				p_data->value.m_int = index_falchion;
			else if (menu.config.knife_model == 7)
				p_data->value.m_int = index_flip;
			else if (menu.config.knife_model == 8)
				p_data->value.m_int = index_gut;
			else if (menu.config.knife_model == 9)
				p_data->value.m_int = index_huntsman;
			else if (menu.config.knife_model == 10)
				p_data->value.m_int = index_shadow_daggers;
			else if (menu.config.knife_model == 11)
				p_data->value.m_int = index_navaja;
			else if (menu.config.knife_model == 12)
				p_data->value.m_int = index_stiletto;
			else if (menu.config.knife_model == 13)
				p_data->value.m_int = index_talon;
			else if (menu.config.knife_model == 14)
				p_data->value.m_int = index_ursus;
		}
	}
	recv_model_index(p_data, p_struct, p_out);
}

void set_view_model_sequence(const c_recv_proxy_data* pDataConst, void* p_struct, void* p_out)noexcept {
	c_recv_proxy_data* p_data = const_cast<c_recv_proxy_data*>(pDataConst);
	base_view_model_t* player_view_model = static_cast<base_view_model_t*>(p_struct);

	if (player_view_model) {
		auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
		player_t* p_owner = static_cast<player_t*>(interfaces::entity_list->get_client_entity(player_view_model->m_howner() & 0xFFF));
		if (p_owner == local_player) {
			std::string sz_model = interfaces::model_info->get_model_name(interfaces::model_info->get_model(player_view_model->model_index()));
			int m_nSequence = p_data->value.m_int;
			if (sz_model == "models/weapons/v_knife_butterfly.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
				}
			}
			else if (sz_model == "models/weapons/v_knife_falchion_advanced.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
					break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (sz_model == "models/weapons/v_knife_push.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
					break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
					break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (sz_model == "models/weapons/v_knife_survival_bowie.mdl") {
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1;
					break;
				default:
					m_nSequence--;
				}
			}
			else if (sz_model == "models/weapons/v_knife_ursus.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_stiletto.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(12, 13);
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_widowmaker.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(14, 15);
					break;
				}
			}
			p_data->value.m_int = m_nSequence;
		}
	}
	sequence_proxy_fn(p_data, p_struct, p_out);
}


void knife_animation_hook()noexcept {
	for (auto client_class = interfaces::client->get_client_classes(); client_class; client_class = client_class->next_ptr) {

		if (!strcmp(client_class->network_name, "CBaseViewModel")) {
			const auto table = reinterpret_cast<recv_table*>(client_class->recvtable_ptr);
			for (int nIndex = 0; nIndex < table->props_count; nIndex++) {
				recv_prop* pProp = &table->props[nIndex];
				if (!pProp || strcmp(pProp->prop_name, "m_nSequence"))
					continue;
				sequence_proxy_fn = static_cast<recv_var_proxy_fn>(pProp->proxy_fn);
				pProp->proxy_fn = static_cast<recv_var_proxy_fn>(set_view_model_sequence);
				break;
			}
			break;
		}
	}
}

void c_knifehook::knife_animation() noexcept {
	knife_animation_hook();

	auto client_class = interfaces::client->get_client_classes();
	while (client_class) {
		const char* pszName = client_class->recvtable_ptr->table_name;
		if (!strcmp(pszName, "DT_SmokeGrenadeProjectile")) {
			for (int i = 0; i < client_class->recvtable_ptr->props_count; i++) {
				recv_prop* pProp = &(client_class->recvtable_ptr->props[i]);
				const char* name = pProp->prop_name;
				if (!strcmp(name, "m_bDidSmokeEffect")) {
				}
			}
		}
		else if (!strcmp(pszName, "DT_BaseViewModel")) {
			for (int i = 0; i < client_class->recvtable_ptr->props_count; i++) {
				recv_prop* pProp = &(client_class->recvtable_ptr->props[i]);
				const char* name = pProp->prop_name;
				if (!strcmp(name, "m_nModelIndex")) {
					recv_model_index = (recv_var_proxy_fn)pProp->proxy_fn;
					pProp->proxy_fn = (recv_var_proxy_fn)hooked_recvproxy_viewmodel;
				}
			}
		}
		client_class = client_class->next_ptr;
	}
}