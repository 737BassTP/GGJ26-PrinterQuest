#pragma once

/*

bool running = 1;
SDL_PixelFormatEnum pixelformatenum = SDL_PIXELFORMAT_RGBA32;

Mix_Music* music = nullptr;

//Forward declarations.
extern void keyboard_update();

extern void game_draw();
extern void game_input();
extern void game_update();
extern void menu_draw();
extern void menu_input();
extern void menu_update();
extern void credits_draw();
extern void credits_input();
extern void credits_update();
//extern void
//extern void

//
void sdl_init()
{
	//SDL
	Uint32 flags_sdl = SDL_INIT_EVERYTHING;
	SDL_Init(flags_sdl);

	//Window
	Uint32 flags_window = 0;
	window = SDL_CreateWindow("GGJ26", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, vW, vH, flags_window);
	if (window == NULL)
	{
		PrintError("SDL_CreateWindow()", SDL_GetError());
		return;
	}

	//Renderer
	Uint32 flags_renderer = 0;
	renderer = SDL_CreateRenderer(window, -1, flags_renderer);
	if (renderer == NULL)
	{
		PrintError("SDL_CreateRenderer()", SDL_GetError());
		return;
	}
	
	//Mixer.
	int flags_mixer = 0;
	flags_mixer |= MIX_INIT_MP3;
	if (Mix_Init(flags_mixer) == 0)
	{
		PrintError("Mix_Init()", SDL_GetError());
		return;
	}
	int chunksize = 1<11;//pow(2,11) = 2048
	Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, chunksize);
	int flags_allow = SDL_AUDIO_ALLOW_ANY_CHANGE;
	int moad = Mix_OpenAudioDevice(48000, AUDIO_F32SYS, 2, chunksize, NULL, flags_allow);
	if (moad == -1)
	{
		PrintError("Mix_OpenAudioDevice()", SDL_GetError());
		return;
	}
}
void sdl_free()
{
	Mix_HaltMusic();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void sdl_loop()
{
	image_t test;
	test.load("assets/s_tileset.png");

	printf("Starting...\n");
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

		//Update.
		if (global_gamemode_is_menu()) { menu_input(); menu_update(); }
		if (global_gamemode_is_game()) { game_input(); game_update(); }
		if (global_gamemode_is_credits()) { credits_input(); credits_update(); }

		//Draw
		SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
		SDL_RenderClear(renderer);
		
		SDL_Rect rect = rectangle(32,32,64,64);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		
		if (global_gamemode_is_menu()) { menu_draw(); }
		if (global_gamemode_is_game()) { game_draw(); }
		if (global_gamemode_is_credits()) { credits_draw(); }

		test.draw(64, 64);

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}
	printf("...finished!\n");

	test.free();
}
void sdl_draw()
{

}

/**/