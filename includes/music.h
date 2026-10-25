//
// Created by v-lucena on 25/10/26.
//

#ifndef BLANK_MUSIC_H
#define BLANK_MUSIC_H
#include <SDL_mixer.h>
#include <string>
#include <unordered_map>

typedef struct MusicManager {
    Mix_Music* bg_music;
    std::unordered_map<std::string, Mix_Chunk*> sound_effects;
} MusicManager;

void music_manager_init(MusicManager* manager);
void music_manager_play_sound(MusicManager* manager, const std::string& sound_name);
void music_manager_cleanup(MusicManager* manager);
#endif //BLANK_MUSIC_H
