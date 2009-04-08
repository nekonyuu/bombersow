#include "BaseSystem/Logging.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"

void client_Main(char* name, sfIPAddress ip, int port)
{
    Map* map = NULL;
    sfPacket* connect_request = client_CreateConnectPacket(name), *response = NULL;
    sfSocketTCP* client_socket = sfSocketTCP_Create();
    unsigned int code = REFUSED;
    if (sfSocketTCP_Connect(client_socket, port, ip, 30.0f))
    {
        sfSocketTCP_SendPacket(client_socket, connect_request);
        sfPacket_Destroy(connect_request);

        sfSocketTCP_ReceivePacket(client_socket, response);

        code = (unsigned int) sfPacket_ReadUint8(response);

        if (code == ACCEPTED)
        {
            unsigned int map_id = (unsigned int) sfPacket_ReadUint8(response);
            unsigned int max_players = (unsigned int) sfPacket_ReadUint8(response);
            unsigned int curr_players = (unsigned int) sfPacket_ReadUint8(response);

            sfPacket_Clear(response);
            map = map_Create(map_id, max_players);
            map->game_port = (unsigned short) port;

            for (int i = 0; i < curr_players; i++)
            {
                unsigned int code = 0;
                sfSocketTCP_ReceivePacket(client_socket, response);
                code = sfPacket_ReadUint8(response);
                if (code == PLAYER)
                    map_AddPlayer(map, player_CreateFromPacket(map, response));

                sfPacket_Clear(response);
            }

            // Ecran de lobby
        }
    }
    else
    {
        // Serveur Down ou mauvaise ip
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

    return new_packet;
}
