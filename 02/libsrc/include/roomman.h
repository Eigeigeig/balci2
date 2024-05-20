/**
 * Copyright (C) 2017 by Arno Nuehm
 *
 * This software is distributed in the hope that it will be useful,
 * or at least interesting, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more details.
 */

#ifndef LIBROOMAN_H
#define LIBROOMAN_H

#include <stdlib.h>
#include <stdint.h>

/**
 * @file
 * @defgroup libfuncs Library functions
 * @brief   API-Definitions for the roomman library
 *
 * This file is to be included by all applications making use
 * of any of the room manager library's functions. It declares prototypes
 * for all user-callable functions of the library.
 * It __does not__ contain library-internal definitions.
 *
 */

/**
 * @mainpage
 *
 * @section Introduction
 *
 * roomman is an open source library implementing a simple room manager
 * for managing room reservations.
 *
 * @note All functions may return fatal errors (@ref ROOMMAN_ERR_FATAL).
 *       The API documentation per function only contains function specific errors.
 */

/**
 * @brief Global definitions
 */
/** The maximum length of a room name */
#define ROOMMAN_MAX_NAME (31U)

/** Operation successful */
#define ROOMMAN_SUCCESS             (0)
/** Invalid room number */
#define ROOMMAN_ERR_INVALID_ROOM    (-1)
/** Room ID is duplicate */
#define ROOMMAN_ERR_DUPLICATE_ROOM  (-2)
/** Provided name is too long */
#define ROOMMAN_ERR_NAMETOOLONG     (-3)
/** Room capacity exceeded or underflow detected */
#define ROOMMAN_ERR_EXCEED          (-4)
/** User name not set */
#define ROOMMAN_ERR_NOUSER          (-5)
/** Room is currently not empty */
#define ROOMMAN_ERR_NOT_EMPTY       (-6)
/** Room is already reserved */
#define ROOMMAN_ERR_RESERVED        (-7)


/** Fatal errors - may be returned by any function returning a @ref roomman_result_t
 * @{
 */
/** Mark the minimum error code to indicate a fatal error. */
#define ROOMMAN_ERR_FATAL           (-100)
/** Error while storing the room information in the database */
#define ROOMMAN_ERR_STORE           (-101)
/** Database corrupted */
#define ROOMMAN_ERR_CORRUPT         (-102)
/**
 * @}
 */



/**
 * Data type for room IDs
 */
typedef uint32_t roomman_id_t;

/**
 * Data type to indicate the success of an operation
 */
typedef int16_t roomman_result_t;

/**
 * @ingroup libfuncs
 * @brief Set the name of the current user (required for room reservations).
 *
 * @param[in] username  Name of the user.
 *
 * @return @ref ROOMMAN_SUCCESS on success
 * @return On error a negative value (either @ref ROOMMAN_ERR_NAMETOOLONG)
 */
roomman_result_t roomman_set_username(char *username);

/**
 * @ingroup libfuncs
 * @brief Create a new room
 *
 * @note Rooms are created empty and not reserved.
 *
 * @param[in] building_name The building's name
 * @param[in] room_name The room's name
 * @param[in] max_pers The maximum number of persons for this room. If 0 is provided the default capacity will be set.
 * @param[out] rid The ID of the newly created room. Undefined if room creation fails.
 *
 * @return @ref ROOMMAN_SUCCESS on success
 * @return On error a negative value (@ref ROOMMAN_ERR_DUPLICATE_ROOM)
 */
roomman_result_t roomman_create_room(char *building_name, char *room_name, uint16_t max_pers, roomman_id_t *rid);

/**
 * @ingroup libfuncs
 * @brief Delete a room
 *
 * @param[in] rid ID of the room to delete
 *
 * @return @ref ROOMMAN_SUCCESS  If the room was successfully deleted
 * @returns A negative value (@ref ROOMMAN_ERR_INVALID_ROOM)
 */
roomman_result_t roomman_delete_room(roomman_id_t rid);

/**
 * @ingroup libfuncs
 * @brief Update the capacity of a room
 *
 * @param[in] rid ID of the room to update
 * @param[in] new_capacity New capacity of the room
 *
 * @return @ref ROOMMAN_SUCCESS If the room was successfully updated
 * @return On error a negative value (@ref ROOMMAN_ERR_INVALID_ROOM)
 */
roomman_result_t roomman_update_capacity(roomman_id_t rid, uint16_t new_capacity);

/**
 * @ingroup libfuncs
 * @brief Reserve a room
 *
 * @note In order to reserve a room, the user must be set
 *       (@ref roomman_set_username).
 *
 * @param[in] rid The ID of the room to be reserved
 *
 * @return @ref ROOMMAN_SUCCESS If the room was successfully updated
 * @return On error a negative value (either @ref ROOMMAN_ERR_NOUSER or
 *          @ref ROOMMAN_ERR_INVALID_ROOM or @ref ROOMMAN_ERR_NOT_EMPTY)
 */
