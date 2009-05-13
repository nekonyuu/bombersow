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

#ifndef LOGGING_H
#define LOGGING_H

#include <assert.h>

// Sécurisation du free() (Evite les crash sur un free déjà effectué)
//#define free_secure(pointer) { free(pointer); pointer = NULL; }

#ifdef DEBUG_MODE
#include "Memleak/halloc.h"
#endif

void logging_Warning(char*, char*);
void logging_Error(char*, char*);
void logging_Info(char*, char*);
inline void free_secure(void*);

#endif
