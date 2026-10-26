#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
namespace math {
constexpr float PI = 3.14159265359f;
constexpr float HALF = 0.5f;
} // namespace math

namespace app {
constexpr const char* WINDOW_TITLE = "Make no Mistakes";
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 500;
} // namespace app

namespace world {
constexpr int BATTLE_X = 0;
constexpr int BATTLE_Y = 0;
constexpr int BATTLE_WIDTH = 500;
constexpr int BATTLE_HEIGHT = 500;

constexpr int PUZZLE_X = 500;
constexpr int PUZZLE_Y = 0;
constexpr int PUZZLE_WIDTH = 300;
constexpr int PUZZLE_HEIGHT = 500;
    constexpr int PUZZLE_COOLDOWN = 5;
    constexpr int TERMINAL_SIZE_W = 32;
    constexpr int TERMINAL_SIZE_H = 48;

constexpr int CRATE_SIZE = 32;
    constexpr int HOLE_SIZE = 48;
    constexpr int BUTTON_SIZE = 48;
} // namespace world

namespace player {
constexpr float START_X = 100.0f;
constexpr float START_Y = 100.0f;
constexpr float SPEED = 180.0f;
constexpr float COLLISION_WIDTH = 32.0f;
constexpr float COLLISION_HEIGHT = 32.0f;
constexpr int RENDER_WIDTH = 32;
constexpr int RENDER_HEIGHT = 32;
constexpr float PROJECTILE_SPAWN_OFFSET = 22.0f;
constexpr int START_HEALTH = 250;
constexpr int MAX_HEALTH = 250;
constexpr float INVULNERABILITY_TIME = 0.5f;
} // namespace player

namespace projectile {
constexpr int MAX_PROJECTILES = 5;
constexpr float SPEED = 500.0f;
constexpr float RADIUS = 3.0f;
} // namespace projectile

namespace hud {
constexpr int TIMER_SECONDS = 60;
constexpr int INITIAL_TITLE_Y = 100;
constexpr int INITIAL_INSTRUCTIONS_Y = 200;
constexpr int INITIAL_START_Y = 300;
constexpr float INITIAL_TITLE_SCALE = 3.0f;
constexpr float INITIAL_TEXT_SCALE = 2.0f;
constexpr int TIME_TEXT_Y = 10;
constexpr int ENEMIES_TEXT_Y = 30;
constexpr float HUD_SCALE = 1.5f;
} // namespace hud

namespace font {
constexpr int GLYPH_SIZE = 8;
constexpr int GLYPH_SPACING = 1;
constexpr int SPACE_WIDTH = 4;
constexpr int DEFAULT_START_X = 50;
} // namespace font

namespace visual {
constexpr int BACKGROUND_R = 20;
constexpr int BACKGROUND_G = 20;
constexpr int BACKGROUND_B = 30;

constexpr int FALLBACK_PLAYER_R = 80;
constexpr int FALLBACK_PLAYER_G = 200;
constexpr int FALLBACK_PLAYER_B = 120;

constexpr int PROJECTILE_R = 29;
constexpr int PROJECTILE_G = 206;
constexpr int PROJECTILE_B = 2;

constexpr int CHECKERBOARD_THRESHOLD = 180;
constexpr int CHECKERBOARD_TOLERANCE = 18;
} // namespace visual
} // namespace constants

#endif // CONSTANTS_H
