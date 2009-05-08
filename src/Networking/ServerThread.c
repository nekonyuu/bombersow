#include "BaseSystem/Logging.h"
#include "GraphicEngine/Draw.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"
#include "BaseSystem/Config.h"

void server_Main(Map* map)
{
    sfThread* players_listening = sfThread_Create(&server_Listen_Connections, map);
    sfThread* update_disconnect_players = sfThread_Create(&map_UpdateDisconnectedPlayers, map);
    sfThread* game_listening = sfThread_Create(&server_Listen_Game, map);
    Network_ServerMutex = sfMutex_Create();

    map->game_socket = sfSocketUDP_Create();
    sfSocketUDP_Bind(map->game_socket, map->game_port);
    if (!sfSocketUDP_IsValid(map->game_socket))
        logging_Error("server_Start", "Port already used");

    sfThread_Launch(players_listening);
    sfThread_Launch(update_disconnect_players);
    server_started = true;
    map->chat_started = true;

    // Ecran d'attente joueurs (Salon de discussion)
    while (!map->game_started)
        sfSleep(0.2f);

    sfThread_Launch(game_listening);

    // Boucle principale du serveur
    while(map->game_started)
        sfSleep(0.2f);

    map->chat_started = false;
    sfThread_Wait(players_listening);
    sfThread_Wait(update_disconnect_players);

    sfSocketUDP_Unbind(map->game_socket);
    sfSocketUDP_Destroy(map->game_socket);
}

// Ecoute les connexions, et attend le paquet de connexion du client
void server_Listen_Connections(void* UserData)
{
    Map* map = (Map*) UserData;                                                             // Cast des données, inhérent à SFML

    sfSocketTCP* connect_socket = sfSocketTCP_Create();
    sfSocketTCP_Listen(connect_socket, map->game_port);

    do
    {
        sfSocketTCP* new_player = sfSocketTCP_Create();
        sfIPAddress* new_player_ip = NULL;
        sfPacket* new_player_packet = NULL, *response_packet = NULL, *player_packet = NULL;
        char* name = NULL;
        sfSocketTCP_Accept(connect_socket, &new_player, new_player_ip);                     // Connexion acceptée
        sfSocketTCP_ReceivePacket(new_player, new_player_packet);                           // Réception du paquet
        unsigned int packet_code = (unsigned int) sfPacket_ReadUint8(new_player_packet);    // Lecture du code du paquet
        switch (packet_code)
        {
        // Paquet de connexion
        case CONNECT:
            // S'il reste de la place sur le serveur on accepte
            if (map->nb_players < map->max_players)
            {
                ChatData* player_data = NULL;
                sfPacket_ReadString(new_player_packet, name);
                sfMutex_Lock(Network_ServerMutex);                                          // Verrouillage
                map_AddPlayer(map, player_Create(name, CROWBAR));                           // Création et ajout du player
                map->players_list[map->nb_players - 1]->player_ip = new_player_ip;
                map->players_list[map->nb_players - 1]->listen_socket = new_player;
                // Réponse du serveur
                player_packet = server_CreateResponsePacket(map, ACCEPTED);
                sfSocketTCP_SendPacket(new_player, player_packet);
                // Envoyer les players présents au nouveau client
                for(int i = 0; i < map->nb_players; i++)
                    sfSocketTCP_SendPacket(new_player, player_CreateStartPacket(map->players_list[i])->packet);
                // Avertir les autres clients
                player_packet = player_CreatePacket(map->players_list[map->nb_players - 1])->packet;
                for(int i = 0; i < map->nb_players - 1; i++)
                    sfSocketTCP_SendPacket(map->players_list[i]->listen_socket, player_packet);
                // Nettoyage
                sfPacket_Destroy(new_player_packet);
                sfPacket_Destroy(response_packet);
                sfPacket_Destroy(player_packet);
                // Préparation et démarrage du thread d'écoute TCP du player
                player_data = chat_CreatePlayerData(map, map->players_list[map->nb_players - 1]->player_id);
                map->players_list[map->nb_players - 1]->player_thread = sfThread_Create(&server_Listen_TCP, player_data);
                sfMutex_Unlock(Network_ServerMutex);                                        // Déverrouille les données
                sfThread_Launch(map->players_list[map->nb_players - 1]->player_thread);
            }
            else  // Sinon on refuse
            {
                sfPacket_Destroy(new_player_packet);
                response_packet = server_CreateResponsePacket(NULL, REFUSED);                  // Création du paquet de réponse
                sfSocketTCP_SendPacket(new_player, response_packet);                        // Envoi
                // Nettoyage
                sfPacket_Destroy(response_packet);
                sfSocketTCP_Destroy(new_player);
            }
            break;

        default:
            break;
        }
    }
    while (server_started);

    sfSocketTCP_Destroy(connect_socket);
}

// Thread d'écoute TCP, gère les discussions et déconnexions
void server_Listen_TCP(void* UserData)
{
    ChatData* player_data = (ChatData*) UserData;                                       // Cast des données void* envoyées, inhérent à SFML
    Map* map = player_data->map;
    unsigned int* index = &player_data->player_id;
    sfSocketTCP* player_socket = map->players_list[*index]->listen_socket;
    sfPacket* packet = sfPacket_Create();

    do
    {
        unsigned int packet_code;
        sfSocketTCP_ReceivePacket(player_socket, packet);                               // Réception
        packet_code = (unsigned int) sfPacket_ReadUint8(packet);                        // Lecture code

        sfMutex_Lock(Network_ServerMutex);                                              // Verrouillage

        switch (packet_code)
        {
        case CHAT:
        {
            sfPacket* resend = sfPacket_Create();                                       // Paquet de renvoi
            char* mess = NULL;
            sfPacket_ReadString(packet, mess);                                          // Lecture message
            sfPacket_WriteUint8(resend, (sfUint8) CHAT);                                // Création paquet
            sfPacket_WriteString(resend, mess);
            for (int i = 0; i < map->nb_players; i++)                                   // Envoi à tous
                sfSocketTCP_SendPacket(map->players_list[i]->listen_socket, resend);
            // Nettoyage
            sfPacket_Clear(packet);
            sfPacket_Destroy(resend);
            break;
        }

        case DISCONNECT:
        {
            map->players_list[*index]->connected = false;                               // Marquage du joueur pour suppression
            sfPacket_Destroy(packet);
            break;
        }

        default:
            break;
        }

        sfMutex_Unlock(Network_ServerMutex);
    }
    while (server_started && map->players_list[*index]->connected);


    free_secure(player_data);
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

        for(int i = 0; i < map->game_packets2send->nb_packets; i++)
            if(map->game_packets2send->packets[i]->code != WEAPON && map->game_packets2send->packets[i]->code != AMMO)
                for(int j = 0; j < map->nb_players; j++)
                    sfSocketUDP_SendPacket(map->game_socket, map->game_packets2send->packets[i]->packet, *map->players_list[j]->player_ip, map->game_port);
            else
                for(int j = 0; j < map->nb_players; j++)
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
    if(response == ACCEPTED)
    {
        sfPacket_WriteUint8(new_packet, (sfUint8) map->mapId);
        sfPacket_WriteUint8(new_packet, (sfUint8) map->max_players);
        sfPacket_WriteUint8(new_packet, (sfUint8) map->nb_players);
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
