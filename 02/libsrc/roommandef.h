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
 * @brief   Library-internal definitions
 *
 * This file contains definitions which are global within the roomman
 * library but are not used (and should not be visible) to users of the
 * library.
 *
 */

#ifndef __ROOMMANDEF_H__
#define __ROOMMANDEF_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>

#include "include/roomman.h"

#define DATAFILE            "roommanfile.dat"
#define INTERMEDIATE_FILE   "roommanfile.bak"

/*
 * Hash parameters according to  Bernstein's function
 * See http://www.strchr.com/hash_functions
 */
#define INITIAL_VALUE       (5381)
#define M                   (33)

#define ROOMMAN_CAPACITY    (10)  /* Default room capacity */

/**
 * @brief Structure of an entry in the database
 */

typedef struct roomman
{
    roomman_id_t rid;                       /**< Unique ID                 */
    char building[ROOMMAN_MAX_NAME+1];      /**< Name of the building */
    char name[ROOMMAN_MAX_NAME+1];          /**< Name of the room */
    uint16_t  occupied;                     /**< currently occupied seats */
    uint16_t  capacity;                     /**< room capacity */
    char reserved_by[ROOMMAN_MAX_NAME+1];   /**< Name of the reserving person; can be empty */
} roomman_t;

/**
 * The name of the currently configured user.
 */
extern char roomman_current_user[ROOMMAN_MAX_NAME+1];

/**
 * @brief Debug support macros
 *
 * Output messages if @c verbose is higher than a given level.
 *
 * NOTE: Usage @c debug((<printf-params>)); i.e. use @e two brackets
 *       to enclose the printf parameter list!
 */
#define debug1(x) do{if(verbose >= 1){ printf x ;fflush(stdout);}}while(0)
#define debug2(x) do{if(verbose >= 2){ printf x ;fflush(stdout);}}while(0)
#define debug3(x) do{if(verbose >= 3){ printf x ;fflush(stdout);}}while(0)
#define debug4(x) do{if(verbose >= 4){ printf x ;fflush(stdout);}}while(0)





#endif /* __ROOMMANDEF_H__ */
