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
		SDL_Texture *m_legs;
        SDL_Texture *m_belly;
        SDL_Texture *m_neck;
        SDL_Texture *m_head;
        float m_x;
        float m_y;
        bool m_active = false;
        float m_animationTime = 1.0f;

        explicit Enemy(SDL_Texture *legs, SDL_Texture *belly, SDL_Texture *neck,
                       SDL_Texture *head, float x, float y)
            : m_head{head}, m_neck{neck}, m_belly{belly}, m_legs{legs}, m_x{x},
              m_y{y} {}

        virtual ~Enemy() noexcept {}
		Enemy(const Enemy &other) = delete;
		Enemy(const Enemy &&other) = delete;
		Enemy& operator=(Enemy&& other) = delete;

        void update(float delta) {
			if (!m_active) {
				m_animationTime -= delta;
			}

            if (m_animationTime < 0) {
                m_active = true;
            }

        }
	};

	class EnemySpawner {
	private:
        float m_cooldown;
        float m_defaultCooldown;
        SDL_Texture *m_enemyParts[4][4]; // 4 enemies made of 4 parts each
        std::function<void(Enemy *)> m_callback;
        SDL_Renderer *m_renderer;
	public:
		explicit EnemySpawner(SDL_Renderer *renderer, float interval, std::function<void(Enemy*)> callback) noexcept;
		virtual ~EnemySpawner() noexcept;
		EnemySpawner(const EnemySpawner &other) = delete;
		EnemySpawner(const EnemySpawner &&other) = delete;
		EnemySpawner& operator=(EnemySpawner&& other) = delete;
		void update(float delta);
	};

#ifdef __cplusplus
}
#endif

#endif // ENEMYSPAWNER_H
