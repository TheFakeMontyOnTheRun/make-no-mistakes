#include "player.h"

#include <math.h>

#include "constants.h"
#include "util.h"

static int sprite_frame_for_direction(float x, float y) {
    const int direction_count = 8;
    const float half_sector = constants::math::kPi / static_cast<float>(direction_count);
    const float sector_size = (constants::math::kPi * 2.0f) / static_cast<float>(direction_count);
    const float angle = atan2f(y, x);
    const int direction = (int)floorf((angle + half_sector) / sector_size);

    static const int frames[] = { 2, 3, 4, 5, 6, 7, 0, 1 };
    const int normalized_direction = (direction + direction_count) % direction_count;
    return frames[normalized_direction];
}

void player_init(Player *player, float x, float y, float speed, float width, float height) {
    if (!player) {
        return;
    }

    player->x = x;
    player->y = y;
    player->width = width;
    player->height = height;
    player->speed = speed;
    player->velocity_x = 0.0f;
    player->velocity_y = 0.0f;
    player->facing_x = 1.0f;
    player->facing_y = 0.0f;
    player->sprite_frame = sprite_frame_for_direction(player->facing_x, player->facing_y);
    player->animation_time = 0.0f;
    player->moving = false;
}

void player_move(Player *player, float move_x, float move_y, float dt) {
    if (!player) {
        return;
    }

    normalize_vector(&move_x, &move_y);

    player->x += move_x * player->speed * dt;
    player->y += move_y * player->speed * dt;
    player->velocity_x = move_x * player->speed;
    player->velocity_y = move_y * player->speed;
    player->moving = move_x != 0.0f || move_y != 0.0f;
    if (player->moving) {
        player->animation_time += dt;
    } else {
        player->animation_time = 0.0f;
    }

    if (player->moving) {
        player->facing_x = move_x;
        player->facing_y = move_y;
        normalize_vector(&player->facing_x, &player->facing_y);
        player->sprite_frame = sprite_frame_for_direction(player->facing_x, player->facing_y);
    }
}

void player_update(Player *player, const Uint8 *key_state, float dt) {
    if (!player || !key_state) {
        return;
    }

    float move_x = 0.0f;
    float move_y = 0.0f;

    if (key_state[SDL_SCANCODE_W] || key_state[SDL_SCANCODE_UP]) {
        move_y -= 1.0f;
    }
    if (key_state[SDL_SCANCODE_S] || key_state[SDL_SCANCODE_DOWN]) {
        move_y += 1.0f;
    }
    if (key_state[SDL_SCANCODE_A] || key_state[SDL_SCANCODE_LEFT]) {
        move_x -= 1.0f;
    }
    if (key_state[SDL_SCANCODE_D] || key_state[SDL_SCANCODE_RIGHT]) {
        move_x += 1.0f;
    }

    player_move(player, move_x, move_y, dt);
}
