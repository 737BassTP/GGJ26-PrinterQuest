#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

/*
extern "C"
{
	#include <libavformat/avformat.h>
	#include <libavcodec/avcodec.h>
}
/**/

#include <array>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
SDL_Texture* texture;
bool running = 1;
SDL_PixelFormatEnum pixelformatenum = SDL_PIXELFORMAT_RGBA32;
SDL_Surface* scanned = nullptr;
bool mute_audio = 0;
const char* name_game = "Printer Quest";

int vW = 1366;
int vH = 768;
int gW = 512;
int gH = 288;
int FPS = 60;
int tile_size = 32;
int world_size = 128;
int world_dim = tile_size * world_size;

#include "defines.hpp"
#include "functions.hpp"
music_t music;
#include "global.hpp"
#include "printer.hpp"
font_t font;

struct printmask_t
{
	printmask_t() = default;
	~printmask_t() { free(); }
	image_t img;
	void load(const char* fp) { img.load(fp); }
	void free() { img.free(); }
	void print(int maskid=0)
	{
		maskid %= img.frames;
		SDL_Surface* mir = SDL_CreateRGBSurface(0, img.w * 2, img.h, 32, 0, 0, 0, 0);

		SDL_FreeSurface(mir);
		std::string msg = "";
		int rv = random(100);
		if (rv > 50) { msg += "Common"; }
		if (rv < 40) { msg += "Uncommon"; }
		if (rv < 30) { msg += "Rare"; }
		if (rv < 20) { msg += "Epic"; }
		if (rv < 10) { msg += "Legendary"; }
		msg += ": ";
		msg += "LVL. ";
		msg += std::to_string(random(100));
		print_image("Print mask", mir, 1, 0, msg.c_str());
	}

};

int main(int argc, char* argv[])
{
	srand(NULL);
	int r = rand();
goto_init_sdl:
	Uint32 flags_sdl = SDL_INIT_EVERYTHING;
	SDL_Init(flags_sdl);
	Uint32 flags_window = SDL_WINDOW_RESIZABLE;
	std::string wintitle = "";
	wintitle += "GGJ26: ";
	wintitle += name_game;
	window = SDL_CreateWindow(wintitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, vW, vH, flags_window);
	if (window == NULL)
	{
		PrintError("SDL_CreateWindow()", SDL_GetError());
		return 1;
	}
	Uint32 flags_renderer = SDL_RENDERER_ACCELERATED;
	renderer = SDL_CreateRenderer(window, -1, flags_renderer);
	if (renderer == NULL)
	{
		PrintError("SDL_CreateRenderer()", SDL_GetError());
		return 2;
	}
	int flags_mixer = 0;
	flags_mixer |= MIX_INIT_MP3;
	if (Mix_Init(flags_mixer) == 0)
	{
		PrintError("Mix_Init()", SDL_GetError());
		return 3;
	}
	int chunksize = 1 < 11;//pow(2,11) = 2048
	Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, chunksize);
	int flags_allow = SDL_AUDIO_ALLOW_ANY_CHANGE;
	int moad = Mix_OpenAudioDevice(48000, AUDIO_F32SYS, 2, chunksize, NULL, flags_allow);
	if (moad == -1)
	{
		PrintError("Mix_OpenAudioDevice()", SDL_GetError());
		return 4;
	}
goto_init_sdl_more:
	texture = SDL_CreateTexture(renderer, pixelformatenum, SDL_TEXTUREACCESS_TARGET, gW, gH);
	Uint32 timer = 0;
goto_init_other:
	//font.load("assets/s_font_strip96.png");
	//font.load("assets/s_font_v11_strip96.png");
	leveldata_t leveldata;
	//leveldata.load("assets/level-raw.dat");


goto_init_menu:
	image_t menu_gradient;
	menu_gradient.load("assets/s_menu_final.png");
	//menu_gradient.load("assets/s_menu_logo_strip3.png");

	if (1)
	{
		if (0) { print_text("Some document", "Text string from GGJ26.exe"); }
		if (0)
		{
			image_t img;
			//img.load("assets/print_menu.png");
			//print_image("Some image",img.surf,0,0);
		}
		if (0)
		{
			SDL_Surface* tmp = scanner_get();
			SDL_SaveBMP(tmp,"scanned.bmp");
		}
	}

