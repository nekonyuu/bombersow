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
#include "GraphicEngine/Draw.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"
#include "BaseSystem/Config.h"


void server_Main(void* UserData)
{
    sfMutex_Lock(server_creation);

    Map* map = (Map*) UserData;
    sfThread* update_disconnect_players = sfThread_Create(&map_UpdateDisconnectedPlayers, map);
    //sfThread* game_listening = sfThread_Create(&server_Listen_Game, map);
    sfThread* tcp_listening = sfThread_Create(&server_Listen_TCP, map);

    Network_ServerMutex = sfMutex_Create();

    sfSocketUDP_Bind(map->game_socket, map->game_port);
    if (!sfSocketUDP_IsValid(map->game_socket))
        logging_Error("server_Start", "Port already used");

    logging_Info("server_Main", "Listen_TCP thread launch...");
    sfThread_Launch(tcp_listening);
    server_started = true;
    map->chat_started = true;
    logging_Info("server_Main", "UpdateDisconnectedPlayers thread launch...");
    sfThread_Launch(update_disconnect_players);
    logging_Info("server_Main", "Server started !");

    // Ecran d'attente joueurs (Salon de discussion)
    while (!map->game_started && server_started)
        sfSleep(0.1f);

    /*
    sfThread_Launch(game_listening);

    // Boucle principale du serveur
    while(map->game_started)
        sfSleep(0.2f);
    */

    map->chat_started = false;
    logging_Info("server_Main", "Waiting server sub-threads end...");
    sfThread_Wait(tcp_listening);
    sfThread_Wait(update_disconnect_players);

    sfThread_Destroy(tcp_listening);
    sfThread_Destroy(update_disconnect_players);

    logging_Info("server_Main", "Unbind ports");
    sfSocketUDP_Unbind(map->game_socket);
    logging_Info("server_Main", "End Server thread");
}

// Ecoute les connexions, et attend le paquet de connexion du client
void server_Listen_TCP(void* UserData)
{
    Map* map = (Map*) UserData;                                                             // Cast des données, inhérent à SFML

    sfSocketTCP* connect_socket = sfSocketTCP_Create();
    sfSocketTCP_Listen(connect_socket, map->game_port);
    sfSelectorTCP_Add(map->tcp_selector, connect_socket);
    sfMutex_Unlock(server_creation);

    do
    {
        unsigned int nb_sck_ready = sfSelectorTCP_Wait(map->tcp_selector, 0.1f);

        if (nb_sck_ready > 0)
        {
            for (int i = 0; i < nb_sck_ready; i++)
            {
                sfSocketTCP* new_socket = sfSelectorTCP_GetSocketReady(map->tcp_selector, i);

                // Socket d'écoute ?
                if (new_socket == connect_socket)
                {
                    sfIPAddress* new_player_ip = NULL;
                    sfSocketTCP* new_player_sck = NULL;
                    sfPacket* new_player_packet = sfPacket_Create(), *response_packet = sfPacket_Create(), *player_packet = sfPacket_Create();
                    char* name = NULL;

                    sfSocketTCP_Accept(connect_socket, &new_player_sck, new_player_ip);                 // Connexion acceptée
                    logging_Info("server_Listen_TCP", "New client connected !");
                    sfSocketTCP_ReceivePacket(new_player_sck, new_player_packet);                       // Réception du paquet
                    logging_Info("server_Listen_TCP", "Read packet code...");
                    unsigned int packet_code = (unsigned int) sfPacket_ReadUint8(new_player_packet);    // Lecture du code du paquet

                    if (packet_code == CONNECT)
                    {
                        // S'il reste de la place sur le serveur on accepte
                        if (map->nb_players < map->max_players)
                        {
                            Player* new_player_ptr = NULL;
                            name = (char*) malloc(20 * sizeof(char));

                            // Ajout au sélecteur TCP
                            sfSelectorTCP_Add(map->tcp_selector, new_player_sck);

                            sfPacket_ReadString(new_player_packet, name);

                            // Verrouillage
                            sfMutex_Lock(Network_ServerMutex);

                            // Création et ajout du player
                            logging_Info("server_Listen_TCP", "Adding player to map");
                            new_player_ptr = player_Create(name, CROWBAR);
                            map_AddPlayer(map, new_player_ptr);
                            map->players_list[map->nb_players - 1]->player_ip = new_player_ip;
                            map->players_list[map->nb_players - 1]->listen_socket = new_player_sck;

                            // Déverrouillage, on évite les blocage à cause de la vitesse moindre du réseau
                            sfMutex_Unlock(Network_ServerMutex);

                            // Réponse du serveur
                            logging_Info("server_Listen_TCP", "Respond to client");
                            player_packet = server_CreateResponsePacket(map, ACCEPTED);
                            sfSocketTCP_SendPacket(new_player_sck, player_packet);

                            // Envoyer les players présents au nouveau client
                            logging_Info("server_Listen_TCP", "Send connected players to the new client...");
                            for (int i = 0; i < map->nb_players; i++)
                            {
                                Packet* players_packet = player_CreateStartPacket(map->players_list[i]);
                                sfSocketTCP_SendPacket(new_player_sck, players_packet->packet);
                                packet_Destroy(players_packet);
                            }

                            // Avertir les autres clients
                            logging_Info("server_Listen_TCP", "Send the new player to connected clients...");
                            Packet* new_player_packet_obj = player_CreateStartPacket(map->players_list[map->nb_players - 1]);
                            for (int i = 0; i < map->nb_players - 1; i++)
                                sfSocketTCP_SendPacket(map->players_list[i]->listen_socket, new_player_packet_obj->packet);

                            // Nettoyage
                            packet_Destroy(new_player_packet_obj);
                            sfPacket_Destroy(new_player_packet);
                            sfPacket_Destroy(response_packet);
                            sfPacket_Destroy(player_packet);
                        }
                        else  // Sinon on refuse
                        {
                            logging_Info("server_Listen_TCP", "Player limit reached ! Refuse connection...");
                            sfPacket_Destroy(new_player_packet);
                            logging_Info("server_Listen_TCP", "Send response packet...");
                            response_packet = server_CreateResponsePacket(NULL, REFUSED);                   // Création du paquet de réponse
                            sfSocketTCP_SendPacket(new_player_sck, response_packet);                        // Envoi
                            // Nettoyage
                            sfPacket_Destroy(response_packet);
                            sfSocketTCP_Destroy(new_player_sck);
                        }
                    }
                }
                else // Cas socket client
                {
                    unsigned int packet_code;
                    sfPacket* packet = sfPacket_Create();
                    sfSocketTCP_ReceivePacket(new_socket, packet);                               // Réception
                    logging_Info("server_Listen_TCP", "New packet received ! Reading...");
                    packet_code = (unsigned int) sfPacket_ReadUint8(packet);                        // Lecture code

                    sfMutex_Lock(Network_ServerMutex);                                              // Verrouillage

                    switch (packet_code)
                    {
                    case CHAT:
                    {
                        logging_Info("server_Listen_TCP", "CHAT type packet, resend to all");
                        sfPacket* resend = sfPacket_Create();                                       // Paquet de renvoi
                        char* mess = (char*) malloc(255 * sizeof(char));

                        sfPacket_ReadString(packet, mess);                                          // Lecture message
                        sfPacket_WriteUint8(resend, (sfUint8) CHAT);                                // Création paquet
                        sfPacket_WriteString(resend, mess);

                        for (int i = 0; i < map->nb_players; i++)                                   // Envoi à tous
                            sfSocketTCP_SendPacket(map->players_list[i]->listen_socket, resend);

                        // Nettoyage
                        sfPacket_Destroy(packet);
                        sfPacket_Destroy(resend);
                        break;
                    }

                    case DISCONNECT:
                    {
                        logging_Info("server_Listen_TCP", "DISCONNECT type packet, flag player for destroy");

                        // Marquage du joueur pour suppression
                        map_GetPlayerFromID(map, (unsigned int) sfPacket_ReadUint8(packet))->connected = false;

                        // Nettoyage
                        sfPacket_Destroy(packet);
                        break;
                    }

                    default:
                        break;
                    }

                    sfMutex_Unlock(Network_ServerMutex);
                }
            }
        }
    }
    while (server_started);

    sfSocketTCP_Destroy(connect_socket);
}

