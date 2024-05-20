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
 * @brief	Lookup function for room manager library
 *
 *
 */


#include "include/roomman.h"
#include "roommandef.h"

roomman_result_t roomman_lookup(
    const char* building,       /**< [in] name of building */
    const char* room_name,      /**< [in] name of room */
    roomman_id_t *rid           /**< [out] The room ID for the requested room. */
	)
{
	int fd;
	int ret;
	roomman_t rm;

	/* check if name lengths are in range */
	if(strlen(building) > ROOMMAN_MAX_NAME ||
	   strlen(room_name) > ROOMMAN_MAX_NAME)
	{
		return ROOMMAN_ERR_NAMETOOLONG;
	}

	fd = open(DATAFILE, O_RDONLY);
	if(fd < 0)
	{
		return ROOMMAN_ERR_INVALID_ROOM;
	}
	/* read file entries one by one */
	while((ret = read(fd, &rm, sizeof(rm))) == sizeof(rm))
	{
		if(!strcmp(rm.building, building) &&
		   !strcmp(rm.name, room_name))
		{	/* found matching room! */
			close(fd);
			*rid = rm.rid;
            return ROOMMAN_SUCCESS;
		}
	}
	close(fd);
	if(ret == 0)

	{
		return ROOMMAN_ERR_INVALID_ROOM;
	}
	/*
	 * if read() returns > 0, but < sizeof(h), it means that the file's
	 * size is not a multiple of sizeof(h), which must never happen.
	 * If it does, we conclude that the file must be damaged.
	 */
	else
	{
		return ROOMMAN_ERR_CORRUPT;
	}
}
