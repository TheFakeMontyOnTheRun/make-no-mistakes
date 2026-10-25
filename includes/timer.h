//
// Created by v-lucena on 24/10/26.
//

#ifndef BLANK_TIMER_H
#define BLANK_TIMER_H
#include <SDL_stdinc.h>

typedef struct Timer {
    Uint64 start_time;
    int countdown;
} Timer;

void timer_init(Timer* timer, Uint64 start_time, int countdown);
float timer_get_time_in_seconds(const Timer *timer);
#endif //BLANK_TIMER_H