void server_Listen_Game(void* UserData)
{
    Map* map = (Map*) UserData;

    while (server_started)
    {
        sfPacket* packet = sfPacket_Create();
        sfIPAddress* packet_ip = NULL;

        // Réception et Lecture
        sfSocketUDP_ReceivePacket(map->game_socket, packet, packet_ip);
        server_ReadUDPPacket(packet, map);

        // Envoi des nouvelles données à chaque player
        map_CreateGamePackets(map);

        for (int i = 0; i < map->game_packets2send->nb_packets; i++)
            if (map->game_packets2send->packets[i]->code != WEAPON && map->game_packets2send->packets[i]->code != AMMO)
                for (int j = 0; j < map->nb_players; j++)
                    sfSocketUDP_SendPacket(map->game_socket, map->game_packets2send->packets[i]->packet, *map->players_list[j]->player_ip, map->game_port);
            else
                for (int j = 0; j < map->nb_players; j++)
                    sfSocketTCP_SendPacket(map->players_list[j]->listen_socket, map->game_packets2send->packets[i]->packet);

        // Nettoyage
        map_DestroyAllPackets(map);
        sfPacket_Destroy(packet);
        free_secure(packet_ip);

        // Environ 50 actualisations par seconde
        sfSleep(0.02f);
    }
}

// Paquet de réponse (ACCEPTED || REFUSED)
sfPacket* server_CreateResponsePacket(Map* map, unsigned int response)
{
    sfPacket* new_packet = sfPacket_Create();
    sfPacket_WriteUint8(new_packet, (sfUint8) response);            // Réponse du serveur
    if (response == ACCEPTED)
    {
        sfPacket_WriteUint8(new_packet, (sfUint8) map->mapId);
        sfPacket_WriteUint8(new_packet, (sfUint8) map->max_players);
        sfPacket_WriteUint8(new_packet, (sfUint8) map->nb_players);
        sfPacket_WriteUint8(new_packet, (sfUint8) map->cpt_players_rev);
    }
    return new_packet;
}

void server_ReadUDPPacket(sfPacket* packet, Map* map)
{
    unsigned int packet_code = sfPacket_ReadUint8(packet);
    switch (packet_code)
    {
    case PLAYER:
        player_ReadPacket(map, packet);
        break;

    case OBJECT:
        object_ReadPacket(map, packet);
        break;

    case BULLET:
        bullet_ReadPacket(map, packet);
        break;

    default:
        return;
    }
}
