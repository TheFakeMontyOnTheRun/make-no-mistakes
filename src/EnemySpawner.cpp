#include "EnemySpawner.hpp"

EnemySpawner::EnemySpawner(float delta,
                           std::function<void(Enemy*)> callback) noexcept
: m_defaultCooldown{delta}, m_callback{callback} {
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
		m_callback(new Enemy());
	}
}
