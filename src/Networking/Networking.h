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

#ifndef NETWORKING_H
#define NETWORKING_H

#include "Objects/GameObjects.h"

#define DEFAULT_PORT 48535

// Permet de regrouper la map et l'id du joueur pour lequel on écoute
typedef struct CHAT_DATA
{
    Map* map;
    unsigned int player_id;
} ChatData;

// Permet de regrouper les données client à envoyer au thread
typedef struct CLIENT_DATA
{
    Map* map;
    char* name;
    sfIPAddress ip;
    int port;
    Config* config;
    bool server_close;
} ClientData;

_Bool server_started;
_Bool client_connected;
sfMutex* server_creation;

// ServerThread.c
void server_Main(void* UserData);
void server_Listen_TCP(void* UserData);
void server_Listen_Game(void* UserData);
sfPacket* server_CreateResponsePacket(Map* map, unsigned int response);
sfPacket* server_CreateDestroyPlayerPacket(unsigned int player_id);
sfPacket* server_CreateClosePacket();
void server_ReadUDPPacket(sfPacket* packet, Map* map);

// ClientThread.c
void client_Main(void* UserData);
sfPacket* client_CreateConnectPacket(char* name);
sfPacket* client_CreateDisconnectPacket(unsigned int player_id);

// ChatHandler.c
sfPacket* chat_CreatePacket(Player* player, const char* message);
char* chat_ReadPacket(Map* map, sfPacket* packet);
ChatData* chat_CreatePlayerData(Map*, unsigned int);

// DataHandler.c
Packet* packet_Create(unsigned int code, sfPacket* packet);
void packet_Destroy(Packet* packet);
Packet* player_CreatePacket(Player*);
Packet* player_CreateStartPacket(Player*);
Player* player_CreateFromPacket(Map*, sfPacket*);
void player_ReadPacket(Map*, sfPacket*);
Packet* object_CreatePacket(Object*);
void object_ReadPacket(Map*, sfPacket*);
Packet* bullet_CreatePacket(Bullet*);
void bullet_ReadPacket(Map*, sfPacket*);
void map_CreateGamePackets(Map*);
void map_DestroyAllPackets(Map*);
ClientData* clientdata_Create(char*, char*, unsigned int, Config*);
void clientdata_Destroy(ClientData*);

#endif
