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
    char* mess2send = NULL;
    int size = strlen(player->stripped->name) + strlen(message) + 4;

    assert(mess2send = (char*) malloc(size * sizeof(char)));
    strcpy(mess2send, (const char*) player->stripped->name);
    strcat(mess2send, " : ");
    strcat(mess2send, message);
    mess2send[size - 1] = '\0';
    sfPacket_WriteUint8(new_packet, (sfUint8) CHAT);
    sfPacket_WriteString(new_packet, mess2send);
    free(mess2send);

    return new_packet;
}

void chat_ReadPacket(sfPacket* packet, char* recv_mess)
{
    sfPacket_ReadString(packet, recv_mess);
    sfPacket_Destroy(packet);
}
