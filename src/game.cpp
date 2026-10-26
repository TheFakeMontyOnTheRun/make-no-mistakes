#include "game.h"
#include "constants.h"
#include "util.h"
#include "EnemySpawner.hpp"

#include <SDL_image.h>

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

EnemySpawner *enemySpawner;
std::vector<Enemy*> enemies;

static bool dragging = false;
static int offset_x = 0;
static int offset_y = 0;

void game_change_screen(Game *game) {
    if (!game) {
        return;
    }

    if (game->screen == START_SCREEN) {
		game->screen = GAME_LOOP_SCREEN;

        enemySpawner = new EnemySpawner(game->renderer, 2, [](Enemy *foe) {
			enemies.emplace_back(foe);
		});

    } else if (game->screen == GAME_LOOP_SCREEN) {
        game->screen = GAME_OVER_SCREEN;
    }
}

void game_init(Game *game, const char *title, const int width, const int height) {
    if (!game) {
        return;
    }

    game->window = nullptr;
    game->renderer = nullptr;
    for (auto & player_direction_texture : game->player_direction_textures) {
        player_direction_texture = nullptr;
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
        constants::world::BATTLE_X,
        constants::world::BATTLE_Y,
        constants::world::BATTLE_WIDTH,
        constants::world::BATTLE_HEIGHT
    };

    game->bg_puzzle_texture = IMG_LoadTexture(
        game->renderer, "assets/bg/puzzle_area.png"
    );
    game->bg_puzzle = {
        constants::world::PUZZLE_X,
        constants::world::PUZZLE_Y,
        constants::world::PUZZLE_WIDTH,
        constants::world::PUZZLE_HEIGHT
    };

    game->crate_texture = IMG_LoadTexture(
        game->renderer, "assets/obj/crate.png"
    );

    game->crate_rect = {
        randomNumber(constants::world::CRATE_MIN_X, constants::world::CRATE_MAX_X),
        randomNumber(constants::world::CRATE_MIN_Y, constants::world::CRATE_MAX_Y),
        constants::world::CRATE_SIZE,
        constants::world::CRATE_SIZE
    };

    font_init(&game->font, "assets/fonts/font_spritesheet.png", game->renderer);
    for (int direction = 0; direction < PLAYER_DIRECTION_COUNT; ++direction) {
        char path[128];
        snprintf(path, sizeof(path), "assets/player/directions/%s_0.png",
                 PLAYER_DIRECTION_NAMES[direction]);
        game->player_direction_textures[direction] =
            load_player_texture(game->renderer, path);
    }

    player_init(&game->player,
                constants::player::START_X,
                constants::player::START_Y,
                constants::player::SPEED,
                constants::player::COLLISION_WIDTH,
                constants::player::COLLISION_HEIGHT);
    game->projectile_count = 0;
    game->keep_running = true;
    game->shoot_pressed = false;
    game->show_initial_screen = true;
    game->projectile_speed = constants::projectile::SPEED;
    game->shoot_cooldown = 0.18f;
    game->shoot_timer = 0.0f;
    timer_init(&game->timer, SDL_GetTicks64(), constants::hud::TIMER_SECONDS);
    game->screen = START_SCREEN;
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
                if (game->screen == START_SCREEN || game->screen == GAME_OVER_SCREEN) {
                    game_change_screen(game);
                    SDL_SetWindowMouseRect(game->window, &game->bg_puzzle);
                    timer_init(&game->timer, SDL_GetTicks64(), constants::hud::TIMER_SECONDS);
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

    if (!enemySpawner) {
		return;
    }

    enemySpawner->update(dt);

    const Uint8 *keyboard_state = SDL_GetKeyboardState(nullptr);
    player_update(&game->player, keyboard_state, dt);

    if (game->screen == GAME_LOOP_SCREEN && game->shoot_pressed &&
        (keyboard_state[SDL_SCANCODE_SPACE] || keyboard_state[SDL_SCANCODE_KP_0])) {
        game->shoot_timer -= dt;
        if (game->shoot_timer <= 0.0f) {
            game->shoot_timer = game->shoot_cooldown;
            fire_projectile(game);
        }
    } else {
        game->shoot_timer = 0.0f;
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
                           constants::visual::BACKGROUND_R,
                           constants::visual::BACKGROUND_G,
                           constants::visual::BACKGROUND_B,
                           255);
    SDL_RenderClear(game->renderer);

    if (game->screen == START_SCREEN) {
        font_render(&game->font, game->renderer,
                    "Make No Mistakes",
                    0,
                    constants::hud::INITIAL_TITLE_Y,
                    constants::hud::INITIAL_TITLE_SCALE,
                    true,
                    false);
        font_render(&game->font, game->renderer,
                    "Use WASD to move SPACE to shoot",
                    0,
                    constants::hud::INITIAL_INSTRUCTIONS_Y,
                    constants::hud::INITIAL_TEXT_SCALE,
                    true,
                    false);
        font_render(&game->font, game->renderer,
                    "Press SPACE to start",
                    0,
                    constants::hud::INITIAL_START_Y,
                    constants::hud::INITIAL_TEXT_SCALE,
                    true,
                    false);
    }

    if (game->screen == GAME_LOOP_SCREEN) {
        SDL_RenderCopy(game->renderer, game->bg_battle_texture, nullptr, &game->bg_battle);
        SDL_RenderCopy(game->renderer, game->bg_puzzle_texture, nullptr, &game->bg_puzzle);

        const SDL_Rect player_rect = {
            .x = static_cast<int>(game->player.x),
            .y = static_cast<int>(game->player.y),
            .w = constants::player::RENDER_WIDTH,
            .h = constants::player::RENDER_HEIGHT
        };

        SDL_Texture* player_texture =
            game->player_direction_textures[game->player.sprite_frame];
        if (player_texture) {
            SDL_RenderCopy(game->renderer, player_texture, nullptr, &player_rect);
        } else {
            SDL_SetRenderDrawColor(game->renderer,
                                   constants::visual::FALLBACK_PLAYER_R,
                                   constants::visual::FALLBACK_PLAYER_G,
                                   constants::visual::FALLBACK_PLAYER_B,
                                   255);
            SDL_RenderFillRect(game->renderer, &player_rect);
        }

        for (int i = 0; i < game->projectile_count; ++i) {
            SDL_Rect shot = {
                .x = static_cast<int>(game->projectiles[i].x - game->projectiles[i].radius),
                .y = static_cast<int>(game->projectiles[i].y - game->projectiles[i].radius),
                .w = static_cast<int>(game->projectiles[i].radius * 2.0f),
                .h = static_cast<int>(game->projectiles[i].radius * 2.0f)
            };

            SDL_SetRenderDrawColor(game->renderer,
                                   constants::visual::PROJECTILE_R,
                                   constants::visual::PROJECTILE_G,
                                   constants::visual::PROJECTILE_B,
                                   255);
            SDL_RenderFillRect(game->renderer, &shot);
        }

        SDL_RenderCopy(
            game->renderer,
            game->crate_texture,
            nullptr,
            &game->crate_rect
        );

        for (const auto &foe : enemies) {

			SDL_Rect foe_head  = {(int)foe->m_x, (int)foe->m_y +  0, (int)32, (int)32};
			SDL_Rect foe_neck  = {(int)foe->m_x, (int)foe->m_y + 10, (int)32, (int)32};
			SDL_Rect foe_belly = {(int)foe->m_x, (int)foe->m_y + 32, (int)32, (int)32};
			SDL_Rect foe_legs  = {(int)foe->m_x, (int)foe->m_y + 64, (int)32, (int)32};

			SDL_RenderCopy(game->renderer, foe->m_neck, nullptr, &foe_neck);
			SDL_RenderCopy(game->renderer, foe->m_belly, nullptr, &foe_belly);
			SDL_RenderCopy(game->renderer, foe->m_legs, nullptr, &foe_legs);

			SDL_RenderCopy(game->renderer, foe->m_head, nullptr, &foe_head);
		}

        const int remaining_time = static_cast<int>(timer_get_time_in_seconds(&game->timer));
        font_render(&game->font,
                    game->renderer,
                    "Time: " + std::to_string(remaining_time),
                    10,
                    constants::hud::TIME_TEXT_Y,
                    constants::hud::HUD_SCALE,
                    false,
                    false);
        font_render(&game->font,
                    game->renderer,
                    "Enemies: 5",
                    10,
                    constants::hud::ENEMIES_TEXT_Y,
                    constants::hud::HUD_SCALE,
                    false,
                    false);

        if (remaining_time <= 0) {
            game_change_screen(game);
        }
    }

    if (game->screen == GAME_OVER_SCREEN) {
        font_render(&game->font, game->renderer, "Game Over", 0, 100, 3.0f, true, false);
        font_render(&game->font, game->renderer, "Press SPACE to restart", 0, 200, 2.0f, true, false);
    }

    SDL_RenderPresent(game->renderer);
}

void game_cleanup(Game *game) {
    if (!game) {
        return;
    }

    for (auto & player_direction_texture : game->player_direction_textures) {
        if (player_direction_texture) {
            SDL_DestroyTexture(player_direction_texture);
            player_direction_texture = nullptr;
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
