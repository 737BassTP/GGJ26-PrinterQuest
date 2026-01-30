#pragma once

image_t player_img;
float player_angle_move = 0.0f;
float player_angle_view = 0.0f;
float player_x = 128.0f;
float player_y = 128.0f;


void game_init()
{
	player_img.load("Image/s_player.png");

}
void game_free()
{
	player_img.free();

}
void game_input()
{

}
void game_update()
{

}
void game_draw()
{
	player_img.draw_ext(player_x, player_y, player_angle_view);

}



