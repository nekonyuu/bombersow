#ifndef OBJECT_H
#define OBJECT_H

#include "SFML/Graphics.h"
#include "GraphicEngine/Image.h"

typedef struct OBJECT
{
    unsigned int type;      // Type d'objet (0 = Plate-forme, 1 = Pi�ge, 2 = Arme, 3 = Ammo)

    /*Voir pour inclusion dans une struct*/
    sfSprite *draw_img;      // Image de l'objet
    Animation *draw_animation;
    int bool_animation;
    /*************************************/

    float start_coord_x;    // Coordonn�es de l'objet
    float start_coord_y;

    float curr_coord_x;     // Coordonn�es courantes de l'objet (serviront au dessin et seront mises � jour pour le d�placement)
    float curr_coord_y;

    float dest_coord_x;     // Coordonn�es d'arriv�e de l'objet (si dynamique)
    float dest_coord_y;

    unsigned int speed;     // Vitesse de mouvement (Plate-forme mobile & Pi�ges)

    unsigned int weapon_id; // ID de l'arme li�e au dessin (si type <= 2)

    unsigned int nb_ammo;   // Nombre de munitions ajout�es par le pack
} Object;

Object* object_Create();
void object_Destroy(Object*);

#endif
