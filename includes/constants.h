#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
namespace math {
constexpr float kPi = 3.14159265359f;
constexpr float kHalf = 0.5f;
} // namespace math

namespace app {
constexpr const char* kWindowTitle = "Make no Mistakes";
constexpr int kScreenWidth = 1600;
constexpr int kScreenHeight = 900;
} // namespace app

namespace world {
constexpr int kBattleX = 0;
constexpr int kBattleY = 0;
constexpr int kBattleWidth = 1100;
constexpr int kBattleHeight = 900;

constexpr int kPuzzleX = 1100;
constexpr int kPuzzleY = 0;
constexpr int kPuzzleWidth = 500;
constexpr int kPuzzleHeight = 900;

constexpr int kCrateMinX = 1150;
constexpr int kCrateMaxX = 1550;
constexpr int kCrateMinY = 50;
constexpr int kCrateMaxY = 850;
constexpr int kCrateSize = 64;
} // namespace world

namespace player {
constexpr float kStartX = 100.0f;
constexpr float kStartY = 100.0f;
constexpr float kSpeed = 180.0f;
constexpr float kCollisionWidth = 32.0f;
constexpr float kCollisionHeight = 32.0f;
constexpr int kRenderWidth = 64;
constexpr int kRenderHeight = 64;
constexpr float kProjectileSpawnOffset = 22.0f;
} // namespace player

namespace projectile {
constexpr int kMaxProjectiles = 5;
constexpr float kSpeed = 500.0f;
constexpr float kRadius = 3.0f;
} // namespace projectile

namespace hud {
constexpr int kTimerSeconds = 60;
constexpr int kInitialTitleY = 100;
constexpr int kInitialInstructionsY = 200;
constexpr int kInitialStartY = 300;
constexpr float kInitialTitleScale = 3.0f;
constexpr float kInitialTextScale = 2.0f;
constexpr int kTimeTextY = 10;
constexpr int kEnemiesTextY = 30;
constexpr float kHudScale = 1.5f;
} // namespace hud

namespace font {
constexpr int kGlyphSize = 8;
constexpr int kGlyphSpacing = 1;
constexpr int kSpaceWidth = 4;
constexpr int kDefaultStartX = 50;
} // namespace font

namespace visual {
constexpr int kBackgroundR = 20;
constexpr int kBackgroundG = 20;
constexpr int kBackgroundB = 30;

constexpr int kFallbackPlayerR = 80;
constexpr int kFallbackPlayerG = 200;
constexpr int kFallbackPlayerB = 120;

constexpr int kProjectileR = 29;
constexpr int kProjectileG = 206;
constexpr int kProjectileB = 2;

constexpr int kCheckerboardThreshold = 180;
constexpr int kCheckerboardTolerance = 18;
} // namespace visual
} // namespace constants

#endif // CONSTANTS_H
