#pragma once

/*

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
//Check for key modifiers; they can be possibly OR'ed together; vk_shift | vk_ctrl.
bool KMOD(SDL_Keymod key) { return (SDL_GetModState() & key) != 0; }

/**/