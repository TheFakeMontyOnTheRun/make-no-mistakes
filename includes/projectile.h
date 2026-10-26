#ifndef PROJECTILE_H
#define PROJECTILE_H

#ifdef __cplusplus
#include "constants.h"
extern "C" {
#endif

typedef struct Game Game;

#define MAX_PROJECTILES constants::projectile::MAX_PROJECTILES

typedef struct Projectile {
    float x;
    float y;
    float vx;
    float vy;
    float radius;
    float speed;
    bool active;
} Projectile;

void projectile_init(Projectile *projectile, float x, float y, float dir_x, float dir_y, float speed);
void projectile_update(Projectile *projectile, float dt);
void fire_projectile(Game *game);
void cleanup_inactive_projectiles(Game *game);

#ifdef __cplusplus
}
#endif

#endif // PROJECTILE_H
