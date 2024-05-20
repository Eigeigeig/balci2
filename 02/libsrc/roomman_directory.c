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
 * @brief   Directory function for roomman library
 */


#include "roommandef.h"

/**
 * @brief   Get a directory of entries in the database
 *
 */
roomman_result_t roomman_directory(
    int32_t* fdptr, /**< [in/out] pointer where to store the open file handle across calls */
    const char* building_name,      /**< [in] name of room's building or NULL if not specified */
    const char* room_name,  /**< [in] name of room or NULL if not specified        */
    roomman_id_t *rid       /**< [out] The room ID for the next matching entry. */
    )
{
    int ret;
    roomman_t rm;

    /* check if name lengths are in range */
    if((building_name && strlen(building_name) > ROOMMAN_MAX_NAME) ||
       (room_name && strlen(room_name) > ROOMMAN_MAX_NAME))
    {
        return ROOMMAN_ERR_NAMETOOLONG;
    }

    if(*fdptr < 0)
    {
        *fdptr = open(DATAFILE, O_RDONLY);
        if(*fdptr < 0)
        {
            return ROOMMAN_ERR_INVALID_ROOM;
        }
    }

    /* read file entries one by one */
    while((ret = read(*fdptr, &rm, sizeof(rm))) == sizeof(rm))
    {
        if((!building_name || !strcmp(rm.building, building_name)) &&
           (!room_name || !strcmp(rm.name, room_name)))
        {   /* found matching room! */
            *rid = rm.rid;
            return ROOMMAN_SUCCESS;
        }
    }
    close(*fdptr);
    *fdptr = -1;
    if(ret == 0)
    {
        return ROOMMAN_ERR_INVALID_ROOM;
    }
    /*
     * if read() returns > 0, but < sizeof(rm), it means that the file's
     * size is not a multiple of sizeof(rm), which must never happen.
     * If it does, we conclude that the file must be damaged.
     */
    else
    {
        return ROOMMAN_ERR_CORRUPT;
    }
}
