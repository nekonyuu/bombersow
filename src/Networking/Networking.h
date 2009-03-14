#ifndef NETWORKING_H
#define NETWORKING_H

#include "SFML/Network.h"
#include "Objects/Map.h"

typedef struct PACKET_LIST
{
    sfPacket** packets;         // Paquets à envoyer
    unsigned int nb_packets;    // Nombre de paquets

} PacketList;

_Bool started;

void server_Main(Map* map, unsigned int port);

sfPacket* chat_CreatePacket(Player* player, const char* message);
void chat_ReadPacket(sfPacket* packet, char* recv_mess);

sfPacket* stplayer_CreatePacket(Player*);
stPlayer* stplayer_CreateFromPacket(sfPacket*);
sfPacket* stobject_CreatePacket(Object*);
PacketList* map_CreateAllPackets(Map*);
void map_DestroyAllPackets(PacketList*);

sfPacket* player_CreateDisconnectPacket(char* name);

#endif
