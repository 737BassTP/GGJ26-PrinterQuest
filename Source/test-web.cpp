#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "functions.hpp"


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool running = true;
image_t img;
const char *title_window = "GGJ26";
Mix_Chunk *sfx_bounce;

int vW = 1366;
int vH = 768;

void loop()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
	{
        if (e.type == SDL_QUIT)
		{
            running = false;
			#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
			#endif
        }
    }

    // Clear screen (black)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw red rectangle
    SDL_Rect rect = { 100, 100, 200, 150 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
	img.draw(32,32);
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, vW, vH, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	SDL_AudioSpec desired;
	desired.freq = 48000;
	desired.format = SDL_AUDIO_U8;
	desired.channels = 2;
	desired.callback = NULL;
	
	SDL_AudioSpec obtained;
	SDL_OpenAudio(desired,obtained);
	
	Mix_Init(MIX_INIT_MP3);
	sfx_bounce = Mix_LoadWAV("assets/a_bounce.wav");
	Mix_PlayChannel(0,sfx_bounce,3);
	
	img.load("assets/s_player.png");

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
#else
    while (running)
	{
        loop();
        SDL_Delay(16); // ~60 FPS
    }
#endif

	img.free();
	Mix_FreeChunk(sfx_bounce);
	Mix_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}