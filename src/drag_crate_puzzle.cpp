//
// Created by Dinis on 25/10/2026.
//

#include "drag_crate_puzzle.h"

#include <SDL_image.h>

#include "constants.h"
#include "util.h"


DragCratePuzzle::DragCratePuzzle(SDL_Rect area) {
    time_remaining = 10;
    puzzle_area_limit = area;

    crate_rect = {
        randomNumber(
            puzzle_area_limit.x,
            puzzle_area_limit.x + puzzle_area_limit.w - constants::world::CRATE_SIZE
        ),
        randomNumber(
            puzzle_area_limit.y,
            puzzle_area_limit.y + puzzle_area_limit.h - constants::world::CRATE_SIZE
        ),
        constants::world::CRATE_SIZE,
        constants::world::CRATE_SIZE
    };
    do {
        hole_rect = {
            randomNumber(
                puzzle_area_limit.x,
                puzzle_area_limit.x + puzzle_area_limit.w - constants::world::HOLE_SIZE
            ),
            randomNumber(
                puzzle_area_limit.y,
                puzzle_area_limit.y + puzzle_area_limit.h - constants::world::HOLE_SIZE
            ),
            constants::world::HOLE_SIZE,
            constants::world::HOLE_SIZE
        };
    } while (SDL_HasIntersection(&crate_rect, &hole_rect));
}

DragCratePuzzle::~DragCratePuzzle() {
    if (crate_texture) {
        SDL_DestroyTexture(crate_texture);
    }
}

void DragCratePuzzle::handle_event(SDL_Event &event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            int mouse_x = event.button.x;
            int mouse_y = event.button.y;

            if (mouse_x >= crate_rect.x && mouse_x <= crate_rect.x + crate_rect.w &&
                mouse_y >= crate_rect.y && mouse_y <= crate_rect.y + crate_rect.h)
            {
                dragging = true;
                offset_x = mouse_x - crate_rect.x;
                offset_y = mouse_y - crate_rect.y;
            }
        }
    }

    if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            dragging = false;
        }
    }

    if (event.type == SDL_MOUSEMOTION && dragging) {
        crate_rect.x = event.motion.x - offset_x;
        crate_rect.y = event.motion.y - offset_y;

        keep_inside_area(crate_rect);
    }
}

void DragCratePuzzle::update(float dt) {
    time_remaining -= dt;

    if (SDL_HasIntersection(&crate_rect, &hole_rect)) {
        solved = true;
    }
}

void DragCratePuzzle::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 30, 30, 255);
    SDL_RenderFillRect(renderer, &hole_rect);

    crate_texture = IMG_LoadTexture(
        renderer, "assets/obj/crate.png"
    );
    SDL_RenderCopy(
        renderer,
        crate_texture,
        nullptr,
        &crate_rect
    );
}

bool DragCratePuzzle::is_solved() const {
    return solved;
}
