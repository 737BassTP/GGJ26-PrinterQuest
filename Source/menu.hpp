#pragma once

/*

image_t menu_gradient_img;

void menu_init()
{
	menu_gradient_img.load("assets/s_menu_logo_strip3.png");

}
void menu_free()
{
	menu_gradient_img.free();


}
void menu_input()
{


}
void menu_update()
{


}

void menu_draw()
{
	menu_gradient_img.draw(0,0);

	SDL_Texture* tex = SDL_CreateTexture(renderer, pixelformatenum, SDL_TEXTUREACCESS_TARGET, gW, gH);

	SDL_SetRenderTarget(renderer, tex);

	//

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, tex, NULL, NULL);
	SDL_DestroyTexture(tex); 
	
	//SDL_Surface* surf = SDL_CreateRGBSurface(0, gW, gH, 32, 0, 0, 0, 0);
	//SDL_FreeSurface(surf);
}

/**/