//
// Created by v-lucena on 23/10/26.
//

#ifndef FONT_H
#define FONT_H

#include <SDL_render.h>
#include <string>

typedef struct Font {
    std::string filepath;
    SDL_Texture* texture;
} Font;

void font_init(Font* font, const std::string& filepath, SDL_Renderer* renderer);
void font_render(Font* font, SDL_Renderer* renderer, const std::string& text, int y, float scale);
void font_cleanup(Font* font);

#endif //FONT_H
