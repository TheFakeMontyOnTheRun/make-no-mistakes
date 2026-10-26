#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <stdbool.h>

#include "font.h"
#include "hud.h"
#include "music.h"
#include "player.h"
#include "projectile.h"
#include "timer.h"

#ifdef __cplusplus
extern "C" {
#endif

enum Screen {
    START_SCREEN,
    GAME_LOOP_SCREEN,
    GAME_OVER_SCREEN
};

typedef struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *bg_battle_texture;
    SDL_Texture *bg_puzzle_texture;
    SDL_Rect bg_battle;
    SDL_Rect bg_puzzle;

    SDL_Texture *crate_texture;
    SDL_Rect crate_rect;
    SDL_Texture *player_direction_textures[8];
    Font font;
    Hud hud;
    Player player;
    MusicManager music_manager;
    Projectile projectiles[MAX_PROJECTILES];
    int projectile_count;
    Timer timer;
    Screen screen;
    bool keep_running;
    bool shoot_pressed;
    bool show_initial_screen;
    float projectile_speed;
    float shoot_cooldown;
    float shoot_timer;
    Uint64 previous_ticks;
} Game;

void game_init(Game *game, const char *title, int width, int height);
void game_handle_events(Game *game);
void game_change_screen(Game *game);
void game_update(Game *game, float dt);
void game_render(Game *game);
void game_cleanup(Game *game);

#ifdef __cplusplus
}
#endif

#endif // GAME_H
