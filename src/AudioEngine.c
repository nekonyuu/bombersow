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

#include "BaseSystem/Logging.h"
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

    engine->update_thread = sfThread_Create(&AudioEngine_UpdateEngine, engine);
    sfThread_Launch(engine->update_thread);

    return engine;
}

void AudioEngine_Destroy(AudioEngine* engine)
{
    AudioEngineSound* ptr = engine->sounds, *ptr2 = NULL;

    engine->started = sfFalse;

    sfThread_Wait(engine->update_thread);
    sfThread_Destroy(engine->update_thread);

    while (ptr != NULL)
    {
        ptr2 = ptr->next;
        sfSound_Destroy(ptr->sound);
        ptr = ptr2;
    }

    for (int i = 0; i < engine->nb_soundbuffers; i++)
        sfSoundBuffer_Destroy(engine->soundbuffers[i]);

    for (int i = 0; i < engine->nb_musics; i++)
        sfMusic_Destroy(engine->musics[i]);
}

void AudioEngine_UpdateEngine(void* UserData)
{
    AudioEngine* engine = (AudioEngine*) UserData;
    AudioEngineSound* ptr = engine->sounds, *ptr2 = NULL;

    while (engine->started)
    {
        while (ptr != NULL)
        {
            ptr2 = ptr->next;

            sfMutex_Lock(g_AudioEngine_SoundsUpdate);

            if (sfSound_GetStatus(ptr->sound) == sfStopped)
            {
                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev;

                sfSound_Destroy(ptr->sound);
                free(ptr);
            }

            sfMutex_Unlock(g_AudioEngine_SoundsUpdate);

            ptr = ptr2;
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

    AudioEngineSound* new_sound = NULL;
    assert(new_sound = malloc(sizeof(AudioEngineSound*)));

    new_sound->sound = sfSound_Create();

    if (engine->sounds)
    {
        engine->sounds->prev = new_sound;
        new_sound->next = engine->sounds;
        new_sound->prev = NULL;
        engine->sounds = new_sound;
    }
    else
    {
        engine->sounds = new_sound;
        new_sound->prev = NULL;
        new_sound->next = NULL;
    }

    sfSound_SetBuffer(new_sound->sound, engine->soundbuffers[index]);
    sfSound_SetLoop(new_sound->sound, sfFalse);
    sfSound_Play(new_sound->sound);

    sfMutex_Unlock(g_AudioEngine_SoundsUpdate);
}
