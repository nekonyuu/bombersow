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

#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <stdbool.h>
#include <SFML/Network.h>
#include <BaseSystem/Config.h>
#include <GraphicEngine/Image.h>

#define NB_MAX_WEAPONS 7
#define SHOTGUN_SHRAPNELS 7

typedef enum JUMP_TYPE {FALLING, NO_JUMP, SIMPLE_JUMP, DOUBLE_JUMP} jump_t;
typedef enum DIRECTION {UP, DOWN, LEFT, RIGHT} Direction;

// Structure stockant le paquet + son type
typedef struct PACKET
{
    unsigned int code;
    sfPacket* packet;
} Packet;

// Structure stockant un tableau de paquets
typedef struct PACKET_LIST
{
    Packet** packets;               // Paquets à envoyer
    unsigned int nb_packets;        // Nombre de paquets

} PacketList;

typedef struct WEAPON_IMG
{
    sfImage* weapon_img;
    sfImage* bullet_img;
} WeaponImg;

// Structure contenant la liste des joueurs connectés pour le salon
typedef struct PLAYERS_LIST
{
    sfString** players;             // Nom des joueurs
    unsigned int max_players;       // Taille du tableau
    unsigned int nb_players;        // Nombre de joueurs
} PlayersList;

enum { PLATFORM, PLATFORM_DYNA, TRAP, WEAPON, AMMO };

typedef struct OBJECT
{
    unsigned int objectID;          // ID de l'object (transmission réseau)
    unsigned int type;              // Type d'objet (0 = Plate-forme fixe, 1 = Plate-forme dyna, 2 = Piège, 3 = Arme, 4 = Ammo)

    Sprite* sprite;                 // Sprite de l'objet

    float start_coord_x;            // Coordonnées de l'objet
    float start_coord_y;

    float curr_coord_x;             // Coordonnées courantes de l'objet (serviront au dessin et seront mises à jour pour le déplacement)
    float curr_coord_y;

    float dest_coord_x;             // Coordonnées d'arrivée de l'objet (si dynamique)
    float dest_coord_y;

    unsigned int speed;             // Vitesse de mouvement (Plate-forme mobile & Pièges)
    sfClock* clock_mouvement;       // Clock pour les mouvements.

    unsigned int weapon_id;         // ID de l'arme liée au dessin (si type <= 2)
    unsigned int nb_ammo;           // Nombre de munitions ajoutées par le pack

    sfBool spawned;                 // Affiché ou pas

    bool gravity;                  // Subit la gravité ?

    struct QUAD_TREE* quad_node;    // Noeud du struct QUAD_TREE de la map qui contient l'objet
    struct LIST_ELEMENT* list_node;

} Object;

// Liste des armes
enum WEAPON_TYPE { CROWBAR, ROCKET_LAUNCHER, GRENADES, SHOTGUN, MACHINEGUN, SNIPER, LASERGUN };

// Définition de la structure Arme
typedef struct WEAPON
{
    char* name;                     // Nom de l'arme
    int type;                       // ID de l'arme

    sfSprite *weapon_img;           // Image de l'arme

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

    bool collected;                // Ramassée ? (utilisé seulement pour l'inventaire du joueur)
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

    float speed_x, speed_y;         // Vitesse

    jump_t jump;                    // Type de saut en cours
    bool jetpack_mode;             // Mode JetPack ?

    unsigned int frags;             // Nombre de tués
    unsigned int killed;            // Nombre de morts

    sfSocketTCP* listen_socket;     // Socket d'écoute
    sfBool connected;               // Booléen de présence sur le serveur
    sfBool ready;                   // Prêt à jouer

    Sprite* sprite;

    struct QUAD_TREE* quad_node;    // Noeud du struct QUAD_TREE de la map qui contient le player
    struct LIST_ELEMENT* list_node;

    bool gravity;

} Player;

/*
// Conteneur pour la liste de players
typedef struct PLAYERS_LIST
{
    Player* head;
    Player* tail;
    unsigned int nb_players;
} PlayerList;
*/

typedef struct BULLET
{
    struct BULLET* prev;            // Liste D-Chain
    struct BULLET* next;

    unsigned int owner;             // Tireur (utile pour le self-damage)
    unsigned int bullet_type;       // Type de balles
    unsigned int damage;            // Dommages infligés
    unsigned int trajectory;        // Type de trajectoire (0 = Rectiligne, 1 = Parabole (Grenade), 2 = Spread (Shotgun)
    unsigned int range;

    float coord_x;                  // Coordonnées
    float coord_y;

    float speed_x;
    float speed_y;

    Sprite* draw_image;             // Image de la balle (Si Balle invisible, Sprite Transparent)

    struct QUAD_TREE* quad_node;    // Noeud du struct QUAD_TREE de la map qui contient le bullet
    struct LIST_ELEMENT* list_node;

    bool gravity;
    float acceleration;

} Bullet;

// Conteneur pour la liste de bullets
typedef struct BULLET_LIST
{
    Bullet* head;
    Bullet* tail;
    unsigned int nb_bullets;
} BulletList;

