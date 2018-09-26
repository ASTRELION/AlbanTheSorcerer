#ifndef DUNGEONMANAGER_H
# define DUNGEONMANAGER_H

#include <stdint.h>
#include "colors.h"

#define DNGN_SIZE_Y 21 // Max dungeon height
#define DNGN_SIZE_X 80 // Max dungeon width
#define DNGN_EDGE C_WHITE "-" C_RESET // Dungeon edge char
#define DNGN_ROCK " " // Dungeon rock char
#define DNGN_ROOM C_GREEN "." C_RESET // Dungeon room char
#define DNGN_PATH C_YELLOW "#" C_RESET // Dungeon path char

#define ROOM_MIN_DX 3 // Min width
#define ROOM_MIN_DY 2 // Min height
#define ROOM_MAX_DX 14 // Max width
#define ROOM_MAX_DY 8 // Max height
#define ROOM_MIN_NUM 5 // Min number of rooms
#define ROOM_MAX_NUM 10 // Max number of rooms

struct room
{
  uint8_t x;
  uint8_t y;
  uint8_t dx;
  uint8_t dy;
};

struct dungeon
{
  int numRooms;
  struct room rooms[ROOM_MAX_NUM];
  char *terrain[DNGN_SIZE_Y][DNGN_SIZE_X];
  uint8_t hardness[DNGN_SIZE_Y][DNGN_SIZE_X];
  uint8_t pcX;
  uint8_t pcY;
};

extern struct dungeon dungeon;

void initializeDungeon();
void loadDungeon();
void saveDungeon();
void generateRooms();
void generatePaths();
int distance(struct room room1, struct room room2);
void displayDungeon();

#endif
