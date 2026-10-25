//
// Created by Dinis on 24/10/2026.
//

#ifndef BLANK_CONFIG_H
#define BLANK_CONFIG_H

#include "constants.h"

struct Config {
    const char* window_title = constants::app::WINDOW_TITLE;
    int screen_width = constants::app::SCREEN_WIDTH;
    int screen_height = constants::app::SCREEN_HEIGHT;
};

#endif //BLANK_CONFIG_H