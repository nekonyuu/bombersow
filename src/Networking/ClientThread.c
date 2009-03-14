#include "Networking/Networking.h"
#include "networking/PacketDefines.h"

sfPacket* player_CreateDisconnectPacket(char* name)
{
    sfPacket* new_packet = sfPacket_Create();
    sfPacket_WriteUint8(new_packet, DISCONNECT);
    sfPacket_WriteString(new_packet, name);

    return new_packet;
}
