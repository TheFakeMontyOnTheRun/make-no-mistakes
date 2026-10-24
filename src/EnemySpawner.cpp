#include "EnemySpawner.hpp"

EnemySpawner::EnemySpawner(SDL_Renderer *renderer, float delta,
                           std::function<void(Enemy *)> callback) noexcept
: m_renderer{renderer}, m_defaultCooldown{delta}, m_callback{callback} {
    m_enemyParts[0][0] = IMG_LoadTexture(m_renderer,"assets/sprites/soldier-legs.png");
    m_enemyParts[0][1] = IMG_LoadTexture(m_renderer,"assets/sprites/soldier-belly.png");
    m_enemyParts[0][2] = IMG_LoadTexture(m_renderer,"assets/sprites/soldier-neck.png");
    m_enemyParts[0][3] = IMG_LoadTexture(m_renderer,"assets/sprites/soldier-head.png");

    m_enemyParts[1][0] = IMG_LoadTexture(m_renderer,"assets/sprites/dracula-legs.png");
    m_enemyParts[1][1] = IMG_LoadTexture(m_renderer,"assets/sprites/dracula-belly.png");
    m_enemyParts[1][2] = IMG_LoadTexture(m_renderer,"assets/sprites/dracula-neck.png");
    m_enemyParts[1][3] = IMG_LoadTexture(m_renderer,"assets/sprites/dracula-head.png");

    m_enemyParts[2][0] = IMG_LoadTexture(m_renderer,"assets/sprites/magritte-legs.png");
    m_enemyParts[2][1] = IMG_LoadTexture(m_renderer,"assets/sprites/magritte-belly.png");
    m_enemyParts[2][2] = IMG_LoadTexture(m_renderer,"assets/sprites/magritte-neck.png");
    m_enemyParts[2][3] = IMG_LoadTexture(m_renderer,"assets/sprites/magritte-head.png");

    m_enemyParts[3][0] = IMG_LoadTexture(m_renderer,"assets/sprites/horse-legs.png");
    m_enemyParts[3][1] = IMG_LoadTexture(m_renderer,"assets/sprites/horse-belly.png");
    m_enemyParts[3][2] = IMG_LoadTexture(m_renderer,"assets/sprites/horse-neck.png");
    m_enemyParts[3][3] = IMG_LoadTexture(m_renderer,"assets/sprites/horse-head.png");
}

EnemySpawner::~EnemySpawner() noexcept {}

void EnemySpawner::update(float delta) {
	bool trigger = false;

    m_cooldown -= delta;

    while (m_cooldown < 0) {
		trigger = true;
		m_cooldown += m_defaultCooldown;
    }


    if (trigger) {

		uint8_t head = 0;
        uint8_t neck = 0;
        uint8_t belly = 0;
        uint8_t legs = 0;



        m_callback(new Enemy(m_enemyParts[head][0], m_enemyParts[neck][1],
                             m_enemyParts[belly][2], m_enemyParts[legs][3],
                             (rand() / ((float)RAND_MAX)) * 1000,
                             (rand() / ((float)RAND_MAX)) * 1000
                             ));
	}
}
