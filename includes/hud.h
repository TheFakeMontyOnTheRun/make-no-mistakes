//
// Created by v-lucena on 25/10/26.
//

#ifndef BLANK_HUD_H
#define BLANK_HUD_H
#include <SDL_rect.h>
#include <SDL_render.h>

typedef struct Hud {
    SDL_Rect background;
    SDL_Rect life;
    int max_life_width;
} Hud;

void hud_init(Hud *hud);
void hud_render(SDL_Renderer* renderer, Hud *hud);
void hud_change_hp(Hud *hud, double percentage);

#endif //BLANK_HUD_H
