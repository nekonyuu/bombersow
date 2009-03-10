#ifndef STRIPPEDOBJECT_H
#define STRIPPEDOBJECT_H

typedef struct STRIPPED_OBJECT      // Envoi des objets dynamiques
{
    unsigned int objectID;          // ID de l'object

    float* start_coord_x;            // Coordonnées de l'objet
    float* start_coord_y;

    float* dest_coord_x;             // Coordonnées d'arrivée de l'objet (si dynamique)
    float* dest_coord_y;

    unsigned int* speed;             // Vitesse de mouvement (Plate-forme mobile & Pièges)
} StObject;

#endif
