#include <string.h>
#include "BaseSystem/Logging.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"

sfPacket* chat_CreatePacket(Player* player, const char* message)
{
    if (!player)
    {
        logging_Warning("chat_CreatePacket", "Player object sent NULL, aborting packet creation");
        return NULL;
    }
    if (!message)
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

// Lit le paquet et renvoie le message construit
char* chat_ReadPacket(Map* map, sfPacket* packet)
{
    char* text = NULL, *recv_mess = NULL;
    unsigned int player_id = (unsigned int) sfPacket_ReadUint8(packet);

    sfPacket_ReadString(packet, recv_mess);
    sfPacket_Destroy(packet);
    strcpy(text, map->players_list[player_id]->char_name);
    strcat(text, ": ");
    strcat(text, recv_mess);
    return text;
}

// Sert seulement à stocker map et id joueur pour envoi en thread
ChatData* chat_CreatePlayerData(Map* map, unsigned int player_id)
{
    assert(map);

    ChatData* data = NULL;
    assert(data = (ChatData*) malloc(sizeof(ChatData)));

    data->map = map;
    data->player_id = player_id;

    return data;
}

void chat_DestroyPlayerData(ChatData* data)
{
    free(data);
}
