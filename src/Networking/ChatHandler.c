#include <string.h>
#include "BaseSystem/Logging.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"

sfPacket* chat_CreatePacket(Player* player, const char* message)
{
    if(!player)
    {
        logging_Warning("chat_CreatePacket", "Player object sent NULL, aborting packet creation");
        return NULL;
    }
    if(!message)
    {
        logging_Warning("chat_CreatePacket", "message sent NULL, aborting packet creation");
        return NULL;
    }

    sfPacket* new_packet = sfPacket_Create();
    sfPacket_WriteUint8(new_packet, (sfUint8) CHAT);
    sfPacket_WriteUint8(new_packet, (sfUint8) player->player_id);
    sfPacket_WriteString(new_packet, message);

    return new_packet;
}

// A REVOIR
char* chat_ReadPacket(sfPacket* packet)
{
    char* recv_mess = NULL;
    unsigned int player_id = (unsigned int) sfPacket_ReadUint8(packet);

    sfPacket_ReadString(packet, recv_mess);
    sfPacket_Destroy(packet);
    return recv_mess;
}

void chat_AddChatPacket2Map(Map* map, sfPacket* packet)
{
    assert(map && packet);
    if (map->chat_packets2send)
        assert(map->chat_packets2send->packets = (sfPacket**) realloc(map->chat_packets2send->packets, ++map->chat_packets2send->nb_packets * sizeof(sfPacket*)));
    else
    {
        assert(map->chat_packets2send = (PacketList*) malloc(sizeof(PacketList)));
        assert(map->chat_packets2send->packets = (sfPacket**) malloc(++map->chat_packets2send->nb_packets * sizeof(sfPacket*)));
    }

    map->chat_packets2send->packets[map->chat_packets2send->nb_packets - 1] = packet;
}
