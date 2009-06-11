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
    float x, y;                 // Coordonnées
    int size_x, size_y;         // Taille
    float speed_x, speed_y;     // Vitesse
} Particle;

typedef struct PARTICLE_TABLE
{
    int nbr_particle;       //nombre actuel de particule
    int nbr_max;            //nombre maximum de particule
    int indice_courant;     //indice actuel du tableau
    Particle** particle;    //tableau de particule
}Particle_Table;


Particle* particle_Create();
Particle* particle_CreateBlood();
void particle_SetPosition(Particle*, float, float);
void particle_Draw(sfRenderWindow*, Particle*, Config*);
void particle_Destroy(Particle*);

Particle_Table* particle_table_Create();
void particle_table_Destroy(Particle_Table*);
void particle_table_AddParticle(Particle_Table*, Particle*);
void particle_table_Draw(sfRenderWindow*, Particle_Table*, Config*);

/*typedef struct PARTICLE_ENGINE
{

}Particle_Engine;*/

#endif
