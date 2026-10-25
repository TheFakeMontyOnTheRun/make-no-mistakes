#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    PLAYER_DIRECTION_COUNT = 8
};

extern const char* const PLAYER_DIRECTION_NAMES[PLAYER_DIRECTION_COUNT];

SDL_Texture* load_player_texture(SDL_Renderer* renderer, const char* filepath);

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
} Player;

void player_init(Player *player, float x, float y, float speed, float width, float height);
void player_move(Player *player, float move_x, float move_y, float dt);
void player_update(Player *player, const Uint8 *key_state, float dt);

#ifdef __cplusplus
}
#endif

#endif // PLAYER_H
