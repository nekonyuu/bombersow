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
#include "Objects/GameObjects.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"

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
    if (!packet)
    {
        logging_Warning("packet_Destroy", "Packet object sent NULL, abort");
        return;
    }

    sfPacket_Destroy(packet->packet);
    free_secure(packet);
}

Packet* player_CreatePacket(Player* player_)
{
    if (!player_)
        logging_Error("player_PacketCreate", "Player object sent NULL, can't continue networking", NULL_PTR);

    sfPacket* new_packet = sfPacket_Create();

    sfPacket_WriteUint8(new_packet, PLAYER_PACKET);
    sfPacket_WriteUint8(new_packet, player_->player_id);
    sfPacket_WriteUint8(new_packet, player_->current_weapon);
    sfPacket_WriteFloat(new_packet, player_->coord_x);
    sfPacket_WriteFloat(new_packet, player_->coord_y);

    return packet_Create(PLAYER_PACKET, new_packet);
}

Packet* player_CreateStartPacket(Player* player_)
{
    if (!player_)
        logging_Error("player_PacketCreate", "Player object sent NULL, can't continue networking", NULL_PTR);

    sfPacket* new_packet = sfPacket_Create();

    sfPacket_WriteUint8(new_packet, PLAYER_PACKET);
    sfPacket_WriteUint8(new_packet, player_->player_id);
    sfPacket_WriteString(new_packet, player_->char_name);
    sfPacket_WriteUint8(new_packet, player_->current_weapon);
    sfPacket_WriteFloat(new_packet, player_->coord_x);
    sfPacket_WriteFloat(new_packet, player_->coord_y);

    return packet_Create(PLAYER_PACKET, new_packet);
}

Player* player_CreateFromPacket(Map* map, sfPacket* packet)
{
    if(!packet)
        logging_Error("player_CreateFromPacket", "sfPacket sent NULL", NULL_PTR);

    Player* new_player;
    char* name = (char*) malloc(20 * sizeof(char));
    unsigned int player_id = (unsigned int) sfPacket_ReadUint8(packet);
    sfPacket_ReadString(packet, name);
    new_player = player_Create(name, (unsigned int) sfPacket_ReadUint8(packet));
    new_player->player_id = player_id;
    new_player->coord_x = sfPacket_ReadFloat(packet);
    new_player->coord_y = sfPacket_ReadFloat(packet);

    return new_player;
}

void player_ReadPacket(Map* map, sfPacket* packet)
{
    unsigned int player_id = (unsigned int) sfPacket_ReadUint8(packet);
    Player* ptr = map_GetPlayerFromID(map, player_id);
    ptr->current_weapon = (unsigned int) sfPacket_ReadUint8(packet);
    ptr->coord_x = sfPacket_ReadFloat(packet);
    ptr->coord_y = sfPacket_ReadFloat(packet);
}

Packet* object_CreatePacket(Object* object_)
{
    if (!object_)
        logging_Error("object_CreatePacket", "Object object sent NULL, can't continue networking", NULL_PTR);

    sfPacket* new_packet = sfPacket_Create();

    sfPacket_WriteUint8(new_packet, OBJECT_PACKET);
    sfPacket_WriteUint8(new_packet, object_->type);
    sfPacket_WriteUint8(new_packet, object_->objectID);

    switch (object_->type)
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

    return packet_Create(OBJECT_PACKET, new_packet);
}

void object_ReadPacket(Map* map, sfPacket* packet)
{
    if (!packet)
    {
        logging_Warning("object_ReadPacket", "sfPacket sent NULL, abort");
        return;
    }

    unsigned int type = (unsigned int) sfPacket_ReadUint8(packet);
    unsigned int id = (unsigned int) sfPacket_ReadUint8(packet);

    switch (type)
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
    if (!bullet)
        logging_Error("bullet_CreatePacket", "Bullet object sent NULL, can't continue networking", NULL_PTR);

    sfPacket* packet = sfPacket_Create();

    sfPacket_WriteUint8(packet, (sfUint8) BULLET_PACKET);
    sfPacket_WriteUint8(packet, (sfUint8) bullet->owner);
    sfPacket_WriteUint8(packet, (sfUint8) bullet->bullet_type);
    sfPacket_WriteUint8(packet, (sfUint8) bullet->trajectory);
    sfPacket_WriteUint8(packet, (sfUint8) bullet->range);
    sfPacket_WriteUint8(packet, (sfUint8) bullet->coord_x);
    sfPacket_WriteUint8(packet, (sfUint8) bullet->coord_y);

    return packet_Create(BULLET_PACKET, packet);
}

void bullet_ReadPacket(Map* map, sfPacket* packet)
{

}

void map_CreateGamePackets(Map* map_)
{
    Bullet* ptr = NULL;
    int cpt = 0;

    if (!map_)
        logging_Error("map_CreateAllPacket", "Map object sent NULL, can't create packet list", NULL_PTR);

    assert(map_->gamepackets2send = (PacketList*) malloc(sizeof(PacketList)));

    sfMutex_Lock(Network_ServerMutex);                                                  // Verrouillage des ressources

    map_->gamepackets2send->nb_packets = map_->nb_players + BulletList_GetNbBullets(map_->bullets);

    // Compte le nombre de paquets Object à créer
    for (int i = 0; i < map_->nb_objects; i++)
        if (map_->objects_list[i]->type > 0)
            map_->gamepackets2send->nb_packets++;

    assert(map_->gamepackets2send->packets = (Packet**) malloc(map_->gamepackets2send->nb_packets * sizeof(Packet*)));

    // Création des paquets
    for (int i = 0; i < map_->nb_players; i++)
        map_->gamepackets2send->packets[i] = player_CreatePacket(map_->players_list[i]);

    for (cpt = 0, ptr = BulletList_GetHead(map_->bullets); ptr != NULL; ptr = bullet_GetNext(ptr), cpt++)
        map_->gamepackets2send->packets[cpt] = bullet_CreatePacket(ptr);

    for (int i = map_->nb_players; i < map_->gamepackets2send->nb_packets; i++)
        if (map_->objects_list[i]->type > 0)
            map_->gamepackets2send->packets[i] = object_CreatePacket(map_->objects_list[i]);

    sfMutex_Unlock(Network_ServerMutex);                                                  // Déverrouillage
}

void map_DestroyAllPackets(Map* map_)
{
    for (int i = 0; i < map_->gamepackets2send->nb_packets; i++)
        packet_Destroy(map_->gamepackets2send->packets[i]);
    free_secure(map_->gamepackets2send->packets);
    free_secure(map_->gamepackets2send);
}

ClientData* clientdata_Create(char* player_name, char* ip, unsigned int port, Config* config, unsigned int max_mess)
{
    ClientData* client_data = (ClientData*) malloc(sizeof(ClientData));
    client_data->map = NULL;

    client_data->messages = ChatMessagesList_Create(max_mess);

    client_data->player = NULL;
    client_data->name = player_name;
    client_data->ip = sfIPAddress_FromString(ip);

    client_data->port = port;
    client_data->config = config;

    client_data->server_close = false;

    return client_data;
}

void clientdata_Destroy(ClientData* ptr)
{
    ChatMessagesList_Destroy(ptr->messages);
    free_secure(ptr);
}
