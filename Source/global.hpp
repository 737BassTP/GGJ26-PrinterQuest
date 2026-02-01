#pragma once

//Game mode.
enum class gamemode_e
{
	MENU,
	TUTORIAL,
	GAME,
	CREDITS,
	GAMEMODE_COUNT
};
int global_gamemode_id = 0;
int global_gamemode_id_prev = global_gamemode_id;
inline int global_gamemode_get() { return global_gamemode_id; }
inline int global_gamemode_count() { return (int)gamemode_e::GAMEMODE_COUNT; }
inline bool global_gamemode_is_menu() { return global_gamemode_id == (int)gamemode_e::MENU; }
inline bool global_gamemode_is_game() { return global_gamemode_id == (int)gamemode_e::GAME; }
inline bool global_gamemode_is_credits() { return global_gamemode_id == (int)gamemode_e::CREDITS; }
inline bool global_gamemode_is_tutorial() { return global_gamemode_id == (int)gamemode_e::TUTORIAL; }
void global_gamemode_update()
{
	if (global_gamemode_id != global_gamemode_id_prev)
	{
		if (global_gamemode_is_menu())
		{
			music.load("assets/m_menu.mp3");
			music.volume(0.75f);
		}
		else if (global_gamemode_is_game())
		{
			music.load("assets/m_level1.mp3");
			music.volume(1.0f);
		}
		else if (global_gamemode_is_credits())
		{
			music.load("assets/m_credits.mp3");
			music.volume(0.75f);
		}
		else if (global_gamemode_is_tutorial())
		{
			//music.load("assets/m_credits.mp3");
			//music.volume(0.75f);

		}
		else
		{
			PrintError("global_gamemode_set","Missing function; else triggered!\n");
		}
		if (not global_gamemode_is_tutorial())
		{
			music.play();
		}
	}
	global_gamemode_id_prev = global_gamemode_id;
}
void global_gamemode_set(gamemode_e v) { global_gamemode_id = (int)v; global_gamemode_update(); }
void global_gamemode_set(int v) { global_gamemode_id = v; global_gamemode_update();
}







