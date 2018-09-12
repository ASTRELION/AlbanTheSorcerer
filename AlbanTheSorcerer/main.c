#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define DNGN_SIZE_Y 21 // Max dungeon height
#define DNGN_SIZE_X 80 // Max dungeon width
#define DNGN_EDGE '-' // Dungeon edge char
#define DNGN_ROCK ' ' // Dungeon rock char
#define DNGN_ROOM '.' // Dungeon room char
#define DNGN_PATH '#' // Dungeon path char

#define ROOM_MIN_DX 3 // Min width
#define ROOM_MIN_DY 2 // Min height
#define ROOM_MAX_DX 14 // Max width
#define ROOM_MAX_DY 8 // Max height
#define ROOM_MIN_NUM 5 // Min number of rooms
#define ROOM_MAX_NUM 10 // Max number of rooms

struct room
{
  int x;
  int y;
  int dx;
  int dy;
};

struct dungeon
{
  int numRooms;
  struct room rooms[ROOM_MAX_NUM];
  char terrain[DNGN_SIZE_Y][DNGN_SIZE_X];
  int hardness[DNGN_SIZE_Y][DNGN_SIZE_X];
};

void initializeDungeon();
void generateRooms();
void generatePaths();
void displayDungeon();

struct dungeon dungeon = {0};

/*
char board[SIZE_Y][SIZE_X];
struct room rooms[MAX_ROOMS];
int numRooms = 0;
*/

int main(int argc, char *argv[])
{
  printf("~\t\t    *~*~*~*~*~{ Alban The Sorcerer }~*~*~*~*~*\n");

  initializeDungeon();

  generateRooms();
  generatePaths();
  displayDungeon();

  printf("~\n~\n");

  return 0;
}

// Initialize terrain and hardness to default values
void initializeDungeon()
{
  int i, j;
  for (i = 0; i < DNGN_SIZE_Y; i++)
  {
    for (j = 0; j < DNGN_SIZE_X; j++)
    {
      // 255 '-' if an edge, 1 ' ' if rock
      if (i == 0 || i == DNGN_SIZE_Y - 1 ||
	  j == 0 || j == DNGN_SIZE_X - 1)
      {
	dungeon.terrain[i][j] = DNGN_EDGE;
	dungeon.hardness[i][j] = 255;
      }
      else
      {
	dungeon.terrain[i][j] = DNGN_ROCK;
	dungeon.hardness[i][j] = 1;
      }
    }
  }
}

void generateRooms()
{
  srand(time(NULL));

  int roomCount = (rand() % (ROOM_MAX_NUM - ROOM_MIN_NUM)) + ROOM_MIN_NUM;
  
  while (dungeon.numRooms <= roomCount)
  {
    int x = (rand() % (DNGN_SIZE_X - ROOM_MIN_DY)) + 1; //  1-77 (valid x's)
    int y = (rand() % (DNGN_SIZE_Y - ROOM_MIN_DX)) + 1; // 1-18 (valid y's)
    int dx = (rand() % ROOM_MAX_DX) + ROOM_MIN_DX; // length (min 3)
    int dy = (rand() % ROOM_MAX_DY) + ROOM_MIN_DY; // width (min 2)

    bool validRoom = true;

    // Check if room is a valid placement
    int r;
    for (r = 0; r < ROOM_MAX_NUM; r++)
    {
      if ((x <= dungeon.rooms[r].x + dungeon.rooms[r].dx && dungeon.rooms[r].x <= x + dx + 1) &&
	  (y + dy >= dungeon.rooms[r].y && dungeon.rooms[r].y + dungeon.rooms[r].dy >= y))
      {
	validRoom = false;
	break;
      }
    }

    // Add room to dungeon
    if (validRoom && x + dx < DNGN_SIZE_X - 2 && y + dy < DNGN_SIZE_Y - 2)
    {
      int i, j;
      for (i = y; i < y + dy; i++)
	{
	  for (j = x; j < x + dx; j++)
	    {
	      dungeon.terrain[i][j] = DNGN_ROOM;
	      dungeon.hardness[i][j] = 0;
	    }
	}

      dungeon.rooms[dungeon.numRooms].x = x;
      dungeon.rooms[dungeon.numRooms].y = y;
      dungeon.rooms[dungeon.numRooms].dx = dx;
      dungeon.rooms[dungeon.numRooms].dy = dy;

      dungeon.numRooms++;
      //break;
    }
  }

  // Sort rooms from smallX -> bigX, smallY -> bigY
  
}

void generatePaths()
{
  int i;
  for (i = 0; i < dungeon.numRooms - 1; i++)
  {
    int x1 = dungeon.rooms[i].x;
    int y1 = dungeon.rooms[i].y;
    int x2 = dungeon.rooms[i + 1].x;
    int y2 = dungeon.rooms[i + 1].y;
    
    while (x1 != x2)
    {
      if (dungeon.terrain[y1][x1] != DNGN_ROOM)
      {
	dungeon.terrain[y1][x1] = DNGN_PATH;
	dungeon.hardness[y1][x1] = 0;
      }

      if (x1 > x2)
	x1--;
      else
	x1++;
    }

    while (y1 != y2)
    {
      if (dungeon.terrain[y1][x1] != DNGN_ROOM)
      {
	dungeon.terrain[y1][x1] = DNGN_PATH;
	dungeon.hardness[y1][x1] = 0;
      }

      if (y1 > y2)
	y1--;
      else
	y1++;
    }
  }
}

void displayDungeon()
{
  int i, j;
  for (i = 0; i < DNGN_SIZE_Y; i++)
  {
    for (j = 0; j < DNGN_SIZE_X; j++)
    {	
      printf("%c", dungeon.terrain[i][j]);
    }

    printf("\n");
  }
}
