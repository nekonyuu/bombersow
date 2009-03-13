#ifndef PLAYER_H
#define PLAYER_H

#include "Objects/Weapon.h"

typedef enum JUMP_TYPE {NO_JUMP, SIMPLE_JUMP} jump_t;

typedef struct STRIP_PLAYER
{
    char* name;                 // Nom
    sfUint8 current_weapon;      // Arme courante
    float coord_x, coord_y;     // Emplacement sur la map

} stPlayer;

typedef struct PLAYER
{
    sfString *name;                 // Nom du joueur
    unsigned int player_id;         // ID joueur
    unsigned int life;              // Vie restante

    stPlayer* stripped;             // Pointeur vers le player allégé

    Weapon **weapons;               // Armes du joueur
    unsigned int nb_weapons;        // Nombre d'armes
    unsigned int current_weapon;    // Arme courante

    float coord_x, coord_y;         // Emplacement sur la map

    unsigned int speed_x, speed_y;  // Vitesse

    jump_t jump;                    // Type de saut en cours

    unsigned int frags;             // Nombre de tués
    unsigned int killed;            // Nombre de morts
} Player;

Player* player_Create(char*, unsigned int);
void player_Destroy(Player*);
void player_Displace(Player*, float, float);
void player_SwitchWeapon(Player*, int);
void player_CollectWeapon(Player*, int);
Bullet** player_WeaponShoot(Player*, int*);
void player_Jump(Player*);

void stplayer_Create(Player*);
void stplayer_Destroy(stPlayer*);
void stplayer_Update(Player*);

#endif
