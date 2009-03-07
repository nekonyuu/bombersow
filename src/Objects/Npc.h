#ifndef NPC_H
#define NPC_H

typedef struct NPC
{
    char* name;
    unsigned int life;

    unsigned int weapon_id;

    float coord_x;
    float coord_y;

    unsigned int frags;
    unsigned int killed;
} Npc

#endif
