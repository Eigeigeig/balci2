
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>

#include "roomman.h"

/**
 * @brief Print usage information
 */

//////////////////////





 ////////////////

static void rtfm(char* argv[])
{
	printf("Usage: %s {<Option>} <param1> {<param2>}\n", argv[0]);
	printf("Function: Room management\n");
	printf("Options:\n");
	printf("     -l {<building>}                     - show current list of roomnos\n");
	printf("     -n <building> <roomno> [<capacity>] - add new room\n");
	printf("     -o <building> <roomno> <occupied>   - update the number of persons currently occupying the room\n");
	printf("     -c <building> <roomno> <capacity>   - update the capacity of the room\n");
	printf("     -t <building> <roomno>              - toggle room reservation\n");
	printf("     -d <building> <roomno>              - delete a room\n");
}


/**
 * @brief Main program
 */
///////////////////////////////////

/* static void print_room_entries(int32_t fdptr) {
    roomman_id_t rid;
    char building[256], room_name[256], reserved_by[256];
    uint16_t occupied, capacity;

    printf("Building\t\t Name\t\t Occupied  \tCapacity\t Reserved\n");
    printf("-------------------------------------------------------------------------------------\n");

    // Loop through all room entries in the directory
    while (roomman_directory(&fdptr, NULL, NULL, &rid) == ROOMMAN_SUCCESS) {
        // Read entry details
        roomman_readentry(rid, building, room_name, &occupied, &capacity, reserved_by);


      if (reserved_by[0] == '\0')
    {
        strcpy(reserved_by, "False");
    }
    else
    {
        strcpy(reserved_by, "True");
    }

        // Print room details
        printf("%s \t\t\t %s \t\t  %d  \t\t%d\t\t %s\n", building, room_name, occupied, capacity, reserved_by);

    }
} */

/////////////////////////////

 static char GetRoomOccupied(char* building, char* roomno)
{
    // Initial Values
    char building_name[50];
    char room_name[50];
    uint16_t occupied;
    uint16_t capacity;
    char reserved_by[50];

    roomman_id_t rid;
    roomman_result_t lookup_result = roomman_lookup(building, roomno, &rid);
    if (lookup_result != ROOMMAN_SUCCESS)
    {
        printf("Error: Room not found or lookup failed\n");
        exit(EXIT_FAILURE);
    }

    // Read the room entry to get its capacity
    roomman_result_t read_result = roomman_readentry(rid, building_name, room_name, &occupied, &capacity, reserved_by);
    if (read_result != ROOMMAN_SUCCESS)
    {
        printf("Error: Failed to read room entry\n");
        exit(EXIT_FAILURE);
    }



    return occupied;
}


////////////////////////////////

static roomman_id_t GetRoomID(char* building, char* roomno)
{
    // Initial Values
    char building_name[50];
    char room_name[50];
    uint16_t occupied;
    uint16_t capacity;
    char reserved_by[50];

    roomman_id_t rid;
    roomman_result_t lookup_result = roomman_lookup(building, roomno, &rid);
    if (lookup_result != ROOMMAN_SUCCESS)
    {
        printf("Error: Room not found or lookup failed\n");
        exit(EXIT_FAILURE);
    }

    // Read the room entry to get its capacity
    roomman_result_t read_result = roomman_readentry(rid, building_name, room_name, &occupied, &capacity, reserved_by);
    if (read_result != ROOMMAN_SUCCESS)
    {
        printf("Error: Failed to read room entry\n");
        exit(EXIT_FAILURE);
    }



    return rid;
}


////////////////////////////
void handle_add_room(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
        printf("Error: Incorrect number of arguments for -n option\n");
        rtfm(argv);
        exit(EXIT_FAILURE);
    }

    char* building = argv[2];
    char* roomno = argv[3];

    if (!isdigit(*roomno)) {
        printf("not a number%s\n", roomno);
        exit(EXIT_FAILURE);
    }

    uint16_t capacity = (argc == 5) ? atoi(argv[4]) : 0; // Convert capacity string to integer
    roomman_id_t rid;

    roomman_result_t results = roomman_create_room(building, roomno, capacity, &rid);
    if (results == ROOMMAN_SUCCESS) {
        printf("New room added successfully: Building %s, Room %s\n", building, roomno);
    } else {
        printf("Error: Failed to add new room\n\n");
        rtfm(argv);
        exit(EXIT_FAILURE);
    }
}
//////////////////////
/*
void handle_list_rooms(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        printf("Error: Incorrect number of arguments for -l option\n");
        rtfm(argv);
        exit(EXIT_FAILURE);
    }

    char* building = (argc == 3) ? argv[2] : NULL;
    int32_t fdptr = -1;
    roomman_id_t rid;
    roomman_result_t results = roomman_directory(&fdptr, building, NULL, &rid);

    if (results == ROOMMAN_SUCCESS) {
        printf("Rooms in building %s:\n", building);
        printf("--------------------\n");
        print_room_entries(fdptr);
    } else {
        printf("Error: Failed to list rooms\n\n");
        rtfm(argv);
        exit(EXIT_FAILURE);
    }
} */

