//
// Created by Dinis on 25/10/2026.
//

#include "puzzle.h"

void Puzzle::keep_inside_area(SDL_Rect& object) const {
    if (object.x < puzzle_area_limit.x) {
        object.x = puzzle_area_limit.x;
    }
    if (object.y < puzzle_area_limit.y) {
        object.y = puzzle_area_limit.y;
    }
    if (object.x + object.w > puzzle_area_limit.x + puzzle_area_limit.w) {
        object.x = puzzle_area_limit.x + puzzle_area_limit.w - object.w;
    }
    if (object.y + object.h > puzzle_area_limit.y + puzzle_area_limit.h) {
        object.y = puzzle_area_limit.y + puzzle_area_limit.h - object.h;
    }
}