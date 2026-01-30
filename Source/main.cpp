#define SDL_MAIN_HANDLED

//#define WEB

#ifdef WEB
#include <emscripten.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#endif

#include <stdio.h>
#include <string>
//#include <>
//#include <>
//#include <>


int vW = 1366;
int vH = 768;
int gW = 512;
int gH = 288;

void PrintError(const char* msg1, const char* msg2) { printf("%s: %s\n",msg1,msg2); }

#include "functions.hpp"
#include "global.hpp"
#include "sdl.hpp"
#include "menu.hpp"
#include "game.hpp"
#include "credits.hpp"
//#include ".hpp"
//#include ".hpp"


int main(int argc, char* argv[])
{
	//printf("test! \n");
	sdl_init();

	menu_init();
	game_init();
	credits_init();

	sdl_loop();

	menu_free();
	game_free();
	credits_free();

	sdl_free();
	return 0;
}
