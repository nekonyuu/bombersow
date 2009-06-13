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

#include <stdio.h>
#include "BaseSystem/Logging.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/CollisionSystem.h"
#include "Objects/GameObjects.h"

Collision* collision_Create()
{

    Collision* collision = NULL;
    assert(collision = (Collision*)malloc(sizeof(Collision)));

    collision->type = 0;
    collision->object = NULL;
    collision->player = NULL;
    collision->bullet = NULL;

    return collision;
}

void collision_Destroy(Collision* collision)
{

    free(collision);
    collision = NULL;

}

/*
    Parcoure l'arbre dans le sens remontant vers la racine
    et descendant vers les dernières feuilles sous le noeud de l'object envoyé
*/
Collision* collision_Detection_Object(void* obj_, int type)
{
    QuadTree* node = NULL;
    QuadTree* node_obj = NULL;
    // Rectangle représentant la zone de l'objet envoyé
    sfIntRect rect_obj;

    // Cast de l'objet au bon type et création de son rectangle de zone
    if (type == OBJECT)
    {
        Object* obj = obj_;
        rect_obj = sprite_GetRect(obj->sprite);
        node = obj->quad_node;
    }
    else if (type == PLAYER)
    {
        Player* obj = obj_;
        rect_obj = sprite_GetRect(obj->sprite);
        node = obj->quad_node;
    }
    else
    {
        Bullet* obj = obj_;
        rect_obj = sprite_GetRect(obj->draw_image);
        node = obj->quad_node;
    }

    node_obj = node;

    List_element* list_temp = NULL;

    Object* object_temp = NULL;
    Player* player_temp = NULL;
    Bullet* bullet_temp = NULL;

    Collision* collision = collision_Create();

    // Parcours de l'arbre en remontant vers la racine pour vérifier si l'objet donné est en collision
    while (node != NULL)
    {
        // On récupère la liste des objects de la section de l'écran représentée par le node
        list_temp = (node->object != NULL) ? node->object->first : NULL;
        // Si la liste des objets n'est pas nulle et tant qu'on a pas parcouru toute la liste
        while (list_temp != NULL && list_temp->elt != NULL)
        {
            // Récupération de l'objet
            object_temp = list_temp->elt;
            // Création du rectangle
            sfIntRect rect_obj2 = sprite_GetRect(object_temp->sprite);
            // Détection de la collision par croisement de deux rectangles
            if ( object_temp != obj_ && sfIntRect_Intersects(&rect_obj, &rect_obj2, NULL) )
            {
                collision->type = OBJECT;
                collision->object = object_temp;
                return collision;
            }
            // S'il n'y a pas eu de collision avec l'object courant, on passe au suivant
            list_temp = list_temp->next;
        }

        // On récupère la liste des bullets de la section
        list_temp = (node->bullet != NULL) ? node->bullet->first : NULL;
        // Même principe
        while (list_temp != NULL && list_temp->elt != NULL)
        {
            bullet_temp = list_temp->elt;
            sfIntRect rect_obj2 = sprite_GetRect(bullet_temp->draw_image);
            if ( bullet_temp != obj_ && sfIntRect_Intersects(&rect_obj, &rect_obj2, NULL) )
            {
                collision->type = BULLET;
                collision->bullet = bullet_temp;
                return collision;
            }

            list_temp = list_temp->next;
        }

        // On réeffectue l'algo pour les players
        list_temp = (node->player != NULL) ? node->player->first : NULL;
        while (list_temp != NULL && list_temp->elt != NULL)
        {
            player_temp = list_temp->elt;
            sfIntRect rect_obj2 = sprite_GetRect(player_temp->sprite);
            if ( player_temp != obj_  && sfIntRect_Intersects(&rect_obj, &rect_obj2, NULL) )
            {
                collision->type = PLAYER;
                collision->player = player_temp;
                return collision;
            }

            list_temp = list_temp->next;
        }

        // On passe au noeud parent
        node = node->parent;
    }

    // Détection de collision sur les feuilles du noeud de l'objet envoyé
    for (int i = 0; i < 4; i++)
    {
        collision_Detection_ObjectArb(obj_, type, node_obj->noeuds[i], collision);
    }

    // Si on a une collision on retourne la collision

    if ( collision->type != OBJECT /* Inhérent à l'initialisation de collision, on ne retourne pas une collision vide */
            || collision->object != NULL )
        return collision;

    //Detruit la struct si on n'a pas de collision
    collision_Destroy(collision);
    return NULL;
}

