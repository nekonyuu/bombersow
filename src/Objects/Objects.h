#ifndef OBJECT_H
#define OBJECT_H

#include "SFML/Graphics.h"

typedef struct OBJECT
{
    unsigned int type;      // Type d'objet (0 = Plate-forme, 1 = Piège, 2 = Arme, 3 = Ammo)

    sfImage *draw_img;      // Image de l'objet

    float coord_x;          // Coordonnées de l'objet
    float coord_y;

    float dest_coord_x;     // Coordonnées d'arrivée de l'objet (si dynamique)
    float dest_coord_y;

    unsigned int speed;     // Vitesse de mouvement (Plate-forme mobile & Pièges)

    unsigned int weapon_id; // ID de l'arme liée au dessin (si type <= 2)

    unsigned int nb_ammo;   // Nombre de munitions ajoutées par le pack
} Object;

Object* object_Create();
void object_Destroy(Object*);

#endif
