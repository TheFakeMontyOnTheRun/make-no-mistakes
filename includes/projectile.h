#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "constants.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>


#define MAX_PROJECTILES constants::projectile::kMaxProjectiles

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

#ifdef __cplusplus
}
#endif

#endif // PROJECTILE_H
