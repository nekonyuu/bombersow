#include "BaseSystem/Logging.h"
#include "GraphicEngine/Draw.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"

void server_Main(Map* map)
{
    sfThread* chat_listening = sfThread_Create(&server_Listen_Connections, map);

    map->game_socket = sfSocketUDP_Create();
    sfSocketUDP_Bind(map->game_socket, map->game_port);
    if(!sfSocketUDP_IsValid(map->game_socket))
        logging_Error("server_Start", "Sent port already used");

    sfThread_Launch(chat_listening);
    map->chat_started = true;

    // Ecran d'attente joueurs (Salon de discussion)
    do
    {
        // TODO : Mutex des thread d'écoute
        if(map->game_started)
            map->chat_started = false;

        sfSleep(1.0f/FRAMERATE);
    }
    while(map->chat_started);

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
                sfSocketUDP_SendPacket(map->game_socket, map->packets2send->packets[i], *map->players_list[j]->player_ip, map->game_port);

        map_DestroyAllPackets(map);
        sfSleep(1.0f/FRAMERATE);
    }
    while(map->game_started);

    sfSocketUDP_Unbind(map->game_socket);
    sfSocketUDP_Destroy(map->game_socket);
}

void server_Listen_Connections(void* UserData)
{
    Map* map = (Map*) UserData;

    sfSocketTCP* chat_bound_socket = sfSocketTCP_Create();
    sfSocketTCP_Listen(chat_bound_socket, map->game_port);

    while(map->game_started)
    {
        sfSocketTCP* new_player = sfSocketTCP_Create();
        sfIPAddress* new_player_ip = NULL;
        sfPacket* new_player_packet = NULL;
        char* name = NULL;
        sfSocketTCP_Accept(chat_bound_socket, &new_player, new_player_ip);
        sfSocketTCP_ReceivePacket(new_player, new_player_packet);
        if(sfPacket_ReadUint8(new_player_packet) == (sfUint8) CONNECT)
        {
            sfPacket_ReadString(new_player_packet, name);
            map_AddPlayer(map, player_Create(name, CROWBAR));
            map->players_list[map->nb_players - 1]->player_ip = new_player_ip;
            map->players_list[map->nb_players - 1]->chat_socket = new_player;

        }
    }
    sfSocketTCP_Destroy(chat_bound_socket);
}

void server_Listen_Game(void* UserData)
{
    Player* player = (Player*) UserData;

    while (player->connected)
    {

    }
}
