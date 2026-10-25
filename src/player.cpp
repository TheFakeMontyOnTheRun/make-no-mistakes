#include "player.h"

#include <SDL_image.h>

#include <cmath>
#include <string>
#include <vector>

#include "constants.h"
#include "util.h"

namespace {

bool is_checkerboard_pixel(Uint8 red, Uint8 green, Uint8 blue) {
    const Uint8 brightest = std::max(red, std::max(green, blue));
    const Uint8 darkest = std::min(red, std::min(green, blue));
    return brightest >= constants::visual::CHECKERBOARD_THRESHOLD &&
           brightest - darkest <= constants::visual::CHECKERBOARD_TOLERANCE;
}

void remove_checkerboard_background(const SDL_Surface* surface) {
    const int pixel_count = surface->w * surface->h;
    auto* pixels = static_cast<Uint32*>(surface->pixels);
    std::vector<bool> transparent(pixel_count, false);
    std::vector<int> pending;

    const auto try_add = [&](int x, int y) {
        const int index = y * surface->w + x;
        if (transparent[index]) {
            return;
        }

        Uint8 red;
        Uint8 green;
        Uint8 blue;
        Uint8 alpha;
        const int pixel_index = y * surface->pitch / sizeof(Uint32) + x;
        SDL_GetRGBA(pixels[pixel_index], surface->format, &red, &green, &blue, &alpha);
        if (is_checkerboard_pixel(red, green, blue)) {
            transparent[index] = true;
            pending.push_back(index);
        }
    };

    for (int x = 0; x < surface->w; ++x) {
        try_add(x, 0);
        try_add(x, surface->h - 1);
    }
    for (int y = 1; y < surface->h - 1; ++y) {
        try_add(0, y);
        try_add(surface->w - 1, y);
    }

    for (const int index : pending) {
        const int x = index % surface->w;
        const int y = index / surface->w;
        const int neighbors[][2] = {{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};

        for (const auto& neighbor : neighbors) {
            if (neighbor[0] >= 0 && neighbor[0] < surface->w &&
                neighbor[1] >= 0 && neighbor[1] < surface->h) {
                try_add(neighbor[0], neighbor[1]);
            }
        }
    }

    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            const int index = y * surface->w + x;
            if (transparent[index]) {
                Uint8 red;
                Uint8 green;
                Uint8 blue;
                Uint8 alpha;
                const int pixel_index = y * surface->pitch / sizeof(Uint32) + x;
                SDL_GetRGBA(pixels[pixel_index], surface->format, &red, &green, &blue, &alpha);
                pixels[pixel_index] = SDL_MapRGBA(surface->format, red, green, blue, 0);
            }
        }
    }
}

SDL_Surface* load_asset(const char* relative_path, std::string* loaded_path) {
    char* base_path = SDL_GetBasePath();
    if (base_path) {
        const std::string executable_asset_path = std::string(base_path) + relative_path;
        SDL_free(base_path);

        SDL_Surface* surface = IMG_Load(executable_asset_path.c_str());
        if (surface) {
            if (loaded_path) {
                *loaded_path = executable_asset_path;
            }
            return surface;
        }
    }

    SDL_Surface* surface = IMG_Load(relative_path);
    if (surface) {
        if (loaded_path) {
            *loaded_path = relative_path;
        }
        return surface;
    }

    return nullptr;
}

} // namespace

const char* const PLAYER_DIRECTION_NAMES[PLAYER_DIRECTION_COUNT] = {
    "north", "northeast", "east", "southeast",
    "south", "southwest", "west", "northwest"
};

static int sprite_frame_for_direction(float x, float y) {
    constexpr int direction_count = 8;
    constexpr float half_sector = constants::math::PI / static_cast<float>(direction_count);
    constexpr float sector_size = (constants::math::PI * 2.0f) / static_cast<float>(direction_count);
    const float angle = atan2f(y, x);
    const int direction = static_cast<int>(floorf((angle + half_sector) / sector_size));

    static const int frames[] = { 2, 3, 4, 5, 6, 7, 0, 1 };
    const int normalized_direction = (direction + direction_count) % direction_count;
    return frames[normalized_direction];
}

SDL_Texture* load_player_texture(SDL_Renderer* renderer, const char* filepath) {
    std::string loaded_path;
    SDL_Surface* loaded_surface = load_asset(filepath, &loaded_path);
    if (!loaded_surface) {
        fprintf(stderr, "Cannot read player asset '%s': %s\n", filepath, IMG_GetError());
        return nullptr;
    }

    SDL_Surface* surface = SDL_ConvertSurfaceFormat(loaded_surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(loaded_surface);
    if (!surface) {
        fprintf(stderr, "Cannot convert player asset '%s': %s\n", loaded_path.c_str(), SDL_GetError());
        return nullptr;
    }

    if (surface->w <= 0 || surface->h <= 0) {
        fprintf(stderr, "Player image is empty in '%s'\n", loaded_path.c_str());
        SDL_FreeSurface(surface);
        return nullptr;
    }

    if (SDL_MUSTLOCK(surface)) {
        SDL_LockSurface(surface);
    }

    remove_checkerboard_background(surface);

    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    } else {
        fprintf(stderr, "Cannot create player texture from '%s': %s\n", loaded_path.c_str(), SDL_GetError());
    }
    return texture;
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
