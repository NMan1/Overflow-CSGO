#pragma once
#include "../../dependencies/common_includes.hpp"

namespace features {
	namespace aimbot {
		namespace legit {
			void run(c_usercmd* user_cmd);
		};
		void run_trigger(c_usercmd* user_cmd);
	};
	namespace misc {
		void bunny_hop(c_usercmd* cmd);
		void auto_strafer(c_usercmd* cmd);
		void movement_blocker(c_usercmd* cmd);
		void crouch_blocker(c_usercmd* cmd);
		void thirdperson();
		void auto_pistol(c_usercmd* cmd);
		void quick_peak(c_usercmd* cmd);
		void draw_start_pos();
		void set_clan_tag(const char* tag);
		void animated_clan_tag(const char* tag);
	};
	namespace visuals {
		void render_esp();
		void render_visuals();
		void dme_chams_run(hooks::dme::fn original, void* thisptr, void* context, void* state, const model_render_info_t& info, matrix3x4_t* custom_bone_to_world);
		void dme_misc(const model_render_info_t& info);
		void scene_chams_run();
	};
	namespace skins {
		extern std::string file_skins;
		int find_paint_kit(int item_definition_index, bool is_knife);
		int find_knife_model(int item_definition_index);
		void replace_paint_model_kit(int item_definition_index, int paint_kit, int knife_index = -1);
		void run();
		void create();
		void save();
		void load();
	}
}