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

#define MARGIN_TOP 350.0f

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
    sfPacket_WriteUint8(new_packet, (sfUint8) CHAT_PACKET);
    sfPacket_WriteUint8(new_packet, (sfUint8) player->player_id);
    sfPacket_WriteString(new_packet, message);

    return new_packet;
}

// Lit le paquet et renvoie le message construit
char* chat_ReadPacket(Map* map, sfPacket* packet)
{
    char* text = malloc(255 * sizeof(char)), *recv_mess = malloc(255 * sizeof(char));
    unsigned int player_id = (unsigned int) sfPacket_ReadUint8(packet);

    sfPacket_ReadString(packet, recv_mess);
    strcpy(text, map_GetPlayerFromID(map, player_id)->char_name);
    strcat(text, ": ");
    strcat(text, recv_mess);

    free_secure(recv_mess);
    return text;
}

ChatMessage* ChatMessage_Create()
{
    ChatMessage* ptr = malloc(sizeof(ChatMessage));

    ptr->prev = NULL;
    ptr->next = NULL;

    ptr->message = sfString_Create();

    return ptr;
}

void ChatMessage_Destroy(ChatMessage* ptr)
{
    sfString_Destroy(ptr->message);
    free_secure(ptr);
}

void ChatMessage_DestroyFromList(ChatMessage* ptr)
{
    if(ptr->prev)
        ptr->prev->next = ptr->next;
    if(ptr->next)
        ptr->next->prev = ptr->prev;

    ChatMessage_Destroy(ptr);
}

void ChatMessage_SetPrev(ChatMessage* ptr, ChatMessage* prev_ptr)
{
    ptr->prev = prev_ptr;
}

void ChatMessage_SetNext(ChatMessage* ptr, ChatMessage* next_ptr)
{
    ptr->next = next_ptr;
}

ChatMessage* ChatMessage_GetPrev(ChatMessage* ptr)
{
    return ptr->prev;
}

ChatMessage* ChatMessage_GetNext(ChatMessage* ptr)
{
    return ptr->next;
}

void ChatMessage_SetText(ChatMessage* ptr, char* mess, unsigned int size)
{
    sfString_SetText(ptr->message, mess);
    sfString_SetSize(ptr->message, size);
}

void ChatMessage_SetCoords(ChatMessage* ptr, unsigned int x, unsigned int y)
{
    sfString_SetPosition(ptr->message, x, y);
}

unsigned int ChatMessage_GetSize(ChatMessage* ptr)
{
    return (int) sfString_GetSize(ptr->message);
}

ChatMessagesList* ChatMessagesList_Create(unsigned int max_mess)
{
    ChatMessagesList* ptr = malloc(sizeof(ChatMessagesList));

    ptr->head = NULL;
    ptr->tail = NULL;

    ptr->nb_mess = 0;
    ptr->max_nb_mess = max_mess;

    return ptr;
}

void ChatMessagesList_AddMessage(ChatMessagesList* ptr, char* mess)
{
    if(!ptr)
        logging_Error("chatmessages_AddMessage", "ChatMessagesList object sent NULL", NULL_PTR);

    ChatMessage* ptr2 = ChatMessage_Create();
    ChatMessage_SetText(ptr2, mess, 20);

    if(!ptr->head)
    {
        ptr->head = ptr2;
        ptr->tail = ptr2;
    }
    else
    {
        ChatMessage* tmp_ptr = NULL;
        for(tmp_ptr = ptr->head; tmp_ptr->next != NULL; tmp_ptr = tmp_ptr->next);
        tmp_ptr->next = ptr2;
        ptr2->prev = tmp_ptr;
        ptr->tail = ptr2;
    }

    if(ptr->max_nb_mess <= ptr->nb_mess)
    {
        ChatMessage* ptr3 = ptr->head;
        ptr->head = ptr->head->next;
        ChatMessage_DestroyFromList(ptr3);
    }
    else
        ptr->nb_mess++;

    ChatMessagesList_UpdateCoords(ptr);
    free_secure(mess);
}

void ChatMessagesList_Draw(ChatMessagesList* ptr, sfRenderWindow* Game)
{
    for (ChatMessage* i = ptr->head; i != NULL; i = i->next)
        sfRenderWindow_DrawString(Game, i->message);
}

void ChatMessagesList_UpdateCoords(ChatMessagesList* ptr)
{
    int cpt = 0;
    for(ChatMessage* i = ptr->head; i != NULL; i = i->next, cpt++)
        ChatMessage_SetCoords(i, 0, MARGIN_TOP + ChatMessage_GetSize(i) * cpt);
}

void ChatMessagesList_Destroy(ChatMessagesList* ptr)
{
    if (!ptr)
        logging_Error("ChatMessagesList_Destroy", "ChatMessageList sent NULL", NULL_PTR);

    ChatMessage* head = ptr->head, *temp_ptr = NULL;
    while(head != NULL)
    {
        temp_ptr = head->next;
        ChatMessage_Destroy(head);
        head = temp_ptr;
    }

    free_secure(ptr);
}
