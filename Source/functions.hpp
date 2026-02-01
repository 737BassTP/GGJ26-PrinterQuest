#pragma once

extern SDL_Renderer* renderer;
inline Uint32 get_timer() { return SDL_GetTicks(); }

void PrintError(const char* msg1, const char* msg2) { printf("%s: %s\n", msg1, msg2); }
inline SDL_Rect rectangle(int x, int y, int w, int h) { SDL_Rect r; r.x=x; r.y=y; r.w=w; r.h=h; return r; }
template <typename T> T lerp(T x1, T x2, float vx) { return (T)(x1 + (x2 - x1) * vx); }
template <typename T> T max2(T a, T b) { return a > b ? a : b; }
template <typename T> T min2(T a, T b) { return a > b ? b : a; }
template <typename T> T mux(int nth, ...)
{
	T ret = (T)0;
	va_list args;
	va_start(args, nth);
	while (nth >= 0)
	{
		ret = va_arg(args, T);
		nth--;
	}
	va_end(args);
	return ret;
}
template <typename T> T degtorad(T d) { return d / (T)180 * (T)M_PI; }
template <typename T> T radtodeg(T r) { return r / (T)M_PI * (T)180; }
template <typename T> bool point_in_rectangle(T x, T y, T rx1, T ry1, T rx2, T ry2) { return (x >= rx1) and (x < rx2) and (y >= ry1) and (y < ry2); }
template <typename T> int rectangle_in_rectangle(T ax1, T ay1, T ax2, T ay2, T bx1, T by1, T bx2, T by2)
{
	int ret = 0;
	ret += point_in_rectangle(ax1, ay1, bx1, by1, bx2, by2);
	ret += point_in_rectangle(ax2, ay1, bx1, by1, bx2, by2);
	ret += point_in_rectangle(ax1, ay2, bx1, by1, bx2, by2);
	ret += point_in_rectangle(ax2, ay2, bx1, by1, bx2, by2);
	return (ret==4) ? 1 : (ret==0) ? 0 : 2;
}
struct image_t
{
	image_t() = default;
	~image_t() { free(); };
	int w = 0;
	int h = 0;
	int fw = 0;
	int frames = 1;
	SDL_Surface* surf = nullptr;
	SDL_Texture* tex = nullptr;
	void load(const char* fp)
	{
		//tex = IMG_LoadTexture(renderer, fp);
		surf = IMG_Load(fp);
		tex = SDL_CreateTextureFromSurface(renderer,surf);
		SDL_QueryTexture(tex, NULL, NULL, &w, &h);

		frames = count_frames(fp);
		fw = surf->w / frames;
	}
	int count_frames(const char* fp)
	{
		int def = 1;
		std::string f = fp;
		//printf("%s\n",fp);
		std::string substr = "_strip";
		int spos = f.find(substr);
		if (spos == -1) { return def; }
		int slen = substr.size();
		spos += slen;
		std::string ext = ".png";
		std::string num = f.substr(spos, f.find(ext));
		//frames is both returned and set as this->frames;
		frames = std::stoi(num);
		//printf("image <%s> has <%i> frames\n", fp, frames);
		return frames;
	}
	void free()
	{
		SDL_FreeSurface(surf);
		SDL_DestroyTexture(tex);
		surf = nullptr;
		tex = nullptr;
	}
	void draw(int x, int y)
	{
		SDL_Rect rect = rectangle(x,y,w,h);
		SDL_RenderCopy(SDL_GetRenderer(window), tex, NULL, &rect);
	}
	void draw_ext(int x, int y, float angle)
	{
		SDL_Rect rect = rectangle(x,y,w,h);
		SDL_RenderCopyEx(SDL_GetRenderer(window), tex, NULL, &rect, (float)angle, NULL, SDL_FLIP_NONE);
	}
	void draw_part(int x,int y,int px,int py,int pw,int ph)
	{
		SDL_Rect srcrect = rectangle(px, py, pw, ph);
		SDL_Rect dstrect = rectangle(x, y, pw, ph);
		SDL_RenderCopy(renderer, tex, &srcrect, &dstrect);
	}
	void draw_scaled(int x, int y, float xs, float ys)
	{
		SDL_Renderer* r = SDL_GetRenderer(window);
		float oxs, oys;
		SDL_RenderGetScale(r, &oxs, &oys);
		SDL_RenderSetScale(r, xs, ys);
		draw(x, y);
		SDL_RenderSetScale(r, oxs, oys);
	}
	void draw_anim(int x, int y, int frame)
	{
		int ww, hh;
		ww = fw;
		hh = surf->h;
		SDL_Rect srcrect = rectangle(frame * fw, 0, ww, hh);
		SDL_Rect dstrect = rectangle(x, y, ww, hh);
		SDL_RenderCopy(SDL_GetRenderer(window), tex, &srcrect, &dstrect);
	}
	void draw_anim_part(int x, int y, int frame, int left, int top, int width, int height)
	{
		int ww, hh;
		ww = min2<int>(fw, width);
		hh = min2<int>(surf->h, height);
		SDL_Rect srcrect = rectangle(frame * fw + left, top, ww, hh);
		SDL_Rect dstrect = rectangle(x, y, ww, hh);
		SDL_RenderCopy(SDL_GetRenderer(window), tex, &srcrect, &dstrect);
	}
};

