//
// Created by Dinis on 25/10/2026.
//

#ifndef BLANK_DRAG_CRATE_H
#define BLANK_DRAG_CRATE_H

#include "puzzle.h"

#ifdef __cplusplus
extern "C" {
#endif

class DragCratePuzzle : public Puzzle {
    public:
    DragCratePuzzle(SDL_Rect area);

    ~DragCratePuzzle() override;

    void handle_event(SDL_Event &event) override;
    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;

    bool is_solved() const override;

    private:
    SDL_Texture* crate_texture{};
    SDL_Rect crate_rect{};
    SDL_Rect hole_rect{};

    bool dragging = false;
    bool solved = false;
    int offset_x = 0.0f;
    int offset_y = 0.0f;
};

#ifdef __cplusplus
}
#endif

#endif //BLANK_DRAG_CRATE_H
