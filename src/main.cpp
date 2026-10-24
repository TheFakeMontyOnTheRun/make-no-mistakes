#include <SDL.h>

#include "game.h"
#include "config.h"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    Game game;
    Config config;
    game_init(&game, config.window_title, config.screen_width, config.screen_height);

    if (!game.window || !game.renderer) {
        return 1;
    }

    while (game.keep_running) {
        game_handle_events(&game);

        Uint64 current_ticks = SDL_GetPerformanceCounter();
        float delta_time = (float)(current_ticks - game.previous_ticks) / (float)SDL_GetPerformanceFrequency();
        game.previous_ticks = current_ticks;

        game_update(&game, delta_time);
        game_render(&game);
    }

    game_cleanup(&game);
    return 0;
}
