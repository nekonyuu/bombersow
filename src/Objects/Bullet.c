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

#include "BaseSystem/Logging.h"
#include "Objects/GameObjects.h"

Bullet* bullet_Create(unsigned int player_id, unsigned int type)
{
    Bullet* new_bullet = NULL;
    assert(new_bullet = (Bullet*) malloc(sizeof(Bullet)));

    new_bullet->prev = NULL;
    new_bullet->next = NULL;

    new_bullet->owner = player_id;
    new_bullet->bullet_type = type;
    new_bullet->damage = armory[type].damage;
    new_bullet->trajectory = armory[type].trajectory;
    new_bullet->range = armory[type].range;
    new_bullet->draw_image = sprite_Create(0, 0, weap_img[type].bullet_img, NULL);

    new_bullet->quad_node = NULL;
    new_bullet->list_node = NULL;

    new_bullet->gravity = 0;
    new_bullet->acceleration = 0;

    return new_bullet;
}

void bullet_Destroy(Bullet* ptr)
{
    if (!ptr)
        logging_Error("bullet_Destroy", "Bullet object sent NULL", NULL_PTR);

    sprite_Destroy(ptr->draw_image);
    free_secure(ptr);
}

void bullet_DeleteFromList(Bullet* bullet2destroy)
{
    if (!bullet2destroy)
        logging_Error("bullet_DeleteFromList", "Bullet object sent NULL", NULL_PTR);

    bullet2destroy->prev->next = bullet2destroy->next;
    bullet2destroy->next->prev = bullet2destroy->prev;

    sprite_Destroy(bullet2destroy->draw_image);
    free_secure(bullet2destroy);
}

// Destruction d'une liste complète, head vaut NULL après destruction
void bullet_DestroyList(Bullet** head)
{
    if (!head)
        logging_Error("bullet_DestroyList", "Bullet list sent NULL", NULL_PTR);

    Bullet* ptr = *head, *ptr2 = NULL;
    while(ptr != NULL)
    {
        ptr2 = ptr->next;
        bullet_Destroy(ptr);
        ptr = ptr2;
    }

    *head = NULL;
}

void bullet_SetNext(Bullet* ptr, Bullet* new_bullet)
{
    ptr->next = new_bullet;
}

void bullet_SetPrev(Bullet* ptr, Bullet* new_bullet)
{
    ptr->prev = new_bullet;
}

Bullet* bullet_GetNext(Bullet* ptr)
{
    return ptr->next;
}

Bullet* bullet_GetPrev(Bullet* ptr)
{
    return ptr->prev;
}

void bullet_Draw(sfRenderWindow* Game, Bullet* bullet)
{
    sprite_Draw(Game, bullet->draw_image);
}

void bullet_DrawList(sfRenderWindow* Game, Bullet* bullet)
{
    for (Bullet* ptr = bullet; ptr != NULL; ptr = ptr->next)
        sprite_Draw(Game, ptr->draw_image);
}

void bullet_SetPosition(Bullet* bullet, float x, float y)
{
    bullet->coord_x = x;
    bullet->coord_y = y;
    sprite_SetPosition(bullet->draw_image, x, y);
}

void bullet_SetSpeed(Bullet* bullet, float x, float y)
{
    bullet->speed_x = x;
    bullet->speed_y = y;
}


