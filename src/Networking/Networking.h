#ifndef NETWORKING_H
#define NETWORKING_H

#include "Objects/Map.h"

_Bool started;

void server_Main(Map* map, unsigned int port);
void server_Listen_Game(void* UserData);

sfPacket* chat_CreatePacket(Player* player, const char* message);
void chat_ReadPacket(sfPacket* packet, char* recv_mess);

sfPacket* stplayer_CreatePacket(Player*);
stPlayer* stplayer_CreateFromPacket(sfPacket*);
sfPacket* stobject_CreatePacket(Object*);
void map_CreateAllPackets(Map*);
void map_DestroyAllPackets(Map*);

sfPacket* player_CreateConnectPacket(char* name);
sfPacket* player_CreateDisconnectPacket(char* name);

#endif
