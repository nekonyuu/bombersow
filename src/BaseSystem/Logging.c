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

#include <stdio.h>
#include <stdlib.h>
#include "BaseSystem/Logging.h"

inline void logging_Warning(char* fct_name, char* text)
{
    printf("Warning - %s : %s\n", fct_name, text);
}

inline void logging_Error(char* fct_name, char* text)
{
    printf("Error - %s : %s\n", fct_name, text);
    exit(1);
}

inline void logging_Info(char* fct_name, char* text)
{
#ifdef DEBUG_MODE
    printf("Debug Info - %s : %s\n", fct_name, text);
#endif
}

inline void free_secure(void* ptr)
{
    free(ptr);
    ptr = NULL;
}

inline void logging_FPSShow(sfRenderWindow* App)
{
    sfString* fps_text = sfString_Create();
    sfString_SetSize(fps_text, 14);
    sfString_SetPosition(fps_text, 0, 0);
    char* tmp_fps = malloc(20 * sizeof(char));

    sprintf(tmp_fps, "%d", (int) (1.0f/sfRenderWindow_GetFrameTime(App)));
    sfString_SetText(fps_text, tmp_fps);
    sfRenderWindow_DrawString(App, fps_text);

    free_secure(tmp_fps);
    sfString_Destroy(fps_text);
}