/*
    Parcoure l'arbre dans le sens descendant vers
    les dernières feuilles sous le noeud de l'object envoyé seulement
*/
void collision_Detection_ObjectArb(void* obj_, int type, QuadTree* node, Collision* collision)
{
    // Détermine si on n'a pas deja trouvé une collision, si non on effectue une recherche descendante
    if ( (collision->type == OBJECT && collision->object == NULL) && node != NULL)
    {
        // Rectangle représentant la zone de l'objet envoyé
        sfIntRect rect_obj;

        // Cast de l'objet au bon type et création de son rectangle de zone
        if (type == OBJECT)
        {
            Object* obj = obj_;
            rect_obj = sprite_GetRect(obj->sprite);
        }
        else if (type == PLAYER)
        {
            Player* obj = obj_;
            rect_obj = sprite_GetRect(obj->sprite);
        }
        else
        {
            Bullet* obj = obj_;
            rect_obj = sprite_GetRect(obj->draw_image);
        }

        List_element* list_temp = NULL;

        Object* object_temp = NULL;
        Player* player_temp = NULL;
        Bullet* bullet_temp = NULL;

        // Parcours de l'arbre en remontant vers la racine pour vérifier si l'objet donné est en collision
        list_temp = (node->object != NULL) ? node->object->first : NULL;
        while (list_temp != NULL && list_temp->elt != NULL)
        {
            // Récupération de l'objet
            object_temp = list_temp->elt;
            // Création du rectangle
            sfIntRect rect_obj2 = sprite_GetRect(object_temp->sprite);
            // Détection de la collision par croisement de deux rectangles
            if ( object_temp != obj_ && sfIntRect_Intersects(&rect_obj, &rect_obj2, NULL) )
            {
                collision->type = OBJECT;
                collision->object = object_temp;
                return;
            }
            // S'il n'y a pas eu de collision avec l'object courant, on passe au suivant
            list_temp = list_temp->next;
        }

        // Récupere le premier element de la liste de Bullet
        list_temp = (node->bullet != NULL) ? node->bullet->first : NULL;
        // Même principe que la boucle précédente
        while (list_temp != NULL && list_temp->elt != NULL)
        {
            bullet_temp = list_temp->elt;
            sfIntRect rect_obj2 = sprite_GetRect(bullet_temp->draw_image);
            if ( bullet_temp != obj_ && sfIntRect_Intersects(&rect_obj, &rect_obj2, NULL) )
            {
                collision->type = BULLET;
                collision->bullet = bullet_temp;
                return;
            }

            list_temp = list_temp->next;
        }

        // Récupere le premier element de la liste de Player
        list_temp = (node->player != NULL) ? node->player->first : NULL;
        // Même principe que la boucle précédente
        while (list_temp != NULL && list_temp->elt != NULL)
        {
            player_temp = list_temp->elt;
            sfIntRect rect_obj2 = sprite_GetRect(player_temp->sprite);
            if ( player_temp != obj_  && sfIntRect_Intersects(&rect_obj, &rect_obj2, NULL) )
            {
                collision->type = PLAYER;
                collision->player = player_temp;
                return;
            }

            list_temp = list_temp->next;
        }

        // Parcours les noeuds fils pour detecter les collisions
        for (int i = 0; i < 4; i++)
            collision_Detection_ObjectArb(obj_, type, node->noeuds[i], collision);

    }
    return;
}

