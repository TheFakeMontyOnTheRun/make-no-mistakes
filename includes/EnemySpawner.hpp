#ifndef ENEMYSPAWNER_H
#define ENEMYSPAWNER_H

#include <cstdint>
#include <functional>

#include <SDL.h>
#include <SDL_image.h>

#ifdef __cplusplus
extern "C" {
#endif

	class Enemy {
    public:
		SDL_Surface *head;
        SDL_Surface *neck;
        SDL_Surface *belly;
        SDL_Surface *legs;
	};

	class EnemySpawner {
	private:
        float m_cooldown;
        float m_defaultCooldown;
        SDL_Surface *m_enemyParts[4][4]; // 4 enemies made of 4 parts each
        std::function<void(Enemy*)> m_callback;
	public:
		explicit EnemySpawner(float interval, std::function<void(Enemy*)> callback) noexcept;
		virtual ~EnemySpawner();
		EnemySpawner(const EnemySpawner &other) = delete;
		EnemySpawner(const EnemySpawner &&other) = delete;
		EnemySpawner& operator=(EnemySpawner&& other) = delete;
		void update(float delta);
	};

#ifdef __cplusplus
}
#endif

#endif // ENEMYSPAWNER_H
