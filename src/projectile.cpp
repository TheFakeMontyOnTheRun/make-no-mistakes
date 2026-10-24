#include "projectile.h"

#include "constants.h"
#include "util.h"

void projectile_init(Projectile *projectile, float x, float y, float dir_x, float dir_y, float speed) {
    if (!projectile) {
        return;
    }

    normalize_vector(&dir_x, &dir_y);

    projectile->x = x;
    projectile->y = y;
    projectile->vx = dir_x * speed;
    projectile->vy = dir_y * speed;
    projectile->radius = constants::projectile::kRadius;
    projectile->speed = speed;
    projectile->active = true;
}

void projectile_update(Projectile *projectile, float dt) {
    if (!projectile || !projectile->active) {
        return;
    }

    projectile->x += projectile->vx * dt;
    projectile->y += projectile->vy * dt;
}
