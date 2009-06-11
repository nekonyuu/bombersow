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

#include "BaseSystem/Logging.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/GravitySystem.h"
#include "PhysicsEngine/CollisionSystem.h"
#include "PhysicsEngine/ParticleSystem.h"
#include "Objects/GameObjects.h"
#include "GraphicEngine/Draw.h"

static const int max_particles = 10000;

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

void particle_SetPosition(Particle* particle, float x, float y)
{
    particle->x = x;
    particle->y = y;
}

void particle_Draw(sfRenderWindow* Game, Particle* particle, Config* config)
{
    //sfRenderWindow_DrawShape(Game, particle->shape);

    glColor3f(particle->particle_color.r, particle->particle_color.g, particle->particle_color.b);
    // Repère à origine différente
    glBegin(GL_QUADS);
        glVertex2d(particle->x, config->height - particle->y);
        glVertex2d(particle->x + particle->size_x, config->height - particle->y);
        glVertex2d(particle->x + particle->size_x, config->height - particle->y + particle->size_y);
        glVertex2d(particle->x, config->height - particle->y + particle->size_y);
    glEnd();

}

void particle_Destroy(Particle* particle)
{
    if (particle != NULL)
        free_secure(particle);
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

    //particle_table->clock = sfClock_Create();

    return particle_table;

}

void particle_table_Destroy(Particle_Table* particle_table)
{

    for (int i = 0; i < particle_table->nbr_max; i++)
        particle_Destroy(particle_table->particle[i]);

    //sfClock_Destroy(particle_table->clock);
    free_secure(particle_table->particle);
    free_secure(particle_table);

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
        particle_Draw(Game, particle_table->particle[i], config);
}
