#pragma once

//Game mode.
enum class gamemode_e
{
	MENU,
	GAME,
	CREDITS
};
int global_gamemode_id = 0;
void global_menu_set(gamemode_e v) { global_gamemode_id = (int)v; }
inline int global_gamemode_get() { return global_gamemode_id; }
inline bool global_gamemode_is_menu() { return global_gamemode_id == (int)gamemode_e::MENU; }
inline bool global_gamemode_is_game() { return global_gamemode_id == (int)gamemode_e::GAME; }
inline bool global_gamemode_is_credits() { return global_gamemode_id == (int)gamemode_e::CREDITS; }








