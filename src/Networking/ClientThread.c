#include "BaseSystem/Logging.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"

void client_Main()
{

}

sfPacket* player_CreateConnectPacket(char* name)
{
    if(!name)
    {
        logging_Warning("player_CreateConnectPacket", "No name sent, aborting packet creation");
        return NULL;
    }

    sfPacket* new_packet = sfPacket_Create();
    sfPacket_WriteUint8(new_packet, CONNECT);
    sfPacket_WriteString(new_packet, name);

    return new_packet;
}

sfPacket* player_CreateDisconnectPacket(char* name)
{
    if(!name)
    {
        logging_Warning("player_CreateDisconnectPacket", "No name sent, aborting packet creation");
        return NULL;
    }

    sfPacket* new_packet = sfPacket_Create();
    sfPacket_WriteUint8(new_packet, DISCONNECT);
    sfPacket_WriteString(new_packet, name);

    return new_packet;
}
