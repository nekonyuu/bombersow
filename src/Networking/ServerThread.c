#include "BaseSystem/Logging.h"
#include "GraphicEngine/Draw.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"

void server_Main(Map* map)
{
    sfThread* players_listening = sfThread_Create(&server_Listen_Connections, map);
    sfThread* update_disconnect_players = sfThread_Create(&map_UpdateDisconnectedPlayers, map);

    map->game_socket = sfSocketUDP_Create();
    sfSocketUDP_Bind(map->game_socket, map->game_port);
    if (!sfSocketUDP_IsValid(map->game_socket))
        logging_Error("server_Start", "Sent port already used");

    sfThread_Launch(players_listening);
    sfThread_Launch(update_disconnect_players);
    server_started = true;
    map->chat_started = true;

    // Ecran d'attente joueurs (Salon de discussion)
    do
    {
        // TODO : Mutex des thread d'écoute

        sfSleep(1.0f/FRAMERATE);
    }
    while (!map->game_started);

    /* A REVOIR
    // Préparation des threads pour l'écoute des paquets
    for (int i = 0; i < map->nb_players; i++)
    {
        map->players_list[i]->player_thread = sfThread_Create(&server_Listen_Game, map->players_list[i]);
        sfThread_Launch(map->players_list[i]->player_thread);
    }

    // Boucle principale du serveur
    do
    {
        // TODO : Polling packets reçus
        map_CreateGamePackets(map);
        for(int i = 0; i < map->game_packets2send->nb_packets; i++)
            for(int j = 0; j < map->nb_players; j++)
                sfSocketUDP_SendPacket(map->game_socket, map->game_packets2send->packets[i], *map->players_list[j]->player_ip, map->game_port);

        map_DestroyAllPackets(map);
        sfSleep(1.0f/FRAMERATE);
    }
    while(map->game_started);
    */

    map->chat_started = false;
    sfThread_Wait(players_listening);
    sfThread_Wait(update_disconnect_players);

    sfSocketUDP_Unbind(map->game_socket);
    sfSocketUDP_Destroy(map->game_socket);
}

// Ecoute les connexions, et attend le paquet de connexion du client
void server_Listen_Connections(void* UserData)
{
    Map* map = (Map*) UserData;

    sfSocketTCP* connect_socket = sfSocketTCP_Create();
    sfSocketTCP_Listen(connect_socket, map->game_port);

    while (server_started)
    {
        sfSocketTCP* new_player = sfSocketTCP_Create();
        sfIPAddress* new_player_ip = NULL;
        sfPacket* new_player_packet = NULL, *response_packet = NULL;
        char* name = NULL;
        sfSocketTCP_Accept(connect_socket, &new_player, new_player_ip);
        sfSocketTCP_ReceivePacket(new_player, new_player_packet);
        unsigned int packet_code = (unsigned int) sfPacket_ReadUint8(new_player_packet);
        switch (packet_code)
        {
        case CONNECT:
            if (map->nb_players < NB_MAX_PLAYERS)
            {
                ChatData* player_data = NULL;
                sfPacket_ReadString(new_player_packet, name);
                map_AddPlayer(map, player_Create(name, CROWBAR));
                map->players_list[map->nb_players - 1]->player_ip = new_player_ip;
                map->players_list[map->nb_players - 1]->chat_socket = new_player;
                response_packet = server_CreateResponsePacket(map->players_list[map->nb_players - 1]->player_id, ACCEPTED);
                sfSocketTCP_SendPacket(new_player, response_packet);
                sfPacket_Destroy(new_player_packet);
                sfPacket_Destroy(response_packet);

                player_data = chat_CreatePlayerData(map, map->players_list[map->nb_players - 1]->player_id);
                map->players_list[map->nb_players - 1]->player_thread = sfThread_Create(&server_Listen_TCP, player_data);
                sfThread_Launch(map->players_list[map->nb_players - 1]->player_thread);
            }
            else
            {
                sfPacket_Destroy(new_player_packet);
                response_packet = server_CreateResponsePacket(0, REFUSED);
                sfSocketTCP_SendPacket(new_player, response_packet);
                sfPacket_Destroy(response_packet);
                sfSocketTCP_Destroy(new_player);
            }
            break;

        default:
            break;
        }
    }
    sfSocketTCP_Destroy(connect_socket);
}

void server_Listen_TCP(void* UserData)
{
    ChatData* player_data = (ChatData*) UserData;
    Map* map = player_data->map;
    unsigned int* index = &map->players_list[player_data->player_id]->player_id;
    sfSocketTCP* player_socket = map->players_list[*index]->chat_socket;
    sfPacket* chat_packet = sfPacket_Create();

    do
    {
        unsigned int packet_code;
        sfSocketTCP_ReceivePacket(player_socket, chat_packet);
        packet_code = (unsigned int) sfPacket_ReadUint8(chat_packet);
        switch (packet_code)
        {
        case CHAT:
        {
            sfPacket* resend = sfPacket_Create();
            char* mess;
            sfPacket_ReadString(chat_packet, mess);
            sfPacket_WriteUint8(resend, (sfUint8) CHAT);
            sfPacket_WriteString(resend, mess);
            for (int i = 0; i < map->nb_players; i++)
                if (i != *index)
                    sfSocketTCP_SendPacket(map->players_list[i]->chat_socket, resend);
            sfPacket_Clear(chat_packet);
            sfPacket_Destroy(resend);
            break;
        }

        case DISCONNECT:
        {
            map->players_list[*index]->connected = false;
            break;
        }

        default:
            break;
        }

    }
    while (server_started && map->players_list[*index]->connected);

    sfPacket_Destroy(chat_packet);
    free(player_data);
}

void server_Listen_Game(void* UserData)
{
    Player* player = (Player*) UserData;

    while (player->connected)
    {

    }
}

// Paquet de réponse (ACCEPTED || REFUSED)
sfPacket* server_CreateResponsePacket(unsigned int player_id, unsigned int response)
{
    sfPacket* new_packet = sfPacket_Create();
    sfPacket_WriteUint8(new_packet, (sfUint8) response);
    sfPacket_WriteUint8(new_packet, (sfUint8) player_id);
    return new_packet;
}

void server_ReadUDPPacket(sfPacket* packet, Map* map)
{
    unsigned int packet_code = sfPacket_ReadUint8(packet);
    switch (packet_code)
    {
    case CHAT:
        break;

    case PLAYER:
        break;

    case OBJECT:
        break;

    default:
        return;
    }
}