roomman_result_t roomman_reserve_room(roomman_id_t rid);

/**
 * @ingroup libfuncs
 * @brief Reserve a room
 *
 * @note Clearing the reservation for an existing room should always succeed
 *       (unless errors with the database access occur).
 *
 * @param[in] rid The ID of the room to be unreserved
 *
 * @return @ref ROOMMAN_SUCCESS If the room was successfully updated
 * @return On error a negative value (@ref ROOMMAN_ERR_INVALID_ROOM)
 */
roomman_result_t roomman_clear_reservation(roomman_id_t rid);

/**
 * @ingroup libfuncs
 * @brief Increase the number of persons currently using the room.
 *
 * @param[in] rid ID of the room to update
 * @param[in] persons Number of additional persons using the room
 *
 * @return @ref ROOMMAN_SUCCESS If the room was successfully updated
 * @return On error a negative value (either @ref ROOMMAN_ERR_INVALID_ROOM or @ref ROOMMAN_ERR_EXCEED)
 */
roomman_result_t roomman_add_persons(roomman_id_t rid, uint16_t persons);

/**
 * @ingroup libfuncs
 * @brief Decrease the number of persons currently using the room.
 *
 * @param[in] rid ID of the room to update
 * @param[in] persons Number of persons leaving the room
 *
 * @return @ref ROOMMAN_SUCCESS If the room was successfully updated
 * @return On error a negative value (either @ref ROOMMAN_ERR_INVALID_ROOM or @ref ROOMMAN_ERR_EXCEED)
 */
roomman_result_t roomman_remove_persons(roomman_id_t rid, uint16_t persons);

/**
 * @ingroup libfuncs
 * @brief   Get contents of an entry in the database
 *
 * This function delivers details of a room identified by UID. The
 * room is __not__ changed by a call to this function.
 *
 * @returns @ref ROOMMAN_SUCCESS if the entry for the given room has been found
 * @returns a negative value if the room was not found (@ref ROOMMAN_ERR_INVALID_ROOM)
 */
roomman_result_t roomman_readentry(
    roomman_id_t rid,   /**< [in] Room's unique ID             */
    char* building,     /**< [out] Where to store name of building*/
    char* name,         /**< [out] Where to store name of room */
    uint16_t* occupied, /**< [out] Where to store the number of occupied places */
    uint16_t* capacity, /**< [out] Where to store capacity */
    char *reserved_by   /**< [out] Contains the name of the person currently reserving the room (if any) */
    );


/**
 * @ingroup libfuncs
 * @brief   Find room in the database
 *
 * This function locates an entry in the database. It returns a unique ID by
 * which the room can be referenced. A room is uniquely identified by the
 * combination of the building's name and the room's name.
 *
 * @return @ref ROOMMAN_SUCCESS on success
 * @return An error code otherwise @ref ROOMMAN_ERR_INVALID_ROOM or @ref ROOMMAN_ERR_NAMETOOLONG)
 */
roomman_result_t roomman_lookup(
    const char* building,       /**< [in] name of building */
    const char* room_name,      /**< [in] name of room */
    roomman_id_t *rid           /**< [out] The room ID for the requested room.
                                     (Undefined if room is not found.) */
    );

/**
 * @ingroup libfuncs
 * @brief   Get a directory of entries in the database
 *
 * This function enables a "wildcard search" of the database. It delivers
 * UIDs of matching entries. The caller may specify a building name or a
 * room name, thus specifying a particular entry (in this case the
 * function is similar to roomman_lookup()). However, it is also possible
 * to specify __only__ an building name or __only__ a room name by passing
 * a NULL value for the name that should not be specified. In this
 * case, the function delivers UIDs of __all__ entries matching the
 * specified name. If both names are passed as NULL, the function delivers
 * the UIDs of __all__ entries in the database.
 *
 * The function delivers __one__ __UID__ __per__ __call__. In order to
 * keep the context across calls, the caller must pass a pointer to an
 * int variable. This variable (__not__ the pointer!) must be initialized
 * with -1 before making the first call.
 *
 * If there are no more remaining entries in the database the function will
 * return @ref ROOMMAN_ERR_INVALID_ROOM.
 *
 * @return @ref ROOMMAN_SUCCESS on success
 * @return An error code otherwise @ref ROOMMAN_ERR_INVALID_ROOM or @ref ROOMMAN_ERR_NAMETOOLONG)
 */
roomman_result_t roomman_directory(
    int32_t* fdptr,         /**< [in/out] pointer where to store the open file handle across calls */
    const char* building,   /**< [in] name of room's building or NULL if not specified */
    const char* room_name,  /**< [in] name of room or NULL if not specified        */

    roomman_id_t *rid       /**< [out] The room ID for the next matching entry.
                                 (Undefined if room is not found.) */
    );

#endif /* LIBROOMAN_H */
