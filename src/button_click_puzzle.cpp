//
// Created by Dinis on 25/10/2026.
//

#include "button_click_puzzle.h"

ButtonClickPuzzle::ButtonClickPuzzle(SDL_Rect area) {
    time_remaining = 5;
    puzzle_area_limit = area;

    button_rect = {550, 100, 64, 64};
}

void ButtonClickPuzzle::handle_event(SDL_Event &event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            const SDL_Point point {
                .x = event.button.x,
                .y = event.button.y
            };

            if (SDL_PointInRect(&point, &button_rect)) {
                solved = true;
            }
        }
    }
}

void ButtonClickPuzzle::update(float dt) {
    time_remaining -= dt;
}

void ButtonClickPuzzle::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 130, 5, 20, 255);
    SDL_RenderFillRect(renderer, &button_rect);
}

bool ButtonClickPuzzle::is_solved() const {
    return solved;
}

