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
		void scene_chams_run();
	};
	namespace skins {
		void run();
	}
}