#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include <string>
#include <vector>
//#include <>
//#include <>

#include <windows.h>
#include <winspool.h>	//printer
//#include <wia.h>        //scanner (faulty in non-MSVC)
#include <objbase.h>
#include <oleauto.h>

SDL_Window* window;
SDL_Renderer* renderer;

int vW = 1366;
int vH = 768;
int gW = 512;
int gH = 288;

void PrintError(const char* msg1, const char* msg2) { printf("%s: %s\n",msg1,msg2); }
SDL_Surface* scanned = nullptr;

#include "defines.hpp"
#include "functions.hpp"
#include "global.hpp"
#include "sdl.hpp"
#include "io.hpp"
#include "printer.hpp"
#include "menu.hpp"
#include "game.hpp"
#include "credits.hpp"
//#include ".hpp"
//#include ".hpp"
//#include ".hpp"

struct music_t
{
	music_t() = default;
	~music_t() { free(); }
	Mix_Music* music = nullptr;
	void load(const char* fp)
	{
		music = Mix_LoadMUS(fp);
	}
	void free()
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = nullptr;
	}
	void play()
	{
		Mix_PlayMusic(music, -1);
	}
	void volume(float vol_nor)
	{
		Mix_VolumeMusic(lerp<int>(0, MIX_MAX_VOLUME,vol_nor/1.0f));
	}
};



bool got_scan = 0;
int main(int argc, char* argv[])
{
	printf("test! \n");
	sdl_init();

	if (0)
	{
		image_t printer_test;
		printer_test.load("assets/s_printer_test_color.png");
		print_image("GGJ26 Printer Job", printer_test.surf, 0, 1);
		printer_test.free();
	}
	if (0)
	{
		scanned = scanner_get();
		SDL_SaveBMP(scanned, "scanned.bmp");
	}

	music_t mus;
	mus.load("assets/m_level1.mp3");
	//mus.load("assets/m_level1.wav");
	mus.play();

	global_menu_set(gamemode_e::MENU);

	menu_init();
	game_init();
	credits_init();

	sdl_loop();

	menu_free();
	game_free();
	credits_free();

	mus.free();
	sdl_free();
	return 0;
}
