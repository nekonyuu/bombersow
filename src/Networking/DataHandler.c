#include "BaseSystem/Logging.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"

// Le destructeur de paquet utilisé sera sfPacket_Destroy(sfPacket*)

Packet* packet_Create(unsigned int code, sfPacket* packet)
{
    Packet* new_packet = NULL;
    assert(new_packet = (Packet*) malloc(sizeof(Packet)));

    new_packet->code = code;
    new_packet->packet = packet;

    return new_packet;
}

void packet_Destroy(Packet* packet)
{
    if(!packet)
    {
        logging_Warning("packet_Destroy", "Packet object sent NULL, abort");
        return;
    }

    sfPacket_Destroy(packet->packet);
    free(packet);
}

Packet* player_CreatePacket(Player* player_)
{
    if (!player_)
        logging_Error("player_PacketCreate", "Player object sent NULL, can't continue networking");

    sfPacket* new_packet = sfPacket_Create();

    sfPacket_WriteUint8(new_packet, PLAYER);
    sfPacket_WriteUint8(new_packet, player_->player_id);
    sfPacket_WriteUint8(new_packet, player_->current_weapon);
    sfPacket_WriteFloat(new_packet, player_->coord_x);
    sfPacket_WriteFloat(new_packet, player_->coord_y);

    return packet_Create(PLAYER, new_packet);
}

Player* player_CreateFromPacket(sfPacket* packet)
{
    assert(packet);

    Player* new_player;
    assert(new_player = (Player*) malloc(sizeof(Player)));

    // TODO : Lecture du paquet, brainstorming sur la gestion des players

    return new_player;
}

void player_ReadPacket(Map* map, sfPacket* packet)
{
    unsigned int player_id = (unsigned int) sfPacket_ReadUint8(packet);
    map->players_list[player_id]->current_weapon = (unsigned int) sfPacket_ReadUint8(packet);
    map->players_list[player_id]->coord_x = sfPacket_ReadFloat(packet);
    map->players_list[player_id]->coord_y = sfPacket_ReadFloat(packet);
}

Packet* object_CreatePacket(Object* object_)
{
    if (!object_)
        logging_Error("object_CreatePacket", "Object object sent NULL, can't continue networking");

    sfPacket* new_packet = sfPacket_Create();

    sfPacket_WriteUint8(new_packet, OBJECT);
    sfPacket_WriteUint8(new_packet, object_->type);
    sfPacket_WriteUint8(new_packet, object_->objectID);

    switch(object_->type)
    {
    case PLATFORM_DYNA:
    case TRAP:
        sfPacket_WriteFloat(new_packet, object_->curr_coord_x);
        sfPacket_WriteFloat(new_packet, object_->curr_coord_y);
        break;

    case WEAPON:
    case AMMO:
        sfPacket_WriteBool(new_packet, object_->spawned);
        break;

    default:
        break;
    }

    return packet_Create(OBJECT, new_packet);
}

void object_ReadPacket(Map* map, sfPacket* packet)
{
    if(!packet)
    {
        logging_Warning("object_ReadPacket", "sfPacket sent NULL, abort");
        return;
    }

    unsigned int type = (unsigned int) sfPacket_ReadUint8(packet);
    unsigned int id = (unsigned int) sfPacket_ReadUint8(packet);

    switch(type)
    {
    case PLATFORM_DYNA:
    case TRAP:
        map->objects_list[id]->curr_coord_x = sfPacket_ReadFloat(packet);
        map->objects_list[id]->curr_coord_y = sfPacket_ReadFloat(packet);
        break;

    case WEAPON:
    case AMMO:
        map->objects_list[id]->spawned = sfPacket_ReadBool(packet);
        break;
    }

    return;
}

Packet* bullet_CreatePacket(Bullet* bullet)
{
    if (!object_)
        logging_Error("bullet_CreatePacket", "Bullet object sent NULL, can't continue networking");

    sfPacket* packet = sfPacket_Create();

    sfPacket_WriteUint8(packet, (sfUint8) BULLET);
    sfPacket_WriteUint8(packet, (sfUint8) bullet->owner);
    sfPacket_WriteUint8(packet, (sfUint8) bullet->type);
    sfPacket_WriteUint8(packet, (sfUint8) bullet->trajectory);
    sfPacket_WriteUint8(packet, (sfUint8) bullet->range);
    sfPacket_WriteUint8(packet, (sfUint8) bullet->coord_x);
    sfPacket_WriteUint8(packet, (sfUint8) bullet->coord_y);

    return packet_Create(BULLET, packet);
}

void bullet_ReadPacket(Map* map, sfPacket* packet)
{

}

void map_CreateGamePackets(Map* map_)
{
    if (!map_)
        logging_Error("map_CreateAllPacket", "Map object sent NULL, can't create packet list");

    assert(map_->game_packets2send = (PacketList*) malloc(sizeof(PacketList)));

    sfMutex_Lock(Network_ServerMutex);                                                  // Verrouillage des ressources

    map_->game_packets2send->nb_packets = map_->nb_players + map_->nb_bullets;

    // Compte le nombre de paquets à créer
    for (int i = 0; i < map_->nb_objects; i++)
        if (map_->objects_list[i]->type > 0)
            map_->game_packets2send->nb_packets++;

    assert(map_->game_packets2send->packets = (Packet**) malloc(map_->game_packets2send->nb_packets * sizeof(Packet*)));

    // Création des paquets
    for (int i = 0; i < map_->nb_players; i++)
        map_->game_packets2send->packets[i] = player_CreatePacket(map_->players_list[i]);

    for (int i = 0; i < map_->nb_bullets; i++)
        map_->game_packets2send->packets[i] = bullet_CreatePacket(map_->bullets_list[i]);

    for (int i = map_->nb_players; i < map_->game_packets2send->nb_packets; i++)
        if (map_->objects_list[i]->type > 0)
            map_->game_packets2send->packets[i] = object_CreatePacket(map_->objects_list[i]);

    sfMutex_Unlock(Network_ServerMutex);                                                  // Déverrouillage
}

void map_DestroyAllPackets(Map* map_)
{
    for (int i = 0; i < map_->game_packets2send->nb_packets; i++)
        packet_Destroy(map_->game_packets2send->packets[i]);
    free(map_->game_packets2send->packets);
    free(map_->game_packets2send);
}
