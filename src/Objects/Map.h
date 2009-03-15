#ifndef MAP_H
#define MAP_H

#include "SFML/Network.h"
#include "Objects/Player.h"
#include "Objects/Objects.h"

#define NB_MAX_PLAYERS 4

typedef struct PACKET_LIST
{
    sfPacket** packets;         // Paquets à envoyer
    unsigned int nb_packets;    // Nombre de paquets

} PacketList;

typedef struct MAP
{
    unsigned int mapId;         // ID de la map (transmission réseau)
    sfSprite* background;       // Arrière-plan

    Image* images;              // Images de la map
    Animation** animations;     // Tableau pour les animations
    unsigned int nb_anim;       // Nombre d'animations

    Object** objects_list;      // Tableau des objets de la map
    unsigned int nb_objects;    // Nombre d'objets sur la map

    Player** players_list;      // Liste des joueurs de la map
    unsigned int nb_players;    // Nombre de joueurs connectés sur la map

    Bullet** bullets_list;      // Liste des balles tirées
    unsigned int nb_bullets;    // Nombre de balles tirées

    PacketList* packets2send;   // Liste des paquets a envoyer de la map
    PacketList* packets2use;

    _Bool chat_started;         // Salon de discussion démarré ?
    _Bool game_started;         // Partie démarrée ?

    sfSocketUDP* game_port;     // Socket de jeu

} Map;

Map* map_Create();
void map_Destroy();
void map_AddObject(Map*, Object*);
void map_DelObject(Map*, unsigned int);
void map_AddPlayer(Map*, Player*);
void map_DelPlayer(Map*, unsigned int);
void map_AddBullet(Map*, Bullet*);
void map_DelBullet(Map*, unsigned int);

#endif
