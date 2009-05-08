/*
    GPL v3 Licence :
    Bombersow is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bombersow is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bombersow.  If not, see <http://www.gnu.org/licenses/>.


    Creative Commons BY-NC-SA :
    This work is licensed under the Creative Commons Attribution-Noncommercial-Share Alike 3.0 Unported License.
    To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter
    to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.

*/

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
