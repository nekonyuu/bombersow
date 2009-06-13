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

#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

typedef struct PARTICLE
{
    sfColor particle_color;     // Couleur de la particule
    int x, y;                   // Coordonnées
    int size_x, size_y;         // Taille
    float speed_x, speed_y;     // Vitesse
} Particle;

typedef struct PARTICLE_TABLE
{
    int nbr_particle;       //nombre actuel de particule
    int nbr_max;            //nombre maximum de particule
    int indice_courant;     //indice actuel du tableau
    Particle** particle;    //tableau de particule
} Particle_Table;

// Structure pour chaque groupe de particule de sang à créer
typedef struct BLOOD_PARTICLES_INFO
{
    int coord_x, coord_y;
    float speed_x, speed_y;

    struct BLOOD_PARTICLES_INFO* next;
} BloodParticleInfo;

// Moteur à particules
typedef struct PARTICLE_ENGINE
{
    Particle_Table* table;
    BloodParticleInfo* blood_part_list;

    sfThread* create_thread;
    sfThread* update_thread;

    bool running;
} ParticleEngine;

void ParticleEngine_Init();
void ParticleEngine_Clean(ParticleEngine* engine);
void ParticleEngine_CreateParticles(void* UserData);
void ParticleEngine_CreateBlood(ParticleEngine* engine, int coord_x, int coord_y, float speed_x, float speed_y);

Particle* particle_Create();
Particle* particle_CreateBlood();

inline void particle_SetPosition(Particle*, int, int);

inline int Particle_GetX(Particle*);
inline int Particle_GetY(Particle*);

inline void Particle_SetX(Particle*, int);
inline void Particle_SetY(Particle*, int);

inline int Particle_GetSizeX(Particle*);
inline int Particle_GetSizeY(Particle*);

inline void Particle_SetSizeX(Particle*, int);
inline void Particle_SetSizeY(Particle*, int);

inline float Particle_GetSpeedX(Particle*);
inline float Particle_GetSpeedY(Particle*);

inline void Particle_SetSpeedX(Particle*, float);
inline void Particle_SetSpeedY(Particle*, float);

void particle_Draw(Particle*, Config*);
void particle_Destroy(Particle*);

Particle_Table* particle_table_Create();
void particle_table_Destroy(Particle_Table*);
void particle_table_AddParticle(Particle_Table*, Particle*);
void particle_table_Draw(sfRenderWindow*, Particle_Table*, Config*);

#endif
