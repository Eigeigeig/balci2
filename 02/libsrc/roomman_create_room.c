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
 * @brief	Init function for room manager library
 */

#include <stdbool.h>

#include "include/roomman.h"
#include "roommandef.h"

/**
 * @brief	Local helper function to compute a hash value from two strings
 */
static roomman_id_t make_hash(const char *s1, const char *s2)
{
	unsigned int hash;
	size_t i;
	char key[2*(ROOMMAN_MAX_NAME+1)];

	/* copy both strings into a single buffer */
	memset(key, 0, sizeof(key));
	strncpy(&key[0], s1, sizeof(key));
	strncpy(&key[ROOMMAN_MAX_NAME+1], s2, sizeof(key)-(ROOMMAN_MAX_NAME+1));

	/* compute a hash value over the buffer's contents */
	hash = INITIAL_VALUE;
	for(i = 0; i < 2*(ROOMMAN_MAX_NAME+1); ++i)
		hash = M * hash + key[i];

	/* make sure always get a >= 0 number */
	return hash;
}


roomman_result_t roomman_create_room(char *building_name, char *room_name, uint16_t max_pers, roomman_id_t *rid)
{
    ssize_t ret;
    roomman_t rm;
    memset(&rm, 0, sizeof(roomman_t));


	/* check if name lengths are in range */
	if(strlen(building_name) > ROOMMAN_MAX_NAME ||
	   strlen(room_name) > ROOMMAN_MAX_NAME)
	{
		return ROOMMAN_ERR_NAMETOOLONG;
	}

	/* open the data file for writing (create if necessary) */
	int fd = open(DATAFILE, O_RDWR|O_CREAT, 0600);
	if(fd < 0)
	{
		return ROOMMAN_ERR_STORE;
	}
	/*
	 *  before adding a new entry, make sure there isn't one
	 *  with the same building and name already!
	 */
	while((ret = read(fd, &rm, sizeof(rm))) == sizeof(rm))
	{
		if(!strcmp(rm.building, building_name) &&
		   !strcmp(rm.name, room_name))
		{
			close(fd);
			return ROOMMAN_ERR_DUPLICATE_ROOM;
		}
	}
	/*
	 * since the file always contains an integer number of structs
	 * with constant size, read() can legally return only sizeof(rm)
	 * (dealt with above) or zero at end-of-file. read() returning < 0
	 * indicates a severe error:
	 */
	if(ret < 0)
	{
		close(fd);
		return ROOMMAN_ERR_STORE;
	}
	/*
	 * if read() returns > 0, but < sizeof(h), it means that the file's
	 * size is not a multiple of sizeof(h), which must never happen.
	 * If it does, we conclude that the file must be damaged.
	 */
	else if(ret > 0)
	{
		close(fd);
		return ROOMMAN_ERR_CORRUPT;
	}
	/*
	 * Build a new entry and append it to the file:
	 */

	rm.rid = make_hash(building_name, room_name);
	strcpy(rm.building, building_name);
	strcpy(rm.name, room_name);
    rm.occupied = 0;
    rm.capacity = ((max_pers > 0) ? max_pers : ROOMMAN_CAPACITY);
    memset(rm.reserved_by, 0, sizeof(rm.reserved_by));

	if((ret = write(fd, &rm, sizeof(rm))) != sizeof(rm))
	{
		close(fd);
		return ROOMMAN_ERR_STORE;
	}
	close(fd);

    *rid = rm.rid;

	return ROOMMAN_SUCCESS;
}

