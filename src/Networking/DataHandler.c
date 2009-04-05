#include "BaseSystem/Logging.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"

// Le destructeur de paquet utilisé sera sfPacket_Destroy(sfPacket*)

sfPacket* player_CreatePacket(Player* player_)
{
    if (!player_)
        logging_Error("player_PacketCreate", "Player object sent NULL, can't continue networking");

    sfPacket* new_packet = sfPacket_Create();

    sfPacket_WriteUint8(new_packet, PLAYER);
    sfPacket_WriteString(new_packet, player_->char_name);
    sfPacket_WriteUint8(new_packet, player_->current_weapon);
    sfPacket_WriteFloat(new_packet, player_->coord_x);
    sfPacket_WriteFloat(new_packet, player_->coord_y);

    return new_packet;
}

Player* player_CreateFromPacket(sfPacket* packet)
{
    assert(packet);

    Player* new_player;
    assert(new_player = (Player*) malloc(sizeof(Player)));

    // TODO : Lecture du paquet, brainstorming sur la gestion des players

    return new_player;
}

sfPacket* object_CreatePacket(Object* object_)
{
    if (!object_)
        logging_Error("object_CreatePacket", "Object object sent NULL, can't continue networking");

    sfPacket* new_packet = sfPacket_Create();

    sfPacket_WriteUint8(new_packet, OBJECT);
    sfPacket_WriteUint8(new_packet, object_->objectID);
    sfPacket_WriteFloat(new_packet, object_->curr_coord_x);
    sfPacket_WriteFloat(new_packet, object_->curr_coord_y);
    sfPacket_WriteUint8(new_packet, object_->speed);

    return new_packet;
}

Object* object_CreateFromPacket(sfPacket* packet)
{
    assert(packet);
    Object* new_object = NULL;

    return new_object;
}

void map_CreateGamePackets(Map* map_)
{
    if (!map_)
        logging_Error("map_CreateAllPacket", "Map object sent NULL, can't create packet list");

    assert(map_->game_packets2send = (PacketList*) malloc(sizeof(PacketList)));

    map_->game_packets2send->nb_packets = map_->nb_players;

    for (int i = 0; i < map_->nb_objects; i++)
        if (map_->objects_list[i]->type > 0)
            map_->game_packets2send->nb_packets++;

    assert(map_->game_packets2send->packets = (sfPacket**) malloc(map_->game_packets2send->nb_packets * sizeof(sfPacket*)));

    for (int i = 0; i < map_->nb_players; i++)
        map_->game_packets2send->packets[i] = player_CreatePacket(map_->players_list[i]);

    for (int i = map_->nb_players; i < map_->game_packets2send->nb_packets; i++)
        if (map_->objects_list[i]->type > 0)
            map_->game_packets2send->packets[i] = object_CreatePacket(map_->objects_list[i]);
}

void map_DestroyAllPackets(Map* map_)
{
    for (int i = 0; i < map_->game_packets2send->nb_packets; i++)
        sfPacket_Destroy(map_->game_packets2send->packets[i]);
    free(map_->game_packets2send->packets);
    free(map_->game_packets2send);
}
