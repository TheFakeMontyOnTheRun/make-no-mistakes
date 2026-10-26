//
// Created by v-lucena on 25/10/26.
//

#include "hud.h"
void hud_init(Hud *hud) {
    hud->background = {
        20,
        40,
        500,
        50
    };
    hud->life = {
        25,
        45,
        490,
        40
    };
    hud->max_life_width = 490;
}

void hud_render(SDL_Renderer* renderer, Hud* hud) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &hud->background);
    SDL_SetRenderDrawColor(renderer, 80, 200, 120, 255);
    SDL_RenderFillRect(renderer, &hud->life);
}

void hud_change_hp(Hud* hud, double percentage) {
    hud->life.w = hud->max_life_width * percentage;
}
