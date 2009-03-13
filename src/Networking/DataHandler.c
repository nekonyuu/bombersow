#include "BaseSystem/Logging.h"
#include "Networking/DataHandler.h"

// Le destructeur de paquet utilisé sera sfPacket_Destroy(sfPacket*)

sfPacket* stplayer_CreatePacket(Player* player_)
{
    if (!player_)
        logging_Error("stplayer_PacketCreate", "Player object sent NULL, can't continue networking");

    sfPacket* new_packet = sfPacket_Create();

    sfPacket_WriteUint8(new_packet, PLAYER);
    sfPacket_WriteString(new_packet, player_->stripped->name);
    sfPacket_WriteUint8(new_packet, player_->stripped->current_weapon);
    sfPacket_WriteFloat(new_packet, player_->stripped->coord_x);
    sfPacket_WriteFloat(new_packet, player_->stripped->coord_y);

    return new_packet;
}

stPlayer* stplayer_CreateFromPacket(sfPacket* packet)
{
    assert(packet);

    stPlayer* new_player;
    assert(new_player = (stPlayer*) malloc(sizeof(stPlayer)));

    // TODO : Lecture du paquet, brainstorming sur la gestion des players

    return new_player;
}

sfPacket* stobject_CreatePacket(Object* object_)
{
    if (!object_)
        logging_Error("stobject_Create", "Object object sent NULL, can't continue networking");

    sfPacket* new_packet = sfPacket_Create();

    sfPacket_WriteUint8(new_packet, OBJECT);
    sfPacket_WriteUint8(new_packet, object_->stripped->objectID);
    sfPacket_WriteFloat(new_packet, object_->stripped->coord_x);
    sfPacket_WriteFloat(new_packet, object_->stripped->coord_y);
    sfPacket_WriteUint8(new_packet, object_->stripped->speed);

    return new_packet;
}

PacketList* map_CreateAllPackets(Map* map_)
{
    if (!map_)
        logging_Error("map_CreateAllPacket", "Map object sent NULL, can't create packet list");

    PacketList* packets;
    assert(packets = (PacketList*) malloc(sizeof(PacketList)));

    packets->nb_packets = map_->nb_players;

    for (int i = 0; i < map_->nb_objects; i++)
        if (map_->objects_list[i]->type > 0)
            packets->nb_packets++;

    assert(packets->packets = (sfPacket**) malloc(packets->nb_packets * sizeof(sfPacket*)));

    for (int i = 0; i < map_->nb_players; i++)
        packets->packets[i] = stplayer_CreatePacket(map_->players_list[i]);

    for (int i = map_->nb_players; i < packets->nb_packets; i++)
        if (map_->objects_list[i]->type > 0)
            packets->packets[i] = stobject_CreatePacket(map_->objects_list[i]);

    return packets;
}

void map_DestroyAllPackets(PacketList* pl)
{
    for (int i = 0; i < pl->nb_packets; i++)
        sfPacket_Destroy(pl->packets[i]);
    free(pl->packets);
    free(pl);
}
