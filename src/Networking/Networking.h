#ifndef NETWORKING_H
#define NETWORKING_H

#include "Objects/GameObjects.h"

_Bool started;

void server_Main(Map* map);
void server_Listen_Connections(void* UserData);
void server_Listen_Game(void* UserData);
sfPacket* server_CreateResponsePacket(unsigned int player_id);


sfPacket* client_CreateConnectPacket(char* name);
sfPacket* client_CreateDisconnectPacket(char* name);

sfPacket* chat_CreatePacket(Player* player, const char* message);
void chat_ReadPacket(sfPacket* packet, char* recv_mess);

sfPacket* player_CreatePacket(Player*);
Player* player_CreateFromPacket(sfPacket*);
sfPacket* object_CreatePacket(Object*);
Object* object_CreateFromPacket(sfPacket*);
void map_CreateAllPackets(Map*);
void map_DestroyAllPackets(Map*);

#endif
