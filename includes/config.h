//
// Created by Dinis on 24/10/2026.
//

#ifndef BLANK_CONFIG_H
#define BLANK_CONFIG_H

#include "constants.h"

struct Config {
    const char* window_title = constants::app::kWindowTitle;
    int screen_width = constants::app::kScreenWidth;
    int screen_height = constants::app::kScreenHeight;
};

#endif //BLANK_CONFIG_H