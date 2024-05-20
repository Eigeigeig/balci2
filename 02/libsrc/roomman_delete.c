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
 * @brief	Delete function for room manager library
 */

#include "include/roomman.h"
#include "roommandef.h"

/**
 * @brief	Delete a room from the database
 *
 */
roomman_result_t roomman_delete_room(roomman_id_t rid)
{
    int fd, fdi;
    ssize_t ret;
    int found = 0;
    roomman_t rm;

    if(rename(DATAFILE, INTERMEDIATE_FILE) < 0)
    {
        return ROOMMAN_ERR_STORE;
    }

    fdi = open(INTERMEDIATE_FILE, O_RDONLY);
    if(fdi < 0)
    {
        return ROOMMAN_ERR_STORE;
    }

    fd = open(DATAFILE, O_RDWR|O_CREAT, 0600);
    if(fd < 0)
    {
        return ROOMMAN_ERR_STORE;
    }

    /* read file entries from intermediate file one by one */
    while((ret = read(fdi, &rm, sizeof(rm))) == sizeof(rm))
    {
        if (rm.rid == rid)
        {	/*
             * Found a matching entry
             */
            found = 1;
        }
        else
        {	/* other room: write it to new file */
            if(write(fd, &rm, sizeof(rm)) != sizeof(rm))
            {
                close(fd);
                close(fdi);
                return ROOMMAN_ERR_STORE;
            }
        }
    }
    close(fd);
    close(fdi);
    if(ret < 0)
    {
        return ROOMMAN_ERR_STORE;
    }
    else if(ret > 0)
    {
        return ROOMMAN_ERR_CORRUPT;
    }
    /* delete intermediate file */
    if(unlink(INTERMEDIATE_FILE) < 0)
    {
        return ROOMMAN_ERR_STORE;
    }

    roomman_result_t result = ROOMMAN_ERR_CORRUPT;

    if(found)
    {
        result = ROOMMAN_SUCCESS;
    }
    else
    {
        result = ROOMMAN_ERR_INVALID_ROOM;
    }

    return result;
}