goto_init_game:
	image_t player_img;
	player_img.load("assets/s_player.png");
	float player_angle_spd = 0.015f;
	float player_angle_move = 0.0f;
	float player_angle_view = 0.0f;
	float player_angle_view_deg = 0.0f;
	float player_speed_cur = 0.0f;
	float player_speed_spd = 0.5f;
	float player_speed_max = 9.0f;
	float player_speed_decay = 4.0f / (float)FPS;
	float player_x = 128.0f;
	float player_y = 128.0f;
	view_t view;

	image_t mask_big;
	mask_big.load("assets/s_masks_big_whole_printerready_strip64.png");
	int mask_big_frames = mask_big.frames;
	image_t world_img;
	world_img.load("assets/world.png");
	image_t proj_bullet;
	//proj_bullet.load("assets/s_proj_bullet_strip8.png");
#define obj_mask_funcsign std::vector<std::vector<int>>
	obj_mask_funcsign obj_masks;
	int obj_masks_num = 99;
	for (int i = 0; i < obj_masks_num; i++)
	{
		/*
		obj_mask_funcsign obj;
		obj
		obj_masks.push_back(obj);
		/**/
	}


goto_init_credits:
	image_t credits_back;
	credits_back.load("assets/s_credits_final.png");
goto_init_tutorial:
	image_t tutorial_back;
	tutorial_back.load("assets/s_tutorial_final.png");

