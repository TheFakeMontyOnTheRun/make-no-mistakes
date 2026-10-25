//
// Created by v-lucena on 24/10/26.
//

#include "timer.h"
#include <SDL.h>

void timer_init(Timer* timer, const Uint64 start_time, int countdown) {
    timer->start_time = start_time;
    timer->countdown = countdown;
}
float timer_get_time_in_seconds(const Timer *timer) {
    const auto elapsed = (SDL_GetTicks64() - timer->start_time) / 1000.0f;
    const float remaining = static_cast<float>(timer->countdown) - elapsed;
    return remaining > 0.0f ? remaining : 0.0f;
}