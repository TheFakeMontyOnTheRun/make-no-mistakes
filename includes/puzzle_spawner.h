//
// Created by Dinis on 25/10/2026.
//

#ifndef BLANK_PUZZLESPAWNER_H
#define BLANK_PUZZLESPAWNER_H
#include <memory>
#include <SDL_render.h>

#include "player.h"
#include "puzzle.h"

#ifdef __cplusplus
extern "C" {
#endif

class PuzzleSpawner {
    public:
    PuzzleSpawner(SDL_Rect area, Player player);
    ~PuzzleSpawner();

    void update(float delta_time, const Player& player);
    void render(SDL_Renderer *renderer);
    void spawn_terminal(const Player& player);
    bool player_touches_terminal(const Player& player) const;
    void reset_cooldown();

    SDL_Rect get_terminal_rect() const {
        return terminal_rect;
    }

    private:
    SDL_Rect terminal_rect{};
    SDL_Rect battle_area_limit = {};

    float cooldown = 0;
    bool terminal_visible = false;
};

#ifdef __cplusplus
}
#endif

#endif //BLANK_PUZZLESPAWNER_H
