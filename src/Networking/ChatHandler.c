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

#include <string.h>
#include "BaseSystem/Logging.h"
#include "Objects/GameObjects.h"
#include "Networking/Networking.h"
#include "Networking/PacketDefines.h"

sfPacket* chat_CreatePacket(Player* player, const char* message)
{
    if (!player)
    {
        logging_Warning("chat_CreatePacket", "Player object sent NULL, aborting packet creation");
        return NULL;
    }
    if (!message)
    {
        logging_Warning("chat_CreatePacket", "message sent NULL, aborting packet creation");
        return NULL;
    }

    sfPacket* new_packet = sfPacket_Create();
    sfPacket_WriteUint8(new_packet, (sfUint8) CHAT);
    sfPacket_WriteUint8(new_packet, (sfUint8) player->player_id);
    sfPacket_WriteString(new_packet, message);

    return new_packet;
}

// Lit le paquet et renvoie le message construit
char* chat_ReadPacket(Map* map, sfPacket* packet)
{
    char* text = NULL, *recv_mess = NULL;
    unsigned int player_id = (unsigned int) sfPacket_ReadUint8(packet);

    sfPacket_ReadString(packet, recv_mess);
    sfPacket_Destroy(packet);
    strcpy(text, map->players_list[player_id]->char_name);
    strcat(text, ": ");
    strcat(text, recv_mess);
    return text;
}

// Sert seulement à stocker map et id joueur pour envoi en thread
ChatData* chat_CreatePlayerData(Map* map, unsigned int player_id)
{
    assert(map);

    ChatData* data = NULL;
    assert(data = (ChatData*) malloc(sizeof(ChatData)));

    data->map = map;
    data->player_id = player_id;

    return data;
}

void chat_DestroyPlayerData(ChatData* data)
{
    free_secure(data);
}

ChatMessages* chatmessages_Create()
{
    ChatMessages* ptr = malloc(sizeof(ChatMessages));
    ptr->messages = NULL;
    ptr->nb_mess = 0;

    return ptr;
}

void chatmessages_AddMessage(ChatMessages* ptr, char* mess)
{
    if(!ptr)
        logging_Error("chatmessages_AddMessage", "ChatMessages object sent NULL", NULL_PTR);

    ptr->messages = (sfString**) realloc(ptr->messages, ++ptr->nb_mess * sizeof(sfString*));
    if(!ptr->messages)
        logging_Error("chatmessages_AddMessage", "Memory allocation error", LOW_MEMORY);

    ptr->messages[ptr->nb_mess - 1] = sfString_Create();
    sfString_SetText(ptr->messages[ptr->nb_mess - 1], mess);
    sfString_SetSize(ptr->messages[ptr->nb_mess - 1], 8);
    sfString_SetPosition(ptr->messages[ptr->nb_mess - 1], 0, ptr->nb_mess * 12);
}

void chatmessages_Draw(ChatMessages* ptr, sfRenderWindow* Game, sfView* view)
{
    sfRenderWindow_SetView(Game, view);

    for (int i = 0; i < ptr->nb_mess; i++)
        sfRenderWindow_DrawString(Game, ptr->messages[i]);

    sfRenderWindow_SetView(Game, sfRenderWindow_GetDefaultView(Game));
}

void chatmessages_Destroy(ChatMessages* ptr)
{
    for (int i = 0; i < ptr->nb_mess; i++)
        sfString_Destroy(ptr->messages[i]);
}
