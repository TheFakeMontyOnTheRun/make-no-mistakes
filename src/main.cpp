#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#include <emscripten/emscripten.h>
#endif

#include "game.h"
#include "config.h"

Game game;

#ifdef __EMSCRIPTEN__
void mainLoop (void) {
	game_handle_events(&game);

    Uint64 current_ticks = SDL_GetPerformanceCounter();
    float delta_time = (float)(current_ticks - game.previous_ticks) / (float)SDL_GetPerformanceFrequency();
    game.previous_ticks = current_ticks;

    game_update(&game, delta_time);
    game_render(&game);
}
#endif

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    Config config;
    game_init(&game, "Make no Mistakes", config.screen_width, config.screen_height);

    if (!game.window || !game.renderer) {
        return 1;
    }

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(mainLoop, 0, 1);
#else
    while (game.keep_running) {
        game_handle_events(&game);

        Uint64 current_ticks = SDL_GetPerformanceCounter();
        float delta_time = (float)(current_ticks - game.previous_ticks) / (float)SDL_GetPerformanceFrequency();
        game.previous_ticks = current_ticks;

        game_update(&game, delta_time);
        game_render(&game);
    }
#endif

    game_cleanup(&game);
    return 0;
}
