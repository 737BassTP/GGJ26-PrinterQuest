#pragma once

extern SDL_Renderer* renderer;

inline SDL_Rect rectangle(int x, int y, int w, int h) { return SDL_Rect(x, y, w, h); }

struct image_t
{
	int w = 0;
	int h = 0;
	SDL_Texture* tex = nullptr;
	void load(const char* fp)
	{
		tex = IMG_LoadTexture(renderer, fp);
		SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	}
	void free()
	{
		SDL_DestroyTexture(tex);
		tex = nullptr;
	}
	void draw(int x, int y)
	{
		SDL_Rect rect = rectangle(x,y,w,h);
		SDL_RenderCopy(renderer, tex, NULL, &rect);
	}
	void draw_ext(int x, int y, float angle)
	{
		SDL_Rect rect = rectangle(x,y,w,h);
		SDL_RenderCopyEx(renderer, tex, NULL, &rect, (float)angle, NULL, SDL_FLIP_NONE);
	}
};


