//
// Created by v-lucena on 25/10/26.
//

#include "music.h"

void music_manager_init(MusicManager* manager) {
    if (!Mix_Init(MIX_INIT_MP3)) {
        fprintf(stderr, "Cannot init SDL_mix!\n");
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Cannot open audio!\n");
    }

    manager->bg_music = Mix_LoadMUS("assets/music/main_song.mp3");
    if (!manager->bg_music) {
        fprintf(stderr, "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    if (Mix_PlayMusic(manager->bg_music, -1) == -1) {
        fprintf(stderr, "Failed to play music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    Mix_Chunk* shoot_sound = nullptr;
    shoot_sound = Mix_LoadWAV("assets/music/shoot_sound.wav");
    if (!shoot_sound) {
        fprintf(stderr, "Failed to load shoot sound! SDL_mixer Error: %s\n", Mix_GetError());
    } else {
        manager->sound_effects["shoot"] = shoot_sound;
    }
}

void music_manager_play_sound(MusicManager* manager, const std::string& sound_name) {
    const auto sound_effect = manager->sound_effects.find(sound_name);
    if (sound_effect != manager->sound_effects.end()) {
        Mix_PlayChannel(-1, sound_effect->second, 0);
    } else {
        fprintf(stderr, "Sound effect '%s' not found!\n", sound_name.c_str());
    }
}

void music_manager_cleanup(MusicManager* manager) {
    Mix_FreeMusic(manager->bg_music);
    for (const auto& pair : manager->sound_effects) {
        Mix_FreeChunk(pair.second);
    }
    manager->sound_effects.clear();
    Mix_Quit();
}