goto_loop:
	global_gamemode_set(gamemode_e::CREDITS);
	global_gamemode_set(gamemode_e::MENU);//force update().

	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT: { running = 0; } break;

			}
		}
		//Input.
		keyboard_update();
		//if (K(1, vk_f12)) { screenshot_save("scrshot.png"); }
		if (K(1, vk_escape))
		{
			if (not global_gamemode_is_menu())
			{
				global_gamemode_set(gamemode_e::MENU);
			}
			else
			{
				running = 0;
			}
		}
		if (K(1, vk_m) and KMOD(vk_shift))
		{
			mute_audio ^= 1;
			music.volume((float)mute_audio);
		}

		//Debug input
		if (1)
		{
			int d = K(1, vk_8) - K(1,vk_9);
			if (d != 0)
			{
				int m = global_gamemode_get();
				int gc = global_gamemode_count();
				m = (m + d) % gc;
				if (m < 0) { m += gc; }

				global_gamemode_set(m);
			}
		}

		//Update.
		timer = get_timer();

		if (global_gamemode_is_menu())
		{
			
			if (K(1, vk_enter))
			{
				global_gamemode_set(gamemode_e::TUTORIAL);
				//kbarr_cur[vk_space] = 0;
				//kbarr_prev[vk_space] = 0;

			}


		}
		if (global_gamemode_is_game())
		{
			if (K(1, vk_enter))
			{
				if (KMOD(vk_shift))
				{
					global_gamemode_set(gamemode_e::CREDITS);
				}
				else
				{
					int mid = random(64) % 64;
					int mih, mifw;
					mih = mask_big.h;
					mifw = mask_big.fw;
					SDL_Surface* masksurf = SDL_CreateRGBSurface(0, mifw, mih, 32, 0, 0, 0, 0);
					SDL_Rect srcrect = rectangle(mid*mifw,0,mifw,mih);
					SDL_BlitSurface(mask_big.surf, &srcrect, masksurf, NULL);

					std::string msg = "";
					int rv = random(100);
					     if (rv > 50) { msg += "Common"; }
					else if (rv < 40) { msg += "Uncommon"; }
					else if (rv < 30) { msg += "Rare"; }
					else if (rv < 20) { msg += "Epic"; }
					else if (rv < 10) { msg += "Legendary"; }
					msg += ": ";
					msg += "LVL. ";
					msg += std::to_string(random(100));

					std::string msgsrc = "";
					msgsrc += "A 'mask' from: ";
					msgsrc += name_game;
					msgsrc += " (GGJ26)";

					print_image(msgsrc.c_str(), masksurf, 0, 0, msg.c_str());
					SDL_FreeSurface(masksurf);
				}
			}

			//Player speed.
			int kh = K(0, vk_left) - K(0, vk_right);
			int kv = K(0, vk_up) - K(0, vk_down);
			bool kk = (kh != 0) || (kv != 0);
			player_speed_cur += kv * player_speed_spd * (player_speed_cur < player_speed_max);
			if (not kv) { player_speed_cur -= player_speed_decay * (player_speed_cur > 0.0f); }
			//Player direction.
			if (kh)
			{
				player_angle_view -= (float)kh * player_angle_spd;
				player_angle_view = fmodf(player_angle_view, 1.0f);
				player_angle_view_deg = 180.0f+lerp<float>(0.0f,360.0f,player_angle_view);

			}
			//Player movement.
			if (player_speed_cur > 0.0f)
			{
				player_x += player_speed_cur * +1.0f * cosf(lerp<float>(0.0f, 2.0f * M_PI, player_angle_view / 1.0f));
				player_y += player_speed_cur * +1.0f * sinf(lerp<float>(0.0f, 2.0f * M_PI, player_angle_view / 1.0f));
			}
			int xlim = view.w/2;
			int ylim = view.h/2;
			player_x = (player_x < xlim) ? xlim : (player_x > (world_dim-xlim)) ? (world_dim-xlim) : player_x;
			player_y = (player_y < ylim) ? ylim : (player_y > (world_dim-ylim)) ? (world_dim-ylim) : player_y;
			
			//Debug print.
			if (K(1, vk_enter))
			{
				//printf("px = %i\npy = %i\n",(int)player_x,(int)player_y);



			}

			//View/camera.
			view.x = (int)player_x - xlim;
			view.y = (int)player_y - ylim;
			


		}
		if (global_gamemode_is_credits())
		{
			//font.draw(32,96,"CREDITS:##Thomas Pedersen#- Programmer#- Audio/Music#- Graphics");

			if (K(1, vk_space))
			{
				global_gamemode_set(gamemode_e::MENU);

			}
		}
		if (global_gamemode_is_tutorial())
		{
			//font.draw(32,96,"CREDITS:##Thomas Pedersen#- Programmer#- Audio/Music#- Graphics");

			if (K(1, vk_space))
			{
				global_gamemode_set(gamemode_e::GAME);

			}
		}


		//Draw
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);//black with full alpha.
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer,texture);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		if (global_gamemode_is_menu())
		{
			/*
			SDL_Texture* tex = SDL_CreateTexture(renderer, pixelformatenum, SDL_TEXTUREACCESS_TARGET, gW, gH);
			SDL_SetRenderTarget(renderer, tex);
			SDL_SetRenderTarget(renderer, NULL);
			SDL_RenderCopy(renderer, tex, NULL, NULL);
			SDL_DestroyTexture(tex);
			//SDL_Surface* surf = SDL_CreateRGBSurface(0, gW, gH, 32, 0, 0, 0, 0);
			//SDL_FreeSurface(surf);
			*/

			//Background.
			int frames = menu_gradient.frames;
			int fw = menu_gradient.fw;
			int hh = menu_gradient.h;
			int xx = 0, yy = 0;
			for (int i = 0; i < frames; i++)
			{
				int sel = mux<int>(i % frames, 0, 1, 2);
				int bobbool = i != 0;
				yy = bobbool * 16.0f * sinf(lerp<float>(0.0f,M_PI,(float)timer / 1000.0f));
				menu_gradient.draw_part(xx, yy, i * fw, 0, fw, hh);
			}

			//Selection text.
			//font.draw(64, 128, "Play: <8>#Quit: <9>");


		}
		if (global_gamemode_is_game())
		{
			//Level
			//world_img.draw_part(0, 0, (int)-player_x, (int)-player_y, gW, gH);
			//world_img.draw_part((int)player_x, (int)player_y, view.x, view.y, view.w, view.h);
			world_img.draw_part(0, 0, view.x, view.y, view.w, view.h);

			//Player
			//player_img.draw_ext((int)player_x, (int)player_y, player_angle_view_deg);
			player_img.draw_ext(view.w/2,view.h/2, player_angle_view_deg);

			//HUD.

		}
		if (global_gamemode_is_credits())
		{
			credits_back.draw(0, 0);


		}
		if (global_gamemode_is_tutorial())
		{
			tutorial_back.draw(0, 0);


		}

		SDL_SetRenderTarget(renderer, NULL);
		//SDL_RenderFillRect(renderer, NULL);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/FPS);
	}
	printf("...finished!\n");

goto_free:

	leveldata.free();
	font.free();
	SDL_DestroyTexture(texture);
	Mix_HaltMusic();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

goto_return:
	return 0;
}