typedef struct MAP
{
    unsigned int mapId;             // ID de la map (transmission réseau)
    unsigned int max_players;       // Nombre maxi de joueurs
    unsigned int cpt_players_rev;   // Compteur courant pour les player_id
    sfSprite* background;           // Arrière-plan

    Image* images;                  // Images de la map
    Animation** animations;         // Tableau pour les animations
    unsigned int nb_anim;           // Nombre d'animations

    Object** objects_list;          // Tableau des objets de la map
    unsigned int nb_objects;        // Nombre d'objets sur la map

    Player** players_list;          // Liste des joueurs de la map
    unsigned int nb_players;        // Nombre de joueurs connectés sur la map

    BulletList* bullets;            // Liste des balles tirées

    PacketList* game_packets2send;  // Liste des paquets de jeu à envoyer

    bool chat_started;              // Salon de discussion démarré ?
    bool game_started;              // Partie démarrée ?

    sfSelectorTCP* tcp_selector;    // Selecteur de sockets TCP pour discussion/connexion/déconnexion
    sfSocketUDP* game_socket;       // Socket de jeu
    unsigned short game_port;       // Port de jeu

    sfClock* clock;                 // Timer d'actualisation
    float clock_time;               // Temps clock

    struct QUAD_TREE* quad_tree;    // struct QUAD_TREE pour la gestion de collisions

} Map;

// Variables Globales
WeaponImg weap_img[NB_MAX_WEAPONS];     // Images des armes
Weapon armory[NB_MAX_WEAPONS];          // Armes du jeu en accès global
sfMutex* Players_CoordUpdate;           // Mutex MàJ Players
sfMutex* Network_ServerMutex;           // Mutex Serveur

// Objects.c
Object* object_Create(unsigned int);
void object_Destroy(Object*);
void object_LoadImg(Object*, sfImage*, Animation*);
void object_Draw(sfRenderWindow*, Object*);
void object_SetPosition(Object*, float, float);

// Weapon.c
Weapon* weapon_Create(int);
void weapon_Destroy(Weapon*);
void armory_LoadImages(WeaponImg* img_tab);
void armory_DestroyImages(WeaponImg* img_tab);
void armory_Create(Weapon*);
void armory_Destroy(Weapon*);

// Player.c
Player* player_Create(char*, unsigned int);
void player_Destroy(Player*);
void player_Displace(Player*, Direction, float, Config*);
void player_SwitchWeapon(Player*, int);
void player_CollectWeapon(Player*, int);
void player_WeaponShoot(Map*, Player*, float, float);
void player_SetPosition(Player*, float, float);
void player_Draw(sfRenderWindow*, Player*);

// Bullet.c
Bullet* bullet_Create(unsigned int, unsigned int);
void bullet_Destroy(Bullet*);
void bullet_DestroyList(Bullet**);
void bullet_DeleteFromList(Bullet*);
void bullet_SetNext(Bullet*, Bullet*);
void bullet_SetPrev(Bullet*, Bullet*);
Bullet* bullet_GetNext(Bullet*);
Bullet* bullet_GetPrev(Bullet*);
void bullet_Draw(sfRenderWindow*, Bullet*);
void bullet_DrawList(sfRenderWindow*, Bullet*);
void bullet_SetPosition(Bullet*, float, float);
void bullet_SetSpeed(Bullet*, float, float);
BulletList* BulletList_Create();
void BulletList_Destroy(BulletList* ptr);
void BulletList_AddBullet(BulletList* ptr, Bullet* ptr2);
void BulletList_DeleteBullet(BulletList* ptr, Bullet* ptr2);
Bullet* BulletList_GetHead(BulletList* ptr);
Bullet* BulletList_GetTail(BulletList* ptr);
unsigned int BulletList_GetNbBullets(BulletList* ptr);

// Map.c
Map* map_Create(unsigned int, unsigned int, Config*);
void map_Destroy();
void map_AddObject(Map*, Object*);
void map_DelObject(Map*, unsigned int);
void map_AddPlayer(Map*, Player*);
void map_DelPlayer(Map*, unsigned int);
void map_AddBullet(Map*, Bullet*);
void map_DelBullet(Map*, Bullet*);
void map_UpdateDisconnectedPlayers(void*);
Player* map_GetPlayerFromID(Map*, unsigned int);
unsigned int map_GetPlayerIDFromName(Map*, char*);
void map_SetGamePort(Map*, unsigned int);
void map_SetCptCurrPlayers(Map*, unsigned int);
void map_Draw(sfRenderWindow*, Map*);

// PlayerControl.c
void control_PlayerControl(sfRenderWindow*, Map*, Player*, Config*, bool*);

// PlayersList.c
PlayersList* playerslist_Create(Map*, sfFont*, sfColor, int, sfStringStyle, float, float);
void playerslist_Destroy(PlayersList*);
void playerslist_Draw(PlayersList*, sfRenderWindow*);
void playerslist_Update(PlayersList*, Map*);

#endif
