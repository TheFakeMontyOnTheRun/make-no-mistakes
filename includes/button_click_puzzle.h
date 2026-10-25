//
// Created by Dinis on 25/10/2026.
//

#ifndef BLANK_BUTTON_CLICK_PUZZLE_H
#define BLANK_BUTTON_CLICK_PUZZLE_H
#include <SDL_events.h>
#include <SDL_render.h>

#include "puzzle.h"

#ifdef __cplusplus
extern "C" {
#endif

class ButtonClickPuzzle : public Puzzle {
public:
    ButtonClickPuzzle(SDL_Rect area);

    void handle_event(SDL_Event &event) override;
    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;

    bool is_solved() const override;

private:
    SDL_Rect button_rect{};
    bool solved = false;
};

#ifdef __cplusplus
}
#endif


#endif //BLANK_BUTTON_CLICK_PUZZLE_H
