#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>
#include "SFML/Graphics.h"
#include "GraphicEngine/Image.h"

typedef struct STRIPPED_OBJECT  // Envoi des objets dynamiques
{
    unsigned int* objectID;      // ID de l'object

    float* start_coord_x;       // Coordonnées de l'objet
    float* start_coord_y;

    float* dest_coord_x;        // Coordonnées d'arrivée de l'objet (si dynamique)
    float* dest_coord_y;

    unsigned int* speed;        // Vitesse de mouvement (Plate-forme mobile & Pièges)
} stObject;

typedef struct OBJECT
{
    unsigned int objectID;      // ID de l'object (transmission réseau)
    unsigned int type;          // Type d'objet (0 = Plate-forme, 1 = Piège, 2 = Arme, 3 = Ammo)
    stObject* stripped;         // Pointeur vers l'object allégé

    /*Voir pour inclusion dans une struct*/
    sfSprite *draw_img;         // Image de l'objet
    Animation *draw_animation;
    int bool_animation;
    /*************************************/

    float start_coord_x;        // Coordonnées de l'objet
    float start_coord_y;

    float curr_coord_x;         // Coordonnées courantes de l'objet (serviront au dessin et seront mises à jour pour le déplacement)
    float curr_coord_y;

    float dest_coord_x;         // Coordonnées d'arrivée de l'objet (si dynamique)
    float dest_coord_y;

    unsigned int speed;         // Vitesse de mouvement (Plate-forme mobile & Pièges)

    unsigned int weapon_id;     // ID de l'arme liée au dessin (si type <= 2)
    unsigned int nb_ammo;       // Nombre de munitions ajoutées par le pack

    _Bool spawned;              // Affiché ou pas
} Object;

Object* object_Create();
void object_Destroy(Object*);

stObject* stobject_Create(Object*);
void stobject_Destroy(stObject*);

#endif