void surface_swap_red_blue(SDL_Surface* surf)
{
	Uint32* pixels = (Uint32*)surf->pixels;
	int s = surf->w * surf->h;
	for (int i = 0; i < s; i++)
	{
		Uint32 old = pixels[i];
		Uint8 r = (old >> 0) & 0xFF;
		Uint8 g = (old >> 8) & 0xFF;
		Uint8 b = (old >> 16) & 0xFF;
		Uint8 a = (old >> 24) & 0xFF;
		Uint32 pnew = (r << 16) | (g << 8) | (b << 0) | (a << 24);
		pixels[i] = pnew;
	}
}
void surface_flip_v(SDL_Surface* surf)
{
	Uint32* pixels = (Uint32*)surf->pixels;
	std::vector<Uint32> newpixels = {};
	int s = surf->w * surf->h;
	for (int i = 0; i < s; i++)
	{
		Uint32 old = pixels[i];
		newpixels.push_back(old);
	}
	for (int i = 0; i < s; i++)
	{
		Uint32 old = newpixels[s-1-i];
		pixels[i] = old;
	}
	newpixels.clear();
}
void surface_pixel_replace(SDL_Surface* surf, Uint32 src, Uint32 dst)
{
	Uint32* pixels = (Uint32*)surf->pixels;
	int s = surf->w * surf->h;
	for (int i = 0; i < s; i++)
	{
		Uint32 old = pixels[i];
		Uint32 pnew = (old == src) ? dst : old;
		pixels[i] = pnew;
	}
}
enum objname_e
{
	objname_mask,
	objname_bullet
};
struct object_t
{
	image_t img;
	int x,y;
	object_t() = default;
	~object_t()
	{
		free();
	}
	void load(const char* fp)
	{
		img.load(fp);
	}
	void free() { img.free(); }
	void set(int xx, int yy) { x = xx; y = yy; }

};



struct music_t
{
	music_t() = default;
	~music_t() { free(); }
	Mix_Music* music = nullptr;
	void load(const char* fp)
	{
		stop();
		music = Mix_LoadMUS(fp);
	}
	void free()
	{
		stop();
		Mix_FreeMusic(music);
		music = nullptr;
	}
	void play()
	{
		Mix_PlayMusic(music, -1);
	}
	void stop()
	{
		Mix_HaltMusic();
	}
	void volume(float vol_nor)
	{
		Mix_VolumeMusic(lerp<int>(0, MIX_MAX_VOLUME, vol_nor / 1.0f));
	}
};

struct font_t
{
	font_t() = default;
	~font_t() { free(); }
	image_t img;
	void load(const char* fp) { img.load(fp); }
	void free() { img.free(); }
	void draw(int x, int y, const char* msg)
	{
		int nlc = '#';
		int xs = x;
		int len = strlen(msg);
		int fw = img.fw;
		int hh = img.h;
		for (int i = 0; i < len; i++)
		{
			int c = msg[i];
			if (c == nlc)
			{
				x = xs;
				y += hh;
				continue;
			}
			c -= 32;
			img.draw_part(x + i * fw, y, c * fw, 0, fw, hh);
		}
	}

};

struct view_t
{
	int x = 0;
	int y = 0;
	int w = gW;
	int h = gH;
};

struct leveldata_t
{
#define levsiz 0x10000
	leveldata_t() = default;
	~leveldata_t() { free(); }
	std::array<Uint8, levsiz> arr = {};
	void load(const char* fp)
	{
		SDL_RWops* f = SDL_RWFromFile(fp, "rb");
		for (int i = 0; i < levsiz; i++)
		{
			arr[i] = SDL_ReadU8(f);
		}
		SDL_RWclose(f);
	}
	void free()
	{
		//arr.clear();
	}
	SDL_Texture* draw(int x, int y)
	{

	}

#undef levsiz
};



const int kbarr_num = SDL_NUM_SCANCODES;
bool kbarr_cur[kbarr_num] = { 0 };
bool kbarr_prev[kbarr_num] = { 0 };
void keyboard_update()
{
	memcpy(kbarr_prev, kbarr_cur, kbarr_num);
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < kbarr_num; i++)
	{
		kbarr_cur[i] = keys[i];
	}
}
bool keyboard_check(SDL_Scancode key) { return kbarr_cur[key] && kbarr_prev[key]; }
bool keyboard_check_pressed(SDL_Scancode key) { return kbarr_cur[key] && !kbarr_prev[key]; }
bool keyboard_check_released(SDL_Scancode key) { return !kbarr_cur[key] && kbarr_prev[key]; }
bool K(int type, SDL_Scancode key)
{
	switch (type)
	{
	case 0: { return keyboard_check(key); } break;
	case 1: { return keyboard_check_pressed(key); } break;
	case 2: { return keyboard_check_released(key); } break;
	default: { printf("<K> default: %i\n", type); } break;
	}
	return 0;
}
bool KMOD(SDL_Keymod key) { return (SDL_GetModState() & key) != 0; }

void screenshot_save(const char* fil)
{
	SDL_Texture* target = SDL_GetRenderTarget(renderer);
	int width, height;
	SDL_QueryTexture(target, NULL, NULL, &width, &height);

	SDL_Texture* texture = SDL_CreateTexture(renderer, pixelformatenum, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_SetRenderTarget(renderer, texture);

	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
	IMG_SavePNG(surface, fil);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	SDL_SetRenderTarget(renderer, target);

	printf("Screenshot saved!\n");
}
int random(int v) { return rand() % v; }

