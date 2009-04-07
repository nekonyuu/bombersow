#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <stdbool.h>
#include <SFML/Network.h>
#include "GraphicEngine/Image.h"

#define NB_MAX_PLAYERS 4
#define NB_MAX_WEAPONS 7
#define SHOTGUN_SHRAPNELS 7

typedef enum JUMP_TYPE {NO_JUMP, SIMPLE_JUMP} jump_t;

typedef struct PACKET_LIST
{
    sfPacket** packets;             // Paquets à envoyer
    unsigned int nb_packets;        // Nombre de paquets

} PacketList;

enum { PLATFORM, TRAP, WEAPON, AMMO };

typedef struct OBJECT
{
    unsigned int objectID;          // ID de l'object (transmission réseau)
    unsigned int type;              // Type d'objet (0 = Plate-forme, 1 = Piège, 2 = Arme, 3 = Ammo)

    /*Voir pour inclusion dans une struct*/
    Sprite* sprite;
    /*************************************/

    float start_coord_x;            // Coordonnées de l'objet
    float start_coord_y;

    float curr_coord_x;             // Coordonnées courantes de l'objet (serviront au dessin et seront mises à jour pour le déplacement)
    float curr_coord_y;

    float dest_coord_x;             // Coordonnées d'arrivée de l'objet (si dynamique)
    float dest_coord_y;

    unsigned int speed;             // Vitesse de mouvement (Plate-forme mobile & Pièges)
    sfClock* clock_mouvement;        // Clock pour les mouvements.

    unsigned int weapon_id;         // ID de l'arme liée au dessin (si type <= 2)
    unsigned int nb_ammo;           // Nombre de munitions ajoutées par le pack

    _Bool spawned;                  // Affiché ou pas

    _Bool gravity;

    struct QUAD_TREE* quad_node;
    struct LIST_ELEMENT* list_node;

} Object;

// Liste des armes
enum WEAPON_TYPE { CROWBAR, ROCKET_LAUNCHER, GRENADES, SHOTGUN, MACHINEGUN, SNIPER, LASERGUN };

// Définition de la structure Arme
typedef struct WEAPON
{
    sfString *name;                 // Nom de l'arme
    int type;                       // ID de l'arme

    sfSprite *weapon_img;           // Image de l'arme
    Sprite *bullet_img;             // Projectile

    int nb_max_bullets;             // Nombre de munitions max (-1 si infini)
    int nb_curr_bullets;            // Nombre de munitions restantes (-1 si infini)
    unsigned int damage;            // Dommages infligés par projectile

    unsigned int reload_latency;    // Temps de recharge (en millisecondes)
    unsigned int switch_latency;    // Temps avant possibilité de changement d'arme
    unsigned int respawn_time;      // Temps de réapparition de l'arme

    int range;                      // Portée de l'arme (-1 si infinie)
    unsigned int splash_radius;     // Distance de dommages collatéraux
    float splash_coef;              // Diminution des dommages selon la distance dans le cercle (dommage = (splash_coef) ^ distance par rapport au centre (en pixels) * dommages de base)
    float selfdamage_coef;          // Self-Damage (selfdamage * (splash_coef^distance du centre) * dmg)

    int proj_speed;                 // Vitesse des projectiles (-1 si instantané)
    unsigned int trajectory;        // Type de trajectoire (0 = Rectiligne, 1 = Parabole (Grenade), 2 = Spread (Shotgun)

    _Bool collected;                // Ramassée ? (utilisé seulement pour l'inventaire du joueur)
} Weapon;

typedef struct PLAYER
{
    sfString *name;                 // Nom du joueur
    char* char_name;                // Nom du joueur en char*
    unsigned int player_id;         // ID joueur
    sfIPAddress* player_ip;         // IP joueur
    unsigned int life;              // Vie restante

    Weapon **weapons;               // Armes du joueur
    unsigned int nb_weapons;        // Nombre d'armes
    unsigned int current_weapon;    // Arme courante

    float coord_x, coord_y;         // Emplacement sur la map
    float m_coord_x, m_coord_y;     // Coordonnées souris

    unsigned int speed_x, speed_y;  // Vitesse

    jump_t jump;                    // Type de saut en cours
    _Bool jetpack_mode;             // Mode JetPack ?

    unsigned int frags;             // Nombre de tués
    unsigned int killed;            // Nombre de morts

    sfThread* player_thread;        // Thread du player
    sfSocketTCP* listen_socket;     // Socket d'écoute
    sfBool connected;               // Booléen de présence sur le serveur
    sfBool ready;                   // Prêt à jouer

    Sprite* sprite;

    struct QUAD_TREE* quad_node;
    struct LIST_ELEMENT* list_node;

    _Bool gravity;

} Player;

typedef struct BULLET
{
    unsigned int owner;             // Tireur (utile pour le self-damage)
    unsigned int bullet_type;       // Type de balles
    unsigned int damage;            // Dommages infligés
    unsigned int trajectory;        // Type de trajectoire (0 = Rectiligne, 1 = Parabole (Grenade), 2 = Spread (Shotgun)
    unsigned int range;

    float coord_x;                  // Coordonnées
    float coord_y;

    Sprite* draw_image;           // Image de la balle (Peut être NULL, balle invisible)

    struct QUAD_TREE* quad_node;
    struct LIST_ELEMENT* list_node;

    _Bool gravity;
    float acceleration;

} Bullet;

typedef struct MAP
{
    unsigned int mapId;             // ID de la map (transmission réseau)
    sfSprite* background;           // Arrière-plan

    Image* images;                  // Images de la map
    Animation** animations;         // Tableau pour les animations
    unsigned int nb_anim;           // Nombre d'animations

    Object** objects_list;          // Tableau des objets de la map
    unsigned int nb_objects;        // Nombre d'objets sur la map

    Player** players_list;          // Liste des joueurs de la map
    unsigned int nb_players;        // Nombre de joueurs connectés sur la map

    Bullet** bullets_list;          // Liste des balles tirées
    unsigned int nb_bullets;        // Nombre de balles tirées

    PacketList* game_packets2send;  // Liste des paquets de jeu à envoyer

    _Bool chat_started;             // Salon de discussion démarré ?
    _Bool game_started;             // Partie démarrée ?

    sfSocketUDP* game_socket;       // Socket de jeu
    unsigned short game_port;       // Port de jeu

    sfClock* clock;

} Map;

Weapon armory[NB_MAX_WEAPONS];      // Armes du jeu en accès global
sfMutex* Network_ServerMutex;       // Mutex Serveur

Object* object_Create(unsigned int);
void object_Destroy(Object*);
void object_LoadImg(Object*, sfImage*, Animation*);
void object_Draw(sfRenderWindow*, Object*);
void object_SetPosition(Object*, float, float);

Weapon* weapon_Create(int);
void weapon_Destroy(Weapon*);
void armory_Create(Weapon*);
void armory_Destroy(Weapon*);

Player* player_Create(char*, unsigned int);
void player_Destroy(Player*);
Player* player_GetPlayerFromID(Map*, unsigned int);
void player_Displace(Player*, float, float);
void player_SwitchWeapon(Player*, int);
void player_CollectWeapon(Player*, int);
void player_WeaponShoot(Map*, Player*);
void player_Jump(Player*);

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
void map_UpdateDisconnectedPlayers(void*);

#endif
