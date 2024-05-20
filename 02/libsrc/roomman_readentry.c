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
 * @brief	Read entry function for room manager library
 */

#include "roommandef.h"

/**
 * @brief	Get contents of an entry in the database
 *
 */
roomman_result_t roomman_readentry(
    roomman_id_t rid,   /**< [in] Room's unique ID             */
    char* building,     /**< [out] Where to store name of building*/
    char* name,         /**< [out] Where to store name of room */
    uint16_t* occupied, /**< [out] Where to store the number of occupied places */
    uint16_t* capacity, /**< [out] Where to store capacity */
    char *reserved_by   /**< [out] Contains the name of the person currently reserving the room (if any) */
	)
{
	int fd;
	int ret;
	roomman_t rm;

	fd = open(DATAFILE, O_RDONLY);
	if(fd < 0)
	{
		return ROOMMAN_ERR_INVALID_ROOM;
	}
	/* read file entries one by one */
	while((ret = read(fd, &rm, sizeof(rm))) == sizeof(rm))
	{
		if(rm.rid == rid)
		{
            /* found matching room! */
            if (building != NULL)
            {
                strcpy(building, rm.building);
            }
            if (name != NULL)
            {
                strcpy(name, rm.name);
            }
            if (occupied != NULL)
            {
                *occupied = rm.occupied;
            }
            if (capacity != NULL)
            {
                *capacity = rm.capacity;
            }
            if (capacity != NULL)
            {
                strcpy(reserved_by, rm.reserved_by);
            }
			close(fd);
			return ROOMMAN_SUCCESS;
		}
	}
	close(fd);
	return ROOMMAN_ERR_INVALID_ROOM;
}
