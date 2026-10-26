#ifndef BLANK_PUZZLE_H
#define BLANK_PUZZLE_H
#include <SDL_events.h>
#include <SDL_render.h>

#ifdef __cplusplus
extern "C" {
#endif

class Puzzle {
    public:
    virtual ~Puzzle() = default;

    virtual void handle_event(SDL_Event &event) = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual bool is_solved() const = 0;

    void keep_inside_area(SDL_Rect &object) const;

    SDL_Rect puzzle_area_limit = {};
    float time_remaining = 0.0f;
};

#ifdef __cplusplus
}
#endif

#endif //BLANK_PUZZLE_H
