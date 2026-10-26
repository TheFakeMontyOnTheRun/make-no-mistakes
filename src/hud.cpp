//
// Created by v-lucena on 25/10/26.
//

#include "hud.h"

#include <algorithm>

void hud_init(Hud *hud) {
    if (!hud) {
        return;
    }

    hud->background = {
        .x = 100,
        .y = 50,
        .w = 260,
        .h = 25
    };
    hud->life = {
        .x = 105,
        .y = 52,
        .w = 250,
        .h = 20
    };
    hud->max_life_width = 250;
}

void hud_render(SDL_Renderer* renderer, Hud* hud) {
    if (!renderer || !hud) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &hud->background);
    SDL_SetRenderDrawColor(renderer, 80, 200, 120, 255);
    SDL_RenderFillRect(renderer, &hud->life);
}

void hud_change_hp(Hud* hud, double percentage) {
    if (!hud) {
        return;
    }

    const double bounded_percentage = std::max(0.0, std::min(1.0, percentage));
    hud->life.w = static_cast<int>(hud->max_life_width * bounded_percentage);
}
