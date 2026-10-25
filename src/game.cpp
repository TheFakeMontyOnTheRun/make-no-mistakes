#include "game.h"
#include "util.h"

#include <SDL_image.h>
#include <stdio.h>

bool dragging = false;
int offset_x = 0;
int offset_y = 0;

static void fire_projectile(Game *game) {
    if (!game || game->projectile_count >= MAX_PROJECTILES) {
        return;
    }

    Projectile *shot = &game->projectiles[game->projectile_count];
    projectile_init(
        shot,
        game->player.x + (game->player.width * 0.5f),
        game->player.y + (game->player.height * 0.5f),
        game->player.facing_x,
        game->player.facing_y,
        game->projectile_speed
    );

    game->projectile_count++;
}

static void cleanup_inactive_projectiles(Game *game) {
    int active_count = 0;

    for (int i = 0; i < game->projectile_count; ++i) {
        Projectile *projectile = &game->projectiles[i];
        projectile_update(projectile, 0.0f);

        if (projectile->x < 0.0f || projectile->x > 640.0f ||
            projectile->y < 0.0f || projectile->y > 480.0f) {
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

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Cannot init SDL!\n");
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
        SDL_Quit();
        return;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, 0);
    if (!game->renderer) {
        fprintf(stderr, "Cannot create renderer!\n");
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        return;
    }

    game->bg_battle_texture = IMG_LoadTexture(
        game->renderer, "assets/bg/battle_area.png"
    );
    game->bg_battle = {
        0, 0, 500, 500
    };

    game->bg_puzzle_texture = IMG_LoadTexture(
        game->renderer, "assets/bg/puzzle_area.png"
    );
    game->bg_puzzle = {
        500, 0, 300, 500
    };

    game->crate_texture = IMG_LoadTexture(
        game->renderer, "assets/obj/crate.png"
        );

    game->crate_rect = {
        randomNumber(550, 750),
        randomNumber(50, 450),
        32, 32
    };

    font_init(&game->font, "assets/fonts/font_spritesheet.png", game->renderer);
    player_init(&game->player, 100.0f, 100.0f, 180.0f, 32.0f, 32.0f);
    game->projectile_count = 0;
    game->timer = 60;
    game->start_time = 0;
    game->keep_running = true;
    game->shoot_pressed = false;
    game->show_initial_screen = true;
    game->projectile_speed = 280.0f;
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

    SDL_SetRenderDrawColor(game->renderer, 20, 20, 30, 255);
    SDL_RenderClear(game->renderer);

    if (game->show_initial_screen) {
        font_render(&game->font, game->renderer, "Make No Mistakes", 100, 3.0f);
        font_render(&game->font, game->renderer, "Use WASD to move SPACE to shoot", 200, 2.0f);
        font_render(&game->font, game->renderer, "Press SPACE to start", 300, 2.0f);
    } else {
        SDL_Rect player_rect = {
            (int) game->player.x,
            (int) game->player.y,
            (int) game->player.width,
            (int) game->player.height
        };

        SDL_SetRenderDrawColor(game->renderer, 80, 200, 120, 255);
        SDL_RenderFillRect(game->renderer, &player_rect);

        for (int i = 0; i < game->projectile_count; ++i) {
            SDL_Rect shot = {
                (int) (game->projectiles[i].x - game->projectiles[i].radius),
                (int) (game->projectiles[i].y - game->projectiles[i].radius),
                (int) (game->projectiles[i].radius * 2.0f),
                (int) (game->projectiles[i].radius * 2.0f)
            };

            SDL_SetRenderDrawColor(game->renderer, 255, 200, 80, 255);
            SDL_RenderFillRect(game->renderer, &shot);
        }


        SDL_RenderCopy(
            game->renderer,
            game->crate_texture,
            nullptr,
            &game->crate_rect
        );

        SDL_RenderCopy(
            game->renderer,
            game->bg_battle_texture,
            nullptr,
            &game->bg_battle
        );
        SDL_RenderCopy(
            game->renderer,
            game->bg_puzzle_texture,
            nullptr,
            &game->bg_puzzle
        );

        const int remaining_time = game->timer - (SDL_GetTicks64() - game->start_time) / 1000.0f;
        font_render(&game->font, game->renderer, "Time: " + std::to_string(remaining_time), 10, 1.5f, START);
        font_render(&game->font, game->renderer, "Enemies: 5", 30, 1.5f, START);

    }

    SDL_RenderPresent(game->renderer);
}

void game_cleanup(Game *game) {
    if (!game) {
        return;
    }

    if (game->renderer) {
        SDL_DestroyRenderer(game->renderer);
        game->renderer = nullptr;
    }

    if (game->window) {
        SDL_DestroyWindow(game->window);
        game->window = nullptr;
    }

    font_cleanup(&game->font);

    SDL_Quit();
}
