//
// Created by v-lucena on 23/10/26.
//

#include "font.h"

#include <SDL_image.h>

#include <unordered_map>

namespace {

struct FontPos {
    int x;
    int y;
};

const std::unordered_map<char, FontPos> kGlyphMap = {
    {'A', {0, 0}}, {'B', {8, 0}}, {'C', {16, 0}}, {'D', {24, 0}}, {'E', {32, 0}},
    {'F', {40, 0}}, {'G', {48, 0}}, {'H', {56, 0}}, {'I', {64, 0}}, {'J', {72, 0}},
    {'K', {0, 8}}, {'L', {8, 8}}, {'M', {16, 8}}, {'N', {24, 8}}, {'O', {32, 8}},
    {'P', {40, 8}}, {'Q', {48, 8}}, {'R', {56, 8}}, {'S', {64, 8}}, {'T', {72, 8}},
    {'U', {0, 16}}, {'V', {8, 16}}, {'W', {16, 16}}, {'X', {24, 16}}, {'Y', {32, 16}},
    {'Z', {40, 16}}, {'a', {48, 16}}, {'b', {56, 16}}, {'c', {64, 16}}, {'d', {72, 16}},
    {'e', {0, 24}}, {'f', {8, 24}}, {'g', {16, 24}}, {'h', {24, 24}}, {'i', {32, 24}},
    {'j', {40, 24}}, {'k', {48, 24}}, {'l', {56, 24}}, {'m', {64, 24}}, {'n', {72, 24}},
    {'o', {0, 32}}, {'p', {8, 32}}, {'q', {16, 32}}, {'r', {24, 32}}, {'s', {32, 32}},
    {'t', {40, 32}}, {'u', {48, 32}}, {'v', {56, 32}}, {'w', {64, 32}}, {'x', {72, 32}},
    {'y', {0, 40}}, {'z', {8, 40}}, {'0', {16, 40}}, {'1', {24, 40}}, {'2', {32, 40}},
    {'3', {40, 40}}, {'4', {48, 40}}, {'5', {56, 40}}, {'6', {64, 40}}, {'7', {72, 40}},
    {'8', {0, 48}}, {'9', {8, 48}}
};

const int kGlyphSize = 8;
const int kGlyphSpacing = 1;
const int kSpaceWidth = 4;

} // namespace

void font_init(Font* font, const std::string& filepath, SDL_Renderer* renderer)
{
    if (!font) {
        return;
    }

    font->filepath = filepath;
    font->texture = nullptr;

    if (!font->filepath.empty() && renderer) {
        font->texture = IMG_LoadTexture(renderer, font->filepath.c_str());
    }
}

void font_render(Font* font, SDL_Renderer* renderer, const std::string& text, int y, float scale, TextAlign align)
{
    if (!font || !renderer || text.empty()) {
        return;
    }

    if (!font->texture && !font->filepath.empty()) {
        font->texture = IMG_LoadTexture(renderer, font->filepath.c_str());
    }

    if (!font->texture) {
        return;
    }

    int text_width, window_width = 0;
    SDL_GetRendererOutputSize(renderer, &window_width, nullptr);
    for (const char ch : text) {
        if (ch == ' ') {
            text_width += static_cast<int>(kSpaceWidth * scale);
            continue;
        }

        const auto it = kGlyphMap.find(ch);
        if (it != kGlyphMap.end()) {
            text_width += static_cast<int>((kGlyphSize + kGlyphSpacing) * scale);
        }
    }

    int draw_x = 0;
    if (align == CENTER) {
        draw_x = (window_width - text_width) / 2;
    } else {
        draw_x = 50;
    }
    const int draw_y = y;

    for (const char ch : text) {
        if (ch == ' ') {
            draw_x += static_cast<int>(kSpaceWidth * scale);
            continue;
        }

        const auto it = kGlyphMap.find(ch);
        if (it == kGlyphMap.end()) {
            continue;
        }

        const FontPos glyph = it->second;
        const SDL_Rect src = { glyph.x, glyph.y, kGlyphSize, kGlyphSize };
        const SDL_Rect dst = {
            draw_x,
            draw_y,
            static_cast<int>(kGlyphSize * scale),
            static_cast<int>(kGlyphSize * scale)
        };

        SDL_RenderCopy(renderer, font->texture, &src, &dst);
        draw_x += static_cast<int>((kGlyphSize + kGlyphSpacing) * scale);
    }
}

void font_cleanup(Font* font)
{
    if (!font) {
        return;
    }

    if (font->texture) {
        SDL_DestroyTexture(font->texture);
        font->texture = nullptr;
    }
}
