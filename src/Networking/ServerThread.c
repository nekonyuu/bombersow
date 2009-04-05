#include "BaseSystem/Logging.h"
#include "GraphicEngine/Draw.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"
#include "BaseSystem/Config.h"

void server_Main(Map* map)
{
    sfThread* players_listening = sfThread_Create(&server_Listen_Connections, map);
    sfThread* update_disconnect_players = sfThread_Create(&map_UpdateDisconnectedPlayers, map);

    map->game_socket = sfSocketUDP_Create();
    sfSocketUDP_Bind(map->game_socket, map->game_port);
    if (!sfSocketUDP_IsValid(map->game_socket))
        logging_Error("server_Start", "Port already used");

    sfThread_Launch(players_listening);
    sfThread_Launch(update_disconnect_players);
    server_started = true;
    map->chat_started = true;

    // Ecran d'attente joueurs (Salon de discussion)
    do
    {


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
    Map* map = (Map*) UserData;                                                             // Cast des données, inhérent  SFML

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
            if (map->nb_players < NB_MAX_PLAYERS)
            {
                ChatData* player_data = NULL;
                sfPacket_ReadString(new_player_packet, name);
                sfMutex_Lock(Network_ServerMutex);                                          // Verrouillage
                map_AddPlayer(map, player_Create(name, CROWBAR));                           // Création et ajout du player
                map->players_list[map->nb_players - 1]->player_ip = new_player_ip;
                map->players_list[map->nb_players - 1]->listen_socket = new_player;
                // Réponse du serveur
                player_packet = server_CreateResponsePacket(map->players_list[map->nb_players - 1]->player_id, ACCEPTED);
                // Envoyer les players présents au nouveau client
                for(int i = 0; i < map->nb_players - 1; i++)
                    sfSocketTCP_SendPacket(new_player, player_CreatePacket(map->players_list[i]));
                // Avertir les autres clients
                player_packet = player_CreatePacket(map->players_list[map->nb_players - 1]);
                for(int i = 0; i < map->nb_players - 2; i++)
                    sfSocketTCP_SendPacket(map->players_list[i]->listen_socket, response_packet);
                // Nettoyage
                sfPacket_Destroy(new_player_packet);
                sfPacket_Destroy(response_packet);
                sfPacket_Destroy(player_packet);
                // Préparation et démarrage du thread d'écoute TCP du player
                player_data = chat_CreatePlayerData(map, map->players_list[map->nb_players - 1]->player_id);
                map->players_list[map->nb_players - 1]->player_thread = sfThread_Create(&server_Listen_TCP, player_data);
                sfThread_Launch(map->players_list[map->nb_players - 1]->player_thread);
                sfMutex_Unlock(Network_ServerMutex);                                        // Déverrouille les données
            }
            else  // Sinon on refuse
            {
                sfPacket_Destroy(new_player_packet);
                response_packet = server_CreateResponsePacket(0, REFUSED);                  // Création du paquet de réponse
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
    ChatData* player_data = (ChatData*) UserData;                                           // Cast des données void* envoyées, inhérent à SFML
    Map* map = player_data->map;
    unsigned int* index = &map->players_list[player_data->player_id]->player_id;
    sfSocketTCP* player_socket = map->players_list[*index]->listen_socket;
    sfPacket* chat_packet = sfPacket_Create();

    do
    {
        unsigned int packet_code;
        sfSocketTCP_ReceivePacket(player_socket, chat_packet);                              // Réception
        packet_code = (unsigned int) sfPacket_ReadUint8(chat_packet);                       // Lecture code

        sfMutex_Lock(Network_ServerMutex);                                                  // Verrouillage

        switch (packet_code)
        {
        case CHAT:
        {
            sfPacket* resend = sfPacket_Create();                                           // Paquet de renvoi
            char* mess;
            sfPacket_ReadString(chat_packet, mess);                                         // Lecture message
            sfPacket_WriteUint8(resend, (sfUint8) CHAT);                                    // Création paquet
            sfPacket_WriteString(resend, mess);
            for (int i = 0; i < map->nb_players; i++)                                       // Envoi à tous
                sfSocketTCP_SendPacket(map->players_list[i]->listen_socket, resend);
            // Nettoyage
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

        sfMutex_Unlock(Network_ServerMutex);
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
        sfMutex_Lock(Network_ServerMutex);          // On verrouille pour éviter les modifications extérieures



        sfMutex_Unlock(Network_ServerMutex);        // On déverrouille pour les autres threads
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
    case PLAYER:
        break;

    case OBJECT:
        break;

    default:
        return;
    }
}
