#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <stdbool.h>
#include "SFML/Network.h"
#include "GraphicEngine/Image.h"

#define NB_MAX_PLAYERS 4
#define NB_MAX_WEAPONS 7
#define SHOTGUN_SHRAPNELS 7

typedef enum JUMP_TYPE {NO_JUMP, SIMPLE_JUMP} jump_t;

typedef struct PACKET_LIST
{
    sfPacket** packets;             // Paquets � envoyer
    unsigned int nb_packets;        // Nombre de paquets

} PacketList;

typedef struct STRIPPED_OBJECT      // Envoi des objets dynamiques
{
    sfUint8 objectID;               // ID de l'object
    float coord_x, coord_y;         // Coordonn�es courantes
    sfUint8 speed;                  // Vitesse de mouvement (Plate-forme mobile & Pi�ges)
} stObject;

typedef struct OBJECT
{
    unsigned int objectID;          // ID de l'object (transmission r�seau)
    unsigned int type;              // Type d'objet (0 = Plate-forme, 1 = Pi�ge, 2 = Arme, 3 = Ammo)
    stObject* stripped;             // Pointeur vers l'object all�g�

    /*Voir pour inclusion dans une struct*/
    sfSprite *draw_img;             // Image de l'objet
    Animation *draw_animation;
    int bool_animation;
    /*************************************/

    float start_coord_x;            // Coordonn�es de l'objet
    float start_coord_y;

    float curr_coord_x;             // Coordonn�es courantes de l'objet (serviront au dessin et seront mises � jour pour le d�placement)
    float curr_coord_y;

    float dest_coord_x;             // Coordonn�es d'arriv�e de l'objet (si dynamique)
    float dest_coord_y;

    unsigned int speed;             // Vitesse de mouvement (Plate-forme mobile & Pi�ges)

    unsigned int weapon_id;         // ID de l'arme li�e au dessin (si type <= 2)
    unsigned int nb_ammo;           // Nombre de munitions ajout�es par le pack

    _Bool spawned;                  // Affich� ou pas
} Object;

// Liste des armes
enum WEAPON_TYPE { CROWBAR, ROCKET_LAUNCHER, GRENADES, SHOTGUN, MACHINEGUN, SNIPER, LASERGUN };

// D�finition de la structure Arme
typedef struct WEAPON
{
    sfString *name;                 // Nom de l'arme
    int type;                       // ID de l'arme

    sfSprite *weapon_img;           // Image de l'arme
    sfSprite *bullet_img;           // Projectile

    int nb_max_bullets;             // Nombre de munitions max (-1 si infini)
    int nb_curr_bullets;            // Nombre de munitions restantes (-1 si infini)
    unsigned int damage;            // Dommages inflig�s par projectile

    unsigned int reload_latency;    // Temps de recharge (en millisecondes)
    unsigned int switch_latency;    // Temps avant possibilit� de changement d'arme
    unsigned int respawn_time;      // Temps de r�apparition de l'arme

    int range;                      // Port�e de l'arme (-1 si infinie)
    unsigned int splash_radius;     // Distance de dommages collat�raux
    float splash_coef;              // Diminution des dommages selon la distance dans le cercle (dommage = (splash_coef) ^ distance par rapport au centre (en pixels) * dommages de base)
    float selfdamage_coef;          // Self-Damage (selfdamage * (splash_coef^distance du centre) * dmg)

    int proj_speed;                 // Vitesse des projectiles (-1 si instantan�)
    unsigned int trajectory;        // Type de trajectoire (0 = Rectiligne, 1 = Parabole (Grenade), 2 = Spread (Shotgun)

    _Bool collected;                // Ramass�e ? (utilis� seulement pour l'inventaire du joueur)
} Weapon;

typedef struct STRIP_PLAYER
{
    char* name;                     // Nom
    sfUint8 current_weapon;         // Arme courante
    float coord_x, coord_y;         // Emplacement sur la map
    float m_coord_x, m_coord_y;     // Coordonn�es souris
    sfBool connected;               // Connect� ?
    sfBool ready;                   // Pr�t � jouer ?
} stPlayer;

