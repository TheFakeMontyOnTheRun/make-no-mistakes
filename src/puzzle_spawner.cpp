//
// Created by Dinis on 25/10/2026.
//

#include "puzzle_spawner.h"

#include "constants.h"
#include "util.h"


PuzzleSpawner::~PuzzleSpawner() = default;

PuzzleSpawner::PuzzleSpawner(SDL_Rect area, Player player) {
    cooldown = constants::world::PUZZLE_COOLDOWN;
    battle_area_limit = area;

    spawn_terminal(player);
    terminal_visible = false;
}

void PuzzleSpawner::update(float delta_time, const Player& player) {
    if (!terminal_visible) {
        cooldown -= delta_time;

        if (cooldown <= 0.0f) {
            spawn_terminal(player);
            terminal_visible = true;
        }
    }
}

void PuzzleSpawner::render(SDL_Renderer *renderer) {
    if (!terminal_visible) {
        return;
    }
    SDL_SetRenderDrawColor(renderer, 105, 120, 135, 255);
    SDL_RenderFillRect(renderer, &terminal_rect);
}

void PuzzleSpawner::spawn_terminal(const Player &player) {
    SDL_Rect player_rect = {
        static_cast<int>(round(player.x)),
        static_cast<int>(round(player.y)),
        constants::player::RENDER_WIDTH * 2,
        constants::player::RENDER_HEIGHT * 2,
    };
    do {
        terminal_rect = {
            randomNumber(
                battle_area_limit.x,
                battle_area_limit.x + battle_area_limit.w - constants::world::TERMINAL_SIZE_W
            ),
            randomNumber(
                battle_area_limit.y,
                battle_area_limit.y + battle_area_limit.h - constants::world::TERMINAL_SIZE_H
            ),
            constants::world::TERMINAL_SIZE_W,
            constants::world::TERMINAL_SIZE_H
        };
    } while (SDL_HasIntersection(&terminal_rect, &player_rect));
}

bool PuzzleSpawner::player_touches_terminal(const Player &player) const {
    if (!terminal_visible) {
        return false;
    }

    SDL_Rect player_rect = {
        static_cast<int>(std::round(player.x)),
        static_cast<int>(std::round(player.y)),
        constants::player::RENDER_WIDTH * 2,
        constants::player::RENDER_HEIGHT * 2
    };

    return SDL_HasIntersection(&player_rect, &terminal_rect);
}

void PuzzleSpawner::reset_cooldown() {
    terminal_visible = false;
    cooldown = constants::world::PUZZLE_COOLDOWN;
}