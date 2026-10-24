#include "game.h"
#include "constants.h"
#include "util.h"

#include <SDL_image.h>

#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>

namespace {

const int kPlayerDirections = 8;

const char* kPlayerDirectionNames[kPlayerDirections] = {
    "north", "northeast", "east", "southeast",
    "south", "southwest", "west", "northwest"
};

bool is_checkerboard_pixel(Uint8 red, Uint8 green, Uint8 blue) {
    const Uint8 brightest = std::max(red, std::max(green, blue));
    const Uint8 darkest = std::min(red, std::min(green, blue));
    return brightest >= constants::visual::kCheckerboardThreshold &&
           brightest - darkest <= constants::visual::kCheckerboardTolerance;
}

void remove_checkerboard_background(SDL_Surface* surface) {
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

    for (size_t current = 0; current < pending.size(); ++current) {
        const int index = pending[current];
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
        fprintf(stderr, "Cannot convert player asset '%s': %s\n",
                loaded_path.c_str(), SDL_GetError());
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
        fprintf(stderr, "Cannot create player texture from '%s': %s\n",
                loaded_path.c_str(), SDL_GetError());
    }
    return texture;
}

} // namespace

bool dragging = false;
int offset_x = 0;
int offset_y = 0;

static void fire_projectile(Game *game) {
    if (!game || game->projectile_count >= MAX_PROJECTILES) {
        return;
    }

    const float spawn_x = game->player.x + (game->player.width * constants::math::kHalf) +
                          (game->player.facing_x * constants::player::kProjectileSpawnOffset);
    const float spawn_y = game->player.y + (game->player.height * constants::math::kHalf) +
                          (game->player.facing_y * constants::player::kProjectileSpawnOffset);

    Projectile *shot = &game->projectiles[game->projectile_count];
    projectile_init(
        shot,
        spawn_x,
        spawn_y,
        game->player.facing_x,
        game->player.facing_y,
        game->projectile_speed
    );

    game->projectile_count++;
}

static void cleanup_inactive_projectiles(Game *game) {
    // Projectiles are confined to the combat area and should not enter puzzle UI space.
    const int world_left = game->bg_battle.x;
    const int world_top = game->bg_battle.y;
    const int world_right = game->bg_battle.x + game->bg_battle.w;
    const int world_bottom = game->bg_battle.y + game->bg_battle.h;

    int active_count = 0;

    for (int i = 0; i < game->projectile_count; ++i) {
        Projectile *projectile = &game->projectiles[i];

        if (projectile->x + projectile->radius < world_left ||
            projectile->x - projectile->radius > world_right ||
            projectile->y + projectile->radius < world_top ||
            projectile->y - projectile->radius > world_bottom) {
            projectile->active = false;
        }

        if (projectile->active) {
            game->projectiles[active_count++] = *projectile;
        }
    }

    game->projectile_count = active_count;
}

void game_init(Game *game, const char *title, int width, int height) {
    if (!game) {
        return;
    }

    game->window = nullptr;
    game->renderer = nullptr;
    for (int i = 0; i < kPlayerDirections; ++i) {
        game->player_direction_textures[i] = nullptr;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Cannot init SDL!\n");
        return;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        fprintf(stderr, "Cannot init SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return;
    }

    game->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    if (!game->window) {
        fprintf(stderr, "Cannot create window!\n");
        IMG_Quit();
        SDL_Quit();
        return;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, 0);
    if (!game->renderer) {
        fprintf(stderr, "Cannot create renderer!\n");
        SDL_DestroyWindow(game->window);
        IMG_Quit();
        SDL_Quit();
        return;
    }

    game->bg_battle_texture = IMG_LoadTexture(
        game->renderer, "assets/bg/battle_area.png"
    );
    game->bg_battle = {
        constants::world::kBattleX,
        constants::world::kBattleY,
        constants::world::kBattleWidth,
        constants::world::kBattleHeight
    };

    game->bg_puzzle_texture = IMG_LoadTexture(
        game->renderer, "assets/bg/puzzle_area.png"
    );
    game->bg_puzzle = {
        constants::world::kPuzzleX,
        constants::world::kPuzzleY,
        constants::world::kPuzzleWidth,
        constants::world::kPuzzleHeight
    };

    game->crate_texture = IMG_LoadTexture(
        game->renderer, "assets/obj/crate.png"
        );

    game->crate_rect = {
        randomNumber(constants::world::kCrateMinX, constants::world::kCrateMaxX),
        randomNumber(constants::world::kCrateMinY, constants::world::kCrateMaxY),
        constants::world::kCrateSize,
        constants::world::kCrateSize
    };

    font_init(&game->font, "assets/fonts/font_spritesheet.png", game->renderer);
    for (int direction = 0; direction < kPlayerDirections; ++direction) {
        char path[128];
        snprintf(path, sizeof(path), "assets/player/directions/%s_0.png",
                 kPlayerDirectionNames[direction]);
        game->player_direction_textures[direction] =
            load_player_texture(game->renderer, path);
    }

    player_init(&game->player,
                constants::player::kStartX,
                constants::player::kStartY,
                constants::player::kSpeed,
                constants::player::kCollisionWidth,
                constants::player::kCollisionHeight);
    game->projectile_count = 0;
    game->timer = constants::hud::kTimerSeconds;
    game->start_time = 0;
    game->keep_running = true;
    game->shoot_pressed = false;
    game->show_initial_screen = true;
    game->projectile_speed = constants::projectile::kSpeed;
    game->previous_ticks = SDL_GetPerformanceCounter();
}

void game_handle_events(Game *game) {
    if (!game) {
        return;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game->keep_running = false;
        }

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                game->keep_running = false;
            }
            if (event.key.keysym.sym == SDLK_SPACE) {
                if (game->show_initial_screen) {
                    game->show_initial_screen = false;
                    SDL_SetWindowMouseRect(game->window, &game->bg_puzzle);
                    game->start_time = SDL_GetTicks64();
                } else {
                    game->shoot_pressed = true;
                }
            }
        }

        if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                game->shoot_pressed = false;
            }
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                int mouse_x = event.button.x;
                int mouse_y = event.button.y;

                if (mouse_x >= game->crate_rect.x && mouse_x <= game->crate_rect.x + game->crate_rect.w &&
                    mouse_y >= game->crate_rect.y && mouse_y <= game->crate_rect.y + game->crate_rect.h)
                {
                    dragging = true;
                    offset_x = mouse_x - game->crate_rect.x;
                    offset_y = mouse_y - game->crate_rect.y;
                }
            }
        }

        if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                dragging = false;
            }
        }

        if (event.type == SDL_MOUSEMOTION && dragging) {
            game->crate_rect.x = event.motion.x - offset_x;
            game->crate_rect.y = event.motion.y - offset_y;
        }
    }
}

void game_update(Game *game, float dt) {
    if (!game) {
        return;
    }

    const Uint8 *keyboard_state = SDL_GetKeyboardState(nullptr);
    player_update(&game->player, keyboard_state, dt);

    if (game->shoot_pressed && (keyboard_state[SDL_SCANCODE_SPACE] || keyboard_state[SDL_SCANCODE_KP_0])) {
        fire_projectile(game);
    }

    for (int i = 0; i < game->projectile_count; ++i) {
        projectile_update(&game->projectiles[i], dt);
    }

    cleanup_inactive_projectiles(game);
}

void game_render(Game *game) {
    if (!game || !game->renderer) {
        return;
    }

    SDL_SetRenderDrawColor(game->renderer,
                           constants::visual::kBackgroundR,
                           constants::visual::kBackgroundG,
                           constants::visual::kBackgroundB,
                           255);
    SDL_RenderClear(game->renderer);

    if (game->show_initial_screen) {
        font_render(&game->font, game->renderer,
                    "Make No Mistakes",
                    constants::hud::kInitialTitleY,
                    constants::hud::kInitialTitleScale);
        font_render(&game->font, game->renderer,
                    "Use WASD to move SPACE to shoot",
                    constants::hud::kInitialInstructionsY,
                    constants::hud::kInitialTextScale);
        font_render(&game->font, game->renderer,
                    "Press SPACE to start",
                    constants::hud::kInitialStartY,
                    constants::hud::kInitialTextScale);
    } else {
        SDL_RenderCopy(game->renderer, game->bg_battle_texture, nullptr, &game->bg_battle);
        SDL_RenderCopy(game->renderer, game->bg_puzzle_texture, nullptr, &game->bg_puzzle);

        SDL_Rect player_rect = {
            (int)game->player.x,
            (int)game->player.y,
            constants::player::kRenderWidth,
            constants::player::kRenderHeight
        };

        SDL_Texture* player_texture =
            game->player_direction_textures[game->player.sprite_frame];
        if (player_texture) {
            SDL_RenderCopy(game->renderer, player_texture, nullptr, &player_rect);
        } else {
            SDL_SetRenderDrawColor(game->renderer,
                                   constants::visual::kFallbackPlayerR,
                                   constants::visual::kFallbackPlayerG,
                                   constants::visual::kFallbackPlayerB,
                                   255);
            SDL_RenderFillRect(game->renderer, &player_rect);
        }

        for (int i = 0; i < game->projectile_count; ++i) {
            SDL_Rect shot = {
                (int) (game->projectiles[i].x - game->projectiles[i].radius),
                (int) (game->projectiles[i].y - game->projectiles[i].radius),
                (int) (game->projectiles[i].radius * 2.0f),
                (int) (game->projectiles[i].radius * 2.0f)
            };

            SDL_SetRenderDrawColor(game->renderer,
                                   constants::visual::kProjectileR,
                                   constants::visual::kProjectileG,
                                   constants::visual::kProjectileB,
                                   255);
            SDL_RenderFillRect(game->renderer, &shot);
        }

        SDL_RenderCopy(
            game->renderer,
            game->crate_texture,
            nullptr,
            &game->crate_rect
        );

        const int remaining_time = game->timer - (SDL_GetTicks64() - game->start_time) / 1000.0f;
        font_render(&game->font,
                    game->renderer,
                    "Time: " + std::to_string(remaining_time),
                    constants::hud::kTimeTextY,
                    constants::hud::kHudScale,
                    START);
        font_render(&game->font,
                    game->renderer,
                    "Enemies: 5",
                    constants::hud::kEnemiesTextY,
                    constants::hud::kHudScale,
                    START);

    }

    SDL_RenderPresent(game->renderer);
}

void game_cleanup(Game *game) {
    if (!game) {
        return;
    }

    for (int direction = 0; direction < kPlayerDirections; ++direction) {
        if (game->player_direction_textures[direction]) {
            SDL_DestroyTexture(game->player_direction_textures[direction]);
            game->player_direction_textures[direction] = nullptr;
        }
    }

    if (game->bg_battle_texture) {
        SDL_DestroyTexture(game->bg_battle_texture);
        game->bg_battle_texture = nullptr;
    }

    if (game->bg_puzzle_texture) {
        SDL_DestroyTexture(game->bg_puzzle_texture);
        game->bg_puzzle_texture = nullptr;
    }

    if (game->crate_texture) {
        SDL_DestroyTexture(game->crate_texture);
        game->crate_texture = nullptr;
    }

    font_cleanup(&game->font);

    if (game->renderer) {
        SDL_DestroyRenderer(game->renderer);
        game->renderer = nullptr;
    }

    if (game->window) {
        SDL_DestroyWindow(game->window);
        game->window = nullptr;
    }

    IMG_Quit();
    SDL_Quit();
}
