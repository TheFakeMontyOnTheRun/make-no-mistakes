#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <stdbool.h>

#include "font.h"
#include "player.h"
#include "projectile.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Font font;
    Player player;
    Projectile projectiles[MAX_PROJECTILES];
    int projectile_count;
    int timer;
    Uint64 start_time;
    bool keep_running;
    bool shoot_pressed;
    bool show_initial_screen;
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
