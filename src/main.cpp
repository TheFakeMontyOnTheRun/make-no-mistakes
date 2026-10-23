#include <SDL.h>

#include "game.h"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    Game game;
    game_init(&game, "Make no Mistakes", 640, 480);

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
