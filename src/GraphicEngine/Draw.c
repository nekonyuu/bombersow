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

#include <SFML/Graphics.h>

#include "BaseSystem/Config.h"
#include "BaseSystem/Logging.h"
#include "GraphicEngine/Draw.h"

void GraphicEngine_Init(sfRenderWindow* Game, Config* config)
{
    sfRenderWindow_PreserveOpenGLStates(Game, sfTrue);
    sfRenderWindow_SetActive(Game, sfTrue);

    /* --- Initialisation OpenGL --- */

    // Initialisation des valeurs d'effacement pour les tampons de couleur et de profondeur
    glClearDepth(1.f);
    glClearColor(255, 255, 255, 0);

    // Activation de la lecture et de l'écriture dans le tampon de profondeur
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Viewport configuré sur toute la fenêtre
    glViewport(0, 0, config->width, config->height);

    // Projection orthogonale
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, config->width, 0, config->height);

    /* ---------- END INIT ---------- */
}

void GraphicEngine_ClearScreen(sfRenderWindow* Game)
{
    sfRenderWindow_SetActive(Game, sfTrue);
    sfRenderWindow_Clear(Game, sfWhite);

    glClearColor(255, 255, 255, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
