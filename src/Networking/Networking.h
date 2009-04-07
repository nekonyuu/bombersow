#ifndef NETWORKING_H
#define NETWORKING_H

#include "Objects/GameObjects.h"

// Permet de regrouper la map et l'id du joueur pour lequel on écoute
typedef struct CHAT_DATA
{
    Map* map;
    unsigned int player_id;
} ChatData;

_Bool server_started;

// ServerThread.c
void server_Main(Map* map);
void server_Listen_Connections(void* UserData);
void server_Listen_TCP(void* UserData);
void server_Listen_Game(void* UserData);
sfPacket* server_CreateResponsePacket(unsigned int player_id, unsigned int response);
void server_ReadUDPPacket(sfPacket* packet, Map* map);

// ClientThread.c
sfPacket* client_CreateConnectPacket(char* name);
sfPacket* client_CreateDisconnectPacket(unsigned int player_id);

// ChatHandler.c
sfPacket* chat_CreatePacket(Player* player, const char* message);
char* chat_ReadPacket(sfPacket* packet);
ChatData* chat_CreatePlayerData(Map*, unsigned int);

// DataHandler.c
Packet* player_CreatePacket(Player*);
Player* player_CreateFromPacket(sfPacket*);
void player_ReadPacket(Map*, sfPacket*);
Packet* object_CreatePacket(Object*);
void object_ReadPacket(Map*, sfPacket*);
Packet* bullet_CreatePacket(Bullet*);
void bullet_ReadPacket(Map*, sfPacket*);
void map_CreateGamePackets(Map*);
void map_DestroyAllPackets(Map*);

#endif
