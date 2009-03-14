#include "BaseSystem/Logging.h"
#include "GraphicEngine/Draw.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"

void server_Main(Map* map, unsigned int port)
{
    sfSocketUDP* bound_socket = sfSocketUDP_Create();
    _Bool chat_start;
    sfSocketUDP_Bind(bound_socket, (unsigned short) port);
    if(!sfSocketUDP_IsValid(bound_socket))
        logging_Error("server_Start", "Sent port already used");

    chat_start = true;

    for (int i = 0; i < map->nb_players; i++)
        sfThread_Launch(map->players_list[i]->listening_thread);

    while(chat_start)
    {
        //
        chat_start = false;
        sfSleep(1.0f/FRAMERATE);
    }

    /*while(started)
    {
        // TODO : Polling packets reçus
        PacketList* packets = map_CreateAllPackets(map);
        for(int i = 0; i < packets->nb_packets; i++)
        {
            for(int j = 0; j < map->nb_players; j++)
            {
                sfSocketTCP_SendPacket(bound_socket, packets->packets[i], map->players_list[j]->player_ip, (unsigned short) port);
            }
        }
        sfSleep(1.0f/FRAMERATE);
    }*/

    sfSocketUDP_Unbind(bound_socket);
    sfSocketUDP_Destroy(bound_socket);
}

void server_Listen(void* UserData)
{
    Player* player = (Player*) UserData;
    _Bool connected = true;

    while (connected)
    {

    }
}
