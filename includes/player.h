#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <stdbool.h>

#include "constants.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
    PLAYER_DIRECTION_COUNT = 8
};

extern const char* const PLAYER_DIRECTION_NAMES[PLAYER_DIRECTION_COUNT];

typedef struct Health {
    int current;
    int max;
    float invulnerability_timer;
    bool alive;
} Health;

typedef struct Player {
    float x;
    float y;
    float width;
    float height;
    float speed;
    float velocity_x;
    float velocity_y;
    float facing_x;
    float facing_y;
    int sprite_frame;
    float animation_time;
    bool moving;
    Health health;
} Player;

void player_init(Player *player, float x, float y, float speed, float width, float height);
void player_move(Player *player, float move_x, float move_y, float dt);
void player_update(Player *player, const Uint8 *key_state, float dt);
void player_update_health(Player *player, float dt);
void player_take_damage(Player *player, int amount);
int player_health(const Player *player);
int player_max_health(const Player *player);
bool player_is_alive(const Player *player);
double player_health_percentage(const Player *player);

SDL_Texture* load_player_texture(SDL_Renderer* renderer, const char* filepath);

#ifdef __cplusplus
}
#endif

#endif // PLAYER_H
