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

#define DEFAULT_PORT 48535

typedef struct CHAT_MESS
{
    struct CHAT_MESS* prev;
    sfString* message;
    struct CHAT_MESS* next;
} ChatMessage;

// Permet de stocker les messages de discusison pour l'affichage
typedef struct CHAT_MESS_VIEW
{
    ChatMessage* head, *tail;
    unsigned int nb_mess;
    unsigned int max_nb_mess;
} ChatMessagesList;

// Permet de regrouper les données client à envoyer au thread
typedef struct CLIENT_DATA
{
    Map* map;

    ChatMessagesList* messages;

    Player* player;
    char* name;
    sfIPAddress ip;

    int port;
    Config* config;

    bool server_close;
} ClientData;

bool server_started;
bool client_connected;
sfMutex* server_creation;

// ServerThread.c
void server_Main(void* UserData);
void server_Listen_TCP(void* UserData);
void server_Listen_Game(void* UserData);
sfPacket* server_CreateResponsePacket(Map* map, Player* player, unsigned int response);
sfPacket* server_CreateDestroyPlayerPacket(unsigned int player_id);
sfPacket* server_CreateClosePacket();
void server_ReadUDPPacket(sfPacket* packet, Map* map);

// ClientThread.c
void client_Main(void* UserData);
sfPacket* client_CreateConnectPacket(char* name);
sfPacket* client_CreateDisconnectPacket(unsigned int player_id);
void client_SendChatPacket(char* mess, Player* player);

// ChatHandler.c
sfPacket* chat_CreatePacket(Player* player, const char* message);
char* chat_ReadPacket(Map* map, sfPacket* packet);
ChatMessage* ChatMessage_Create();
void ChatMessage_Destroy(ChatMessage* ptr);
void ChatMessage_DestroyFromList(ChatMessage* ptr);
void ChatMessage_SetPrev(ChatMessage* ptr, ChatMessage* prev_ptr);
void ChatMessage_SetNext(ChatMessage* ptr, ChatMessage* next_ptr);
ChatMessage* ChatMessage_GetPrev(ChatMessage* ptr);
ChatMessage* ChatMessage_GetNext(ChatMessage* ptr);
void ChatMessage_SetText(ChatMessage* ptr, char* mess, unsigned int size);
void ChatMessage_SetCoords(ChatMessage* ptr, unsigned int x, unsigned int y);
unsigned int ChatMessage_GetSize(ChatMessage* ptr);
ChatMessagesList* ChatMessagesList_Create(unsigned int max_mess);
void ChatMessagesList_AddMessage(ChatMessagesList* ptr, char* mess);
void ChatMessagesList_Draw(ChatMessagesList* ptr, sfRenderWindow* Game);
void ChatMessagesList_UpdateCoords(ChatMessagesList* ptr);
void ChatMessagesList_Destroy(ChatMessagesList* ptr);

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
ClientData* clientdata_Create(char*, char*, unsigned int, Config*, unsigned int);
void clientdata_Destroy(ClientData*);

#endif
