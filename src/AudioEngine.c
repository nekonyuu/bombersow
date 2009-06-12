/*
    GPL v3 Licence :
    Bombersow is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bombersow is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bombersow.  If not, see <http://www.gnu.org/licenses/>.


    Creative Commons BY-NC-SA :
    This work is licensed under the Creative Commons Attribution-Noncommercial-Share Alike 3.0 Unported License.
    To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter
    to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.

*/

#include <SFML/Audio.h>

#include "AudioEngine.h"

sfMutex* g_AudioEngine_SoundsUpdate;

AudioEngine* AudioEngine_Init()
{
    AudioEngine* engine = NULL;
    assert(engine = (AudioEngine*) malloc(sizeof(AudioEngine)));

    engine->soundbuffers = NULL;
    engine->nb_soundbuffers = 0;

    engine->sounds = NULL;

    engine->musics = NULL;
    engine->nb_musics = 0;

    engine->started = sfTrue;

    g_AudioEngine_SoundsUpdate = sfMutex_Create();

    return engine;
}

void AudioEngine_Destroy(AudioEngine* engine)
{
    for(int i = 0; i < engine->nb_sounds; i++)
        sfSound_Destroy(engine->sounds[i]);

    for(int i = 0; i < engine->nb_sounds; i++)
        sfSoundBuffer_Destroy(engine->soundbuffers[i]);

    for(int i = 0; i < engine->nb_sounds; i++)
        sfMusic_Destroy(engine->musics[i]);
}

void AudioEngine_UpdateEngine(void* UserData)
{
    AudioEngine* engine = (AudioEngine*) UserData;

    while(engine->started)
    {
        for(AudioEngineSounds* ptr = engine->sounds; ptr != NULL; ptr = ptr->next)
        {

        }

        sfSleep(0.5f);
    }
}

void AudioEngine_LoadSound(AudioEngine* engine, char* path)
{
    assert(engine->soundbuffers = realloc(engine->soundbuffers, ++engine->nb_soundbuffers * sizeof(sfSoundBuffer*)));
    engine->soundbuffers[engine->nb_soundbuffers - 1] = sfSoundBuffer_CreateFromFile(path);
}

void AudioEngine_LoadMusic(AudioEngine* engine, char* path)
{
    assert(engine->musics = realloc(engine->musics, ++engine->nb_musics * sizeof(sfMusic*)));
    engine->musics[engine->nb_musics - 1] = sfMusic_CreateFromFile(path);
}

void AudioEngine_PlaySound(AudioEngine* engine, int index)
{
    sfMutex_Lock(g_AudioEngine_SoundsUpdate);

    AudioEngineSounds* new_sound = NULL;
    assert(new_sound = malloc(sizeof(AudioEngineSounds*)));

    new_sound->sound = sfSound_Create();

    sfSound_SetBuffer(engine->sounds[engine->nb_sounds - 1], engine->soundbuffers[index]);
    sfSound_SetLoop(engine->sounds[engine->nb_sounds - 1], sfFalse);
    sfSound_Play(engine->sounds[engine->nb_sounds - 1]);

    sfMutex_Unlock(g_AudioEngine_SoundsUpdate);
}
