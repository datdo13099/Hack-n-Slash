#include "soundManager.h"

SoundManager SoundManager::soundManager;

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
    for (auto& sound : sounds)
    {
        Mix_FreeChunk(sound.sound);
    }
    sounds.clear();
}

void SoundManager::loadSound(string name, string file)
{
    Mix_Chunk* sound = Mix_LoadWAV(file.c_str());
    if (sound != NULL)
    {
        soundListing newSound;
        newSound.sound = sound;
        newSound.name = name;
        sounds.push_back(newSound);
    }
}

void SoundManager::playSound(string name)
{
    for (auto& sound : sounds)
    {
        if (sound.name == name)
        {
            Mix_PlayChannel(-1, sound.sound, 0);
            break;
        }
    }
}

void SoundManager::setAllSoundVolume(int volume)
{
    for (auto& sound : sounds)
    {
        Mix_VolumeChunk(sound.sound, volume);
    }
}