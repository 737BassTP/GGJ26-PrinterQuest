#pragma once

SDL_Window* window;
SDL_Renderer* renderer;
bool running = 1;
SDL_PixelFormatEnum pixelformatenum = SDL_PIXELFORMAT_RGBA32;

//Forward declarations.
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
}
void sdl_free()
{
	Mix_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void sdl_loop()
{
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
		if (global_gamemode_is_menu()) { menu_input(); menu_update(); }
		if (global_gamemode_is_game()) { game_input(); game_update(); }
		if (global_gamemode_is_credits()) { credits_input(); credits_update(); }


		SDL_RenderClear(renderer);
		
		SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
		SDL_Rect rect = rectangle(32,32,64,64);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		
		if (global_gamemode_is_menu()) { menu_draw(); }
		if (global_gamemode_is_game()) { game_draw(); }
		if (global_gamemode_is_credits()) { credits_draw(); }

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}
	printf("...finished!\n");
}
void sdl_draw()
{

}