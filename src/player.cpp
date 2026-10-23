#include "player.h"

#include "util.h"

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

    if (move_x != 0.0f || move_y != 0.0f) {
        player->facing_x = move_x;
        player->facing_y = move_y;
        normalize_vector(&player->facing_x, &player->facing_y);
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
