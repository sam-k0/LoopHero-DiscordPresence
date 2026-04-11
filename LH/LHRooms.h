#pragma once
namespace LHRooms {

	enum LHRoom {
		rm_titles = 0,
		rm_creater_dialog = 1,
		rm_piramidka = 2,
		rm_load = 3,
		rm_intro = 4,
		rm_camp = 5,
		rm_tutor = 6,
		rm_game = 7,
		rm_cutscenes = 8,
		rm_music_maker = 9
	};

	const char* GetRoomName(int id) {
		static const char* names[] = {
			"rm_titles",
			"rm_creater_dialog",
			"rm_piramidka",
			"rm_load",
			"rm_intro",
			"rm_camp",
			"rm_tutor",
			"rm_game",
			"rm_cutscenes",
			"rm_music_maker"
		};
		if (id < 0 || id > 9) return "Unknown";
		return names[id];
	}
}