typedef struct PLAYER
{
    sfString *name;                 // Nom du joueur
    unsigned int player_id;         // ID joueur
    sfIPAddress* player_ip;         // IP joueur
    unsigned int life;              // Vie restante

    stPlayer* stripped;             // Pointeur vers le player all�g�

    Weapon **weapons;               // Armes du joueur
    unsigned int nb_weapons;        // Nombre d'armes
    unsigned int current_weapon;    // Arme courante

    float coord_x, coord_y;         // Emplacement sur la map
    float m_coord_x, m_coord_y;     // Coordonn�es souris

    unsigned int speed_x, speed_y;  // Vitesse

    jump_t jump;                    // Type de saut en cours
    _Bool jetpack_mode;             // Mode JetPack ?

    unsigned int frags;             // Nombre de tu�s
    unsigned int killed;            // Nombre de morts

    sfThread* player_thread;        // Thread du player (d'�coute si serveur, de transmission si client)
    sfSocketTCP* chat_socket;       // Socket d'�coute de discussion
    sfBool connected;               // Bool�en de pr�sence sur le serveur
    sfBool ready;                   // Pr�t � jouer
} Player;

typedef struct BULLET
{
    unsigned int owner;             // Tireur (utile pour le self-damage)
    unsigned int bullet_type;       // Type de balles
    unsigned int damage;            // Dommages inflig�s
    unsigned int trajectory;        // Type de trajectoire (0 = Rectiligne, 1 = Parabole (Grenade), 2 = Spread (Shotgun)
    sfSprite* draw_image;           // Image de la balle (Peut �tre NULL, balle invisible)
} Bullet;

typedef struct MAP
{
    unsigned int mapId;             // ID de la map (transmission r�seau)
    sfSprite* background;           // Arri�re-plan

    Image* images;                  // Images de la map
    Animation** animations;         // Tableau pour les animations
    unsigned int nb_anim;           // Nombre d'animations

    Object** objects_list;          // Tableau des objets de la map
    unsigned int nb_objects;        // Nombre d'objets sur la map

    Player** players_list;          // Liste des joueurs de la map
    unsigned int nb_players;        // Nombre de joueurs connect�s sur la map

    Bullet** bullets_list;          // Liste des balles tir�es
    unsigned int nb_bullets;        // Nombre de balles tir�es

    PacketList* packets2send;       // Liste des paquets a envoyer de la map
    PacketList* packets2use;

    _Bool chat_started;             // Salon de discussion d�marr� ?
    _Bool game_started;             // Partie d�marr�e ?

    sfSocketUDP* game_socket;       // Socket de jeu
    unsigned short game_port;       // Port de jeu

} Map;

Weapon armory[NB_MAX_WEAPONS];      // Armes du jeu en acc�s global

Object* object_Create();
void object_Destroy(Object*);
void stobject_Create(Object*);
void stobject_Destroy(stObject*);
void stobject_Update(Object*);

Weapon* weapon_Create(int);
void weapon_Destroy(Weapon*);
void armory_Create(Weapon*);
void armory_Destroy(Weapon*);

Player* player_Create(char*, unsigned int);
void player_Destroy(Player*);
void player_Displace(Player*, float, float);
void player_SwitchWeapon(Player*, int);
void player_CollectWeapon(Player*, int);
void player_WeaponShoot(Map*, Player*);
void player_Jump(Player*);
void stplayer_Create(Player*);
void stplayer_Destroy(stPlayer*);
void stplayer_Update(Player*);

Bullet* bullet_Create(unsigned int, unsigned int);
void bullet_Destroy(Bullet*);

Map* map_Create();
void map_Destroy();
void map_AddObject(Map*, Object*);
void map_DelObject(Map*, unsigned int);
void map_AddPlayer(Map*, Player*);
void map_DelPlayer(Map*, unsigned int);
void map_AddBullet(Map*, Bullet*);
void map_DelBullet(Map*, unsigned int);


#endif