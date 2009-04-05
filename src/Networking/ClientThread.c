#include "BaseSystem/Logging.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"

void client_Main(Map* map, sfIPAddress ip, char* name)
{
    sfPacket* connect_request = client_CreateConnectPacket(name);
    sfSocketTCP* client_socket = sfSocketTCP_Create();
    sfPacket *response = NULL;
    unsigned int code = REFUSED;
    sfSocketTCP_Connect(client_socket, map->game_port, ip, 30.0f);
    sfSocketTCP_SendPacket(client_socket, connect_request);
    sfPacket_Destroy(connect_request);

    sfSocketTCP_ReceivePacket(client_socket, response);
    code = (unsigned int) sfPacket_ReadUint8(response);

    if(code != ACCEPTED)
    {

    }
    else
    {

    }
}

sfPacket* client_CreateConnectPacket(char* name)
{
    if (!name)
    {
        logging_Warning("player_CreateConnectPacket", "No name sent, aborting packet creation");
        return NULL;
    }

    sfPacket* new_packet = sfPacket_Create();
    sfPacket_WriteUint8(new_packet, CONNECT);
    sfPacket_WriteString(new_packet, name);

    return new_packet;
}

sfPacket* client_CreateDisconnectPacket(unsigned int player_id)
{
    if (!player_id)
    {
        logging_Warning("player_CreateDisconnectPacket", "No player_id sent, aborting packet creation");
        return NULL;
    }

    sfPacket* new_packet = sfPacket_Create();
    sfPacket_WriteUint8(new_packet, DISCONNECT);
    sfPacket_WriteUint8(new_packet, (sfUint8) player_id);

    return new_packet;
}
