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

#ifndef CONFIG_H
#define CONFIG_H

// Index des binds dans le tableau de config
enum BINDINGS
{
    UP_KEY,
    DOWN_KEY,
    LEFT_KEY,
    RIGHT_KEY,
    FIRE_KEY,
    JETPACK_MODE_KEY,
    CB_KEY,
    RL_KEY,
    GL_KEY,
    SG_KEY,
    MG_KEY,
    SNP_KEY,
    LG_KEY
};

// Structure de config
typedef struct CONFIG_ARRAY
{
    int width;
    int height;
    char* pseudo;
    //sfKeyCode* key_bindings;
} Config;

#endif
