#include "BaseSystem/Logging.h"
#include "GraphicEngine/Draw.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"

void server_Main(Map* map, unsigned int port)
{
    sfThread* chat_listening = sfThread_Create(&server_Listen_Connections, map);

    map->game_port = sfSocketUDP_Create();
    sfSocketUDP_Bind(game_bound_socket, (unsigned short) port);
    if(!sfSocketUDP_IsValid(game_bound_socket))
        logging_Error("server_Start", "Sent port already used");

    map->chat_started = true;

    // Ecran d'attente joueurs (Salon de discussion)
    do
    {


        if(map->game_started)
            map->chat_started = false;

        sfSleep(1.0f/FRAMERATE);
    }
    while(map->chat_started);

    /*
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
        map_CreateAllPackets(map);
        for(int i = 0; i < map->packets2send->nb_packets; i++)
            for(int j = 0; j < map->nb_players; j++)
                sfSocketUDP_SendPacket(game_bound_socket, map->packets2send->packets[i], map->players_list[j]->player_ip, (unsigned short) port);

        map_DestroyAllPackets(map);
        sfSleep(1.0f/FRAMERATE);
    }
    while(map->game_started);
    */

    sfSocketTCP_Destroy(chat_bound_socket);
    sfSocketUDP_Unbind(game_bound_socket);
    sfSocketUDP_Destroy(game_bound_socket);
}

void server_Listen_Connections(void* UserData)
{
    Map* map = (Map*) UserData;

    sfSocketTCP* chat_bound_socket = sfSocketTCP_Create();
    sfSocketTCP_Listen(chat_bound_socket, (unsigned short) port);

    while(map->game_started)
    {
        //map_AddPlayer(map, , CROWBAR);
        sfSocketTCP* new_player = sfSocketTCP_Create();
        sfSocketTCP_Accept(chat_bound_socket, new_player, map->players_list[map->nb_players]->player_ip);

        map->nb_players++;
    }
}

void server_Listen_Game(void* UserData)
{
    Player* player = (Player*) UserData;

    while (player->connected)
    {

    }
}
