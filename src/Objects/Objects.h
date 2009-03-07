#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.h>

typedef struct OBJECT
{
    unsigned int type;      // Type d'objet (0 = Plate-forme, 1 = Piège, 2 = Arme)

    sfImage *draw_img;      // Image de l'objet

    unsigned int move_type; // Type de mouvement (0 = Fixe, 1 = Mouvement Horizontal, 2 = Mouvement Vertical)
    float coord_x;
    float coord_y;
    float move_distance;    // Distance de mouvement (0 si move_type = 0)
    unsigned int speed;     // Vitesse de mouvement (Plate-forme mobile & Pièges)

    unsigned int weapon_id; // ID de l'arme liée au dessin (si type = 2)
} Object;

#endif
