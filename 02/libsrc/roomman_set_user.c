/**
 * Copyright (C) 2017 by Arno Nuehm
 *
 * This software is distributed in the hope that it will be useful,
 * or at least interesting, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more details.
 */

/**
 * @file
 * @brief	Set the user function for room manager library
 *
 *
 */

#include "include/roomman.h"
#include "roommandef.h"

char roomman_current_user[ROOMMAN_MAX_NAME+1];

roomman_result_t roomman_set_username(char *username)
{
    if (strlen(username) > ROOMMAN_MAX_NAME)
    {
        return ROOMMAN_ERR_NAMETOOLONG;
    }
    strcpy(roomman_current_user, username);
    return ROOMMAN_SUCCESS;
}