void handle_list_rooms(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        printf("Error: Incorrect number of arguments for -l option\n");
        rtfm(argv);
        exit(EXIT_FAILURE);
    }

    char* building = (argc == 3) ? argv[2] : NULL;
    int32_t fdptr = -1;
    roomman_id_t rid;
    roomman_result_t results = roomman_directory(&fdptr, building, NULL, &rid);

    if (results == ROOMMAN_SUCCESS) {
        printf("Rooms in building %s:\n", building);
        printf("--------------------\n");

        // Print header
        printf("Building\t\t Name\t\t Occupied  \tCapacity\t Reserved\n");
        printf("-------------------------------------------------------------------------------------\n");

        // Loop through all room entries in the directory
        while (roomman_directory(&fdptr, NULL, NULL, &rid) == ROOMMAN_SUCCESS) {
            // Read entry details
            char room_building[256], room_name[256], reserved_by[256];
            uint16_t occupied, capacity;
            roomman_readentry(rid, room_building, room_name, &occupied, &capacity, reserved_by);

            if (reserved_by[0] == '\0') {
                strcpy(reserved_by, "False");
            } else {
                strcpy(reserved_by, "True");
            }

            // Print room details
            printf("%s \t\t\t %s \t\t  %d  \t\t%d\t\t %s\n", room_building, room_name, occupied, capacity, reserved_by);
        }
    } else {
        printf("Error: Failed to list rooms\n\n");
        rtfm(argv);
        exit(EXIT_FAILURE);
    }
}



////////////////////////


void handle_update_occupancy(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
        printf("Error: Incorrect number of arguments for -o option\n");
        rtfm(argv);
        exit(EXIT_FAILURE);
    }

    char* building = argv[2];
    char* roomno = argv[3];
    uint16_t persons = (argc == 5) ? atoi(argv[4]) : 0;

    roomman_id_t rid;
    roomman_result_t results = roomman_lookup(building, roomno, &rid);
    if (results != ROOMMAN_SUCCESS) {
        printf("Error: Room not found or lookup failed\n");
        exit(EXIT_FAILURE);
    }

    uint16_t occupied = GetRoomOccupied(building, roomno);

    if (persons > occupied) {
        uint16_t new_persons = persons - occupied;
        results = roomman_add_persons(rid, new_persons);
    } else if (persons < occupied) {
        uint16_t new_persons = occupied - persons;
        results = roomman_remove_persons(rid, new_persons);
    }

    if (results == ROOMMAN_SUCCESS) {
        printf("Updated numbers of persons occupying room %s in Building %s to %d\n", roomno, building, persons);
    } else {
        printf("Error");
        exit(EXIT_FAILURE);
    }
}

