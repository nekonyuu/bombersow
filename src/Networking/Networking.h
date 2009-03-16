#ifndef NETWORKING_H
#define NETWORKING_H

#include "Objects/GameObjects.h"

_Bool server_started;

// ServerThread.c
void server_Main(Map* map);
void server_Listen_Connections(void* UserData);
void server_Listen_Chat(void* UserData);
void server_Listen_Game(void* UserData);
sfPacket* server_CreateResponsePacket(unsigned int player_id, unsigned int response);

// ClientThread.c
sfPacket* client_CreateConnectPacket(char* name);
sfPacket* client_CreateDisconnectPacket(unsigned int player_id);

// ChatHandler.c
sfPacket* chat_CreatePacket(Player* player, const char* message);
char* chat_ReadPacket(sfPacket* packet);
void chat_AddChatPacket2Map(Map* map, sfPacket* packet);

// DataHandler.c
sfPacket* player_CreatePacket(Player*);
Player* player_CreateFromPacket(sfPacket*);
sfPacket* object_CreatePacket(Object*);
Object* object_CreateFromPacket(sfPacket*);
void map_CreateClientPackets(Map*);
void map_DestroyAllPackets(Map*);

#endif
