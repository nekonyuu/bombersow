#ifndef DATACONSTRUCTOR_H
#define DATACONSTRUCTOR_H

#include "SFML/Network.h"
#include "Objects/Map.h"

enum {PLAYER, OBJECT};

typedef struct PACKET_LIST
{
    sfPacket** packets;         // Paquets à envoyer
    unsigned int nb_packets;    // Nombre de paquets

} PacketList;

sfPacket* stplayer_CreatePacket(Player*);
stPlayer* stplayer_CreateFromPacket(sfPacket*);
sfPacket* stobject_CreatePacket(Object*);
PacketList* map_CreateAllPackets(Map*);
void map_DestroyAllPackets(PacketList*);

#endif