/*
void handle_update_occupancy(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
        printf("Error: Incorrect number of arguments for -o option\n");
        rtfm(argv);
        exit(EXIT_FAILURE);
    }

    char* building = argv[2];
    char* roomno = argv[3];
    uint16_t persons = (argc == 5) ? atoi(argv[4]) : 0;

    // Deðiþkenler
    char building_name[50];
    char room_name[50];
    uint16_t occupied;
    uint16_t capacity;
    char reserved_by[50];

    roomman_id_t rid;
    roomman_result_t lookup_result = roomman_lookup(building, roomno, &rid);
    if (lookup_result != ROOMMAN_SUCCESS) {
        printf("Error: Room not found or lookup failed\n");
        exit(EXIT_FAILURE);
    }

    // Oda giriþini oku
    roomman_result_t read_result = roomman_readentry(rid, building_name, room_name, &occupied, &capacity, reserved_by);
    if (read_result != ROOMMAN_SUCCESS) {
        printf("Error: Failed to read room entry\n");
        exit(EXIT_FAILURE);
    }

    // Yeni kiþi sayýsýný belirle
    uint16_t new_occupied;
    if (persons > occupied) {
        new_occupied = occupied + (persons - occupied);
    } else if (persons < occupied) {
        new_occupied = occupied - (occupied - persons);
    } else {
        new_occupied = occupied;
    }

    // Kiþi sayýsýný güncelle
    roomman_result_t update_result = roomman_update_capacity(rid, new_occupied);
    if (update_result == ROOMMAN_SUCCESS) {
        printf("Updated number of persons occupying room %s in Building %s to %d\n", roomno, building, new_occupied);
    } else {
        printf("Error: Failed to update occupancy\n");
        exit(EXIT_FAILURE);
    }
}  */



//////////////////////////

void handle_update_capacity(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
        printf("Error: Incorrect number of arguments for -c option\n");
        rtfm(argv);
        exit(EXIT_FAILURE);
    }

    char* building = argv[2];
    char* roomno = argv[3];
    uint16_t new_capacity = atoi(argv[4]);

    roomman_id_t rid;
    roomman_result_t lookup_result = roomman_lookup(building, roomno, &rid);
    if (lookup_result != ROOMMAN_SUCCESS) {
        printf("Error: Failed to find the room\n");
        exit(EXIT_FAILURE);
    }

    roomman_result_t results = roomman_update_capacity(rid, new_capacity);
    if (results == ROOMMAN_SUCCESS) {
        printf("Occupancy updated successfully for Room %s in Building %s\n", roomno, building);
    } else {
        printf("Error");
        exit(EXIT_FAILURE);
    }
}
///////////////////

void handle_toggle_reservation(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        printf("Error: Incorrect number of arguments for -t option\n");
        rtfm(argv);
        exit(EXIT_FAILURE);
    }

    char* building = argv[2];
    char* roomno = argv[3];

    roomman_id_t rid;
    roomman_result_t lookup_result = roomman_lookup(building, roomno, &rid);
    if (lookup_result != ROOMMAN_SUCCESS) {
        printf("Error: Failed to find the room\n");
        exit(EXIT_FAILURE);
    }

    roomman_result_t results = roomman_reserve_room(rid);
    if (results == ROOMMAN_ERR_RESERVED) {
        results = roomman_clear_reservation(rid);
    } else if (results == ROOMMAN_ERR_NOT_EMPTY) {
        printf("Could not reserve non-empty room %s in Building %s\n", roomno, building);
        exit(EXIT_FAILURE);
    }

    if (results == ROOMMAN_SUCCESS) {
        printf("Reserved room %s in Building %s\n", roomno, building);
    } else {
        printf("Error");
        exit(EXIT_FAILURE);
    }
}

////////////////////////

void handle_delete_room(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Error: Incorrect number of arguments for -d option\n");
        rtfm(argv);
        exit(EXIT_FAILURE);
    }

    char* building = argv[2];
    char* roomno = argv[3];

    roomman_id_t rid;
    roomman_result_t lookup_result = roomman_lookup(building, roomno, &rid);
    if (lookup_result != ROOMMAN_SUCCESS) {
        printf("Error: Failed to find the room\n");
        exit(EXIT_FAILURE);
    }

    roomman_result_t results = roomman_delete_room(rid);
    if (results == ROOMMAN_SUCCESS) {
        printf("Room %s in Building %s deleted\n", roomno, building);
    } else {
        printf("Error");
        exit(EXIT_FAILURE);
    }
}


////////////////////////




 int main(int argc, char* argv[])
{

  if (argc == 1) {
        rtfm(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];

    switch (option) {
        case 'n':
            handle_add_room(argc, argv);
            break;
        case 'l':
            handle_list_rooms(argc, argv);
            break;
        case 'o':
            handle_update_occupancy(argc, argv);
            break;
        case 'c':
            handle_update_capacity(argc, argv);
            break;
        case 't':
            handle_toggle_reservation(argc, argv);
            break;
        case 'd':
            handle_delete_room(argc, argv);
            break;
        default:
            printf("Error: Unknown option\n");
            rtfm(argv);
            exit(EXIT_FAILURE);
    }

    return 0;
}






