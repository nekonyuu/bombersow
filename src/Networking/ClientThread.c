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

void client_Main(void* UserData)
{
    sfMutex_Lock(server_creation);

    logging_Info("client_Main", "Client thread start...");

    ClientData* client_data = (ClientData*) UserData;

    sfSocketTCP* client_socket = sfSocketTCP_Create();
    unsigned int code = REFUSED;

    if (!sfSocketTCP_Connect(client_socket, client_data->port, client_data->ip, 0))     // Bug CSFML 1.4, sfSocketTCP_Connect retourne sfFalse au lieu de sfTrue et vice versa
    {
        sfPacket* connect_request = client_CreateConnectPacket(client_data->name), *response = sfPacket_Create();
        logging_Info("client_Main", "Sending connect resquest");
        sfSocketTCP_SendPacket(client_socket, connect_request);
        sfPacket_Destroy(connect_request);

        logging_Info("client_Main", "Receive response");
        sfSocketTCP_ReceivePacket(client_socket, response);

        code = (unsigned int) sfPacket_ReadUint8(response);

        if (code == ACCEPTED)
        {
            logging_Info("client_Main", "Connection accepted, reading server info");

            unsigned int map_id = (unsigned int) sfPacket_ReadUint8(response);
            unsigned int max_players = (unsigned int) sfPacket_ReadUint8(response);
            unsigned int curr_players = (unsigned int) sfPacket_ReadUint8(response);
            unsigned int cpt_players_rev = (unsigned int) sfPacket_ReadUint8(response);

            sfPacket_Clear(response);
            logging_Info("client_Main", "Creating map...");
            client_data->map = map_Create(map_id, max_players, client_data->config);
            map_SetGamePort(client_data->map, (unsigned short) client_data->port);
            map_SetCptCurrPlayers(client_data->map, cpt_players_rev);

            sfSelectorTCP_Add(client_data->map->tcp_selector, client_socket);

            logging_Info("client_Main", "Adding players");
            for (int i = 0; i < curr_players; i++)
            {
                unsigned int code = 0;
                sfSocketTCP_ReceivePacket(client_socket, response);
                code = sfPacket_ReadUint8(response);
                if (code == PLAYER)
                    map_AddPlayer(client_data->map, player_CreateFromPacket(client_data->map, response));

                sfPacket_Clear(response);
            }
            logging_Info("client_Main", "Client connected and ready !");
            client_connected = true;
            sfMutex_Unlock(server_creation);

            while (client_connected)
            {
                unsigned int code = 0;
                unsigned int nb_sck_ready = sfSelectorTCP_Wait(client_data->map->tcp_selector, 0.1f);

                if (nb_sck_ready > 0)
                {
                    for (int i = 0; i < nb_sck_ready; i++)
                    {
                        sfSocketTCP* new_socket = sfSelectorTCP_GetSocketReady(client_data->map->tcp_selector, i);
                        sfSocketTCP_ReceivePacket(new_socket, response);
                        code = sfPacket_ReadUint8(response);
                        switch(code)
                        {
                            case PLAYER:
                                map_AddPlayer(client_data->map, player_CreateFromPacket(client_data->map, response));
                                break;
                            case DISCONNECT:
                                map_DelPlayer(client_data->map, (unsigned int) sfPacket_ReadUint8(response));
                                break;
                            case SERVER_CLOSING:
                                client_connected = false;
                                client_data->server_close = true;
                                break;
                            default:

                                break;
                        }

                        sfPacket_Clear(response);
                    }
                }
            }

            logging_Info("client_Main", "Disconnecting...");
            sfPacket_Destroy(response);
            if(!client_data->server_close)
            {
                response = client_CreateDisconnectPacket(map_GetPlayerIDFromName(client_data->map, client_data->name));
                sfSocketTCP_SendPacket(client_socket, response);
            }
            sfSelectorTCP_Remove(client_data->map->tcp_selector, client_socket);

            logging_Info("client_Main", "Cleaning resources...");
            map_Destroy(client_data->map);
        }
        else if (code == REFUSED)
        {
            sfMutex_Unlock(server_creation);
            logging_Info("client_Main", "Server has refused connection");
        }

        sfPacket_Destroy(response);
    }
    else
    {
        // Serveur Down ou mauvaise ip
        sfMutex_Unlock(server_creation);
        logging_Info("client_Main", "Can't reach server...");
    }

    sfSocketTCP_Destroy(client_socket);
    logging_Info("client_Main", "End Client thread");
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
    sfPacket_WriteUint8(new_packet, player_id);

    return new_packet;
}
