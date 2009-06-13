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

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "BaseSystem/Logging.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/GravitySystem.h"
#include "PhysicsEngine/CollisionSystem.h"
#include "PhysicsEngine/ParticleSystem.h"
#include "Objects/GameObjects.h"
#include "GraphicEngine/Draw.h"

static const int max_particles = 10000;
static const int nb_blood_particles = 30;
sfMutex* g_ParticleEngine_Mutex;

void ParticleEngine_Init(Map* map)
{
    ParticleEngine* engine = NULL;
    assert(engine = malloc(sizeof(ParticleEngine)));

    g_ParticleEngine_Mutex = sfMutex_Create();

    map->particle_engine = engine;

    engine->table = particle_table_Create();
    engine->blood_part_list = NULL;

    engine->running = true;

    engine->create_thread = sfThread_Create(&ParticleEngine_CreateParticles, engine);
    engine->update_thread = sfThread_Create(&gravitysystem_BloodUpdate, map);

    sfThread_Launch(engine->create_thread);
    sfThread_Launch(engine->update_thread);
}

void ParticleEngine_Clean(ParticleEngine* engine)
{
    BloodParticleInfo* ptr = engine->blood_part_list, *ptr2 = NULL;

    engine->running = false;

    sfThread_Wait(engine->update_thread);
    sfThread_Wait(engine->create_thread);

    sfThread_Destroy(engine->update_thread);
    sfThread_Destroy(engine->create_thread);

    sfMutex_Destroy(g_ParticleEngine_Mutex);

    particle_table_Destroy(engine->table);

    while (ptr != NULL)
    {
        ptr2 = ptr->next;
        free(ptr);
        ptr = ptr2;
    }

    free(engine);
}

void ParticleEngine_CreateParticles(void* UserData)
{
    ParticleEngine* engine = (ParticleEngine*) UserData;
    BloodParticleInfo* ptr = NULL, *ptr2 = NULL;

    while (engine->running)
    {
        sfMutex_Lock(g_ParticleEngine_Mutex);

        ptr = engine->blood_part_list;
        engine->blood_part_list = NULL;

        sfMutex_Unlock(g_ParticleEngine_Mutex);

        while (ptr != NULL)
        {
            ptr2 = ptr->next;

            for (int i = 0; i < nb_blood_particles; i++)
            {
                logging_Info("ParticleEngine_CreateParticles", "Creating particles...");

                Particle* particle = particle_CreateBlood();
                particle_SetPosition(particle, ptr->coord_x, ptr->coord_y);
                particle->speed_x = ptr->speed_x / (3 + (i % 5));
                particle->speed_y = ptr->speed_y / (5 + (i % 5));

                float angle = (i % 2) ? -(i / 2 + 1)* 3.14 / 72 : (i / 2)* 3.14 / 72;
                float vec_x2 = particle->speed_x * cos(angle) + particle->speed_y * sin(angle);
                float vec_y2 = -particle->speed_x * sin(angle) + particle->speed_y * cos(angle);

                particle->speed_x = vec_x2;
                particle->speed_y = vec_y2;

                particle_table_AddParticle(engine->table, particle);
            }

            free(ptr);

            ptr = ptr2;
        }

        sfSleep(0.005f);
    }
}

void ParticleEngine_CreateBlood(ParticleEngine* engine, int coord_x, int coord_y, float speed_x, float speed_y)
{
    BloodParticleInfo* info = NULL;
    assert(info = malloc(sizeof(BloodParticleInfo)));

    info->coord_x = coord_x;
    info->coord_y = coord_y;

    info->speed_x = speed_x;
    info->speed_y = speed_y;

    if(!engine->blood_part_list)
    {
        info->next = NULL;

        sfMutex_Lock(g_ParticleEngine_Mutex);

        engine->blood_part_list = info;

        sfMutex_Unlock(g_ParticleEngine_Mutex);
    }
    else
    {
        BloodParticleInfo* ptr = NULL;
        for(ptr = engine->blood_part_list; ptr->next != NULL; ptr = ptr->next);

        info->next = NULL;
        ptr->next = info;
    }
}

