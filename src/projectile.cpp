#include "projectile.h"

#include "constants.h"
#include "game.h"
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
    projectile->radius = constants::projectile::RADIUS;
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

void fire_projectile(Game *game) {
    if (!game || game->projectile_count >= MAX_PROJECTILES) {
        return;
    }

    const float spawn_x = game->player.x + (game->player.width * constants::math::HALF) +
                          (game->player.facing_x * constants::player::PROJECTILE_SPAWN_OFFSET);
    const float spawn_y = game->player.y + (game->player.height * constants::math::HALF) +
                          (game->player.facing_y * constants::player::PROJECTILE_SPAWN_OFFSET);

    Projectile *shot = &game->projectiles[game->projectile_count];
    projectile_init(
        shot,
        spawn_x,
        spawn_y,
        game->player.facing_x,
        game->player.facing_y,
        game->projectile_speed
    );

    game->projectile_count++;
    music_manager_play_sound(&game->music_manager, "shoot");
}

void cleanup_inactive_projectiles(Game *game) {
    if (!game) {
        return;
    }

    const int world_left = game->bg_battle.x;
    const int world_top = game->bg_battle.y;
    const int world_right = game->bg_battle.x + game->bg_battle.w;
    const int world_bottom = game->bg_battle.y + game->bg_battle.h;

    int active_count = 0;
    for (int i = 0; i < game->projectile_count; ++i) {
        Projectile *projectile = &game->projectiles[i];

        if (projectile->x + projectile->radius < world_left ||
            projectile->x - projectile->radius > world_right ||
            projectile->y + projectile->radius < world_top ||
            projectile->y - projectile->radius > world_bottom) {
            projectile->active = false;
        }

        if (projectile->active) {
            game->projectiles[active_count++] = *projectile;
        }
    }

    game->projectile_count = active_count;
}
