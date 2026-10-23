#ifndef GAME_H
#define GAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL.h>
#include <stdbool.h>

#include "player.h"
#include "projectile.h"

typedef struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Player player;
    Projectile projectiles[MAX_PROJECTILES];
    int projectile_count;
    bool keep_running;
    bool shoot_pressed;
    float projectile_speed;
    Uint64 previous_ticks;
} Game;

void game_init(Game *game, const char *title, int width, int height);
void game_handle_events(Game *game);
void game_update(Game *game, float dt);
void game_render(Game *game);
void game_cleanup(Game *game);

#ifdef __cplusplus
}
#endif

#endif // GAME_H