Particle* particle_Create()
{
    Particle* particle = NULL;
    assert(particle = (Particle*) malloc(sizeof(Particle)));

    particle->x = 0;
    particle->y = 0;

    particle->size_x = 0;
    particle->size_y = 0;

    particle->speed_x = 0;
    particle->speed_y = 0;

    return particle;
}

Particle* particle_CreateBlood()
{
    Particle* particle = particle_Create();

    particle->particle_color = sfColor_FromRGB(255,0,0);

    particle->size_x = 3;
    particle->size_y = 3;

    return particle;
}

inline void particle_SetPosition(Particle* particle, int x, int y)
{
    particle->x = x;
    particle->y = y;
}

inline int Particle_GetX(Particle* particle)
{
    return particle->x;
}

inline int Particle_GetY(Particle* particle)
{
    return particle->y;
}

inline void Particle_SetX(Particle* particle, int x)
{
    particle->x = x;
}

inline void Particle_SetY(Particle* particle, int y)
{
    particle->y = y;
}

inline int Particle_GetSizeX(Particle* particle)
{
    return particle->size_x;
}

inline int Particle_GetSizeY(Particle* particle)
{
    return particle->size_y;
}

inline void Particle_SetSizeX(Particle* particle, int x)
{
    particle->size_x = x;
}

inline void Particle_SetSizeY(Particle* particle, int y)
{
    particle->size_y = y;
}

inline float Particle_GetSpeedX(Particle* particle)
{
    return particle->speed_x;
}

inline float Particle_GetSpeedY(Particle* particle)
{
    return particle->speed_y;
}

inline void Particle_SetSpeedX(Particle* particle, float speed)
{
    particle->speed_x = speed;
}

inline void Particle_SetSpeedY(Particle* particle, float speed)
{
    particle->speed_y = speed;
}

void particle_Draw(Particle* particle, Config* config)
{
    // Couleur rouge sang
    glColor3f(particle->particle_color.r, particle->particle_color.g, particle->particle_color.b);
    // Repère à origine différente
    glBegin(GL_TRIANGLE_STRIP); // Dessin par triangles assemblés, plus performant pour le GPU
    glVertex2i(particle->x, config->height - particle->y - particle->size_y);
    glVertex2i(particle->x + particle->size_x, config->height - particle->y - particle->size_y);
    glVertex2i(particle->x + particle->size_x, config->height - particle->y);
    glVertex2i(particle->x, config->height - particle->y);
    glEnd();
}

void particle_Destroy(Particle* particle)
{
    if (particle != NULL)
        free(particle);
}

Particle_Table* particle_table_Create()
{
    Particle_Table* particle_table = NULL;
    assert(particle_table = (Particle_Table*)malloc(sizeof(Particle_Table)));

    particle_table->nbr_particle = 0;
    particle_table->indice_courant = 0;
    particle_table->nbr_max = max_particles;

    particle_table->particle = (Particle**) malloc(particle_table->nbr_max * sizeof(Particle*));
    for (int i = 0; i < particle_table->nbr_max; i++)
        particle_table->particle[i] = NULL;

    return particle_table;
}

void particle_table_Destroy(Particle_Table* particle_table)
{
    for (int i = 0; i < particle_table->nbr_max; i++)
        particle_Destroy(particle_table->particle[i]);

    free(particle_table->particle);
    free(particle_table);
}

void particle_table_AddParticle(Particle_Table* particle_table, Particle* particle)
{
    particle_Destroy(particle_table->particle[particle_table->indice_courant]);
    particle_table->particle[particle_table->indice_courant] = particle;
    if (particle_table->nbr_particle + 1 < particle_table->nbr_max)
        particle_table->nbr_particle++;
    particle_table->indice_courant++;
    if (particle_table->indice_courant >= particle_table->nbr_max)
        particle_table->indice_courant = 0;
}

void particle_table_Draw(sfRenderWindow* Game, Particle_Table* particle_table, Config* config)
{
    sfRenderWindow_SetActive(Game, sfTrue);

    for (int i = 0; i < particle_table->nbr_particle; i++)
        particle_Draw(particle_table->particle[i], config);
}
