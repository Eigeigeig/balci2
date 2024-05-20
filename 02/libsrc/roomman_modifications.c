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
 * @brief	Manage room reservations
 *
 *
 */

#include <stdint.h>
#include <stdbool.h>

#include "include/roomman.h"
#include "roommandef.h"


roomman_result_t _update_room(
                     roomman_id_t rid,	/**< Room's unique ID */
                     bool set_occupied,
                     int32_t occupied_delta,
                     bool set_capacity,
                     uint16_t capacity,
                     bool set_reserved,
                     char *reserved_by
                    )
{
    int fd;
    int ret;
    roomman_t rm;

    fd = open(DATAFILE, O_RDWR);
    if(fd < 0)
    {
        return ROOMMAN_ERR_INVALID_ROOM;
    }
    /* read file entries one by one */
    while((ret = read(fd, &rm, sizeof(rm))) == sizeof(rm))
    {
        if(rm.rid == rid)
        {
            if (set_capacity)
            {
                rm.capacity = capacity;
            }
            if (set_occupied)
            {
                uint16_t new_occupied = (rm.occupied + occupied_delta);
                if ((new_occupied > rm.capacity) || (new_occupied < 0)) {
                    close(fd);
                    return ROOMMAN_ERR_EXCEED;
                }
                rm.occupied = new_occupied;
            }
            if (set_reserved)
            {
                if (reserved_by == NULL)
                {
                    memset(rm.reserved_by, 0, sizeof(rm.reserved_by));
                }
                else
                {
                    if (rm.occupied != 0)
                    {
                        return ROOMMAN_ERR_NOT_EMPTY;
                    }
                    else if (strlen(rm.reserved_by) != 0)
                    {
                        return ROOMMAN_ERR_RESERVED;
                    }
                    else
                    {
                        strcpy(rm.reserved_by, reserved_by);
                    }
                }
            }

            /* write entry (with modifications) back to file: */
            if(lseek(fd, -sizeof(rm), SEEK_CUR) ==(off_t)-1)
            {
                close(fd);
                return ROOMMAN_ERR_STORE;
            }

            if(write(fd, &rm, sizeof(rm)) != sizeof(rm))
            {
                close(fd);
                return ROOMMAN_ERR_STORE;
            }
            close(fd);
            return ROOMMAN_SUCCESS;
        }
    }
    return ROOMMAN_ERR_INVALID_ROOM;
}

roomman_result_t roomman_update_capacity(roomman_id_t rid, uint16_t new_capacity)
{
    return _update_room(rid, false, 0, true, new_capacity, false, NULL);
}

roomman_result_t roomman_add_persons(roomman_id_t rid, uint16_t persons)
{
    return _update_room(rid, true, persons, false, 0, false, NULL);
}

roomman_result_t roomman_remove_persons(roomman_id_t rid, uint16_t persons)
{
    return _update_room(rid, true, ((-1) * persons), false, 0, false, NULL);
}

roomman_result_t roomman_reserve_room(roomman_id_t rid)
{
    if (strlen(roomman_current_user) < 1)
    {
        return ROOMMAN_ERR_NOUSER;
    }
    return _update_room(rid, false, 0, false, 0, true, roomman_current_user);
}


roomman_result_t roomman_clear_reservation(roomman_id_t rid)
{
    return _update_room(rid, false, 0, false, 0, true, NULL);
}

