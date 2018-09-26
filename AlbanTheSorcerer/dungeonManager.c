#include "dungeonManager.h" // Generate, save, load

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>

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

void loadDungeon()
{
  FILE *f;
  char *path = malloc(strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1);
  strcpy(path, getenv("HOME"));
  strcat(path, "/.rlg327/dungeon");

  f = fopen(path, "r");

  if (!f)
  {
    fprintf(stderr, "Failed to open %s\n", path);
    return;
  }

  // Get semantic
  char semantic[12];
  fread(&semantic, 12, 1, f);

  // Get version
  uint32_t version;
  fread(&version, 4, 1, f);
  version = be32toh(version);

  // Get file size
  uint32_t fileSize;
  fread(&fileSize, 4, 1, f);
  fileSize = be32toh(fileSize);

  // Get (x,y) of player
  int8_t pcX;
  fread(&pcX, 1, 1, f);
  int8_t pcY;
  fread(&pcY, 1, 1, f);
  dungeon.pcX = pcX;
  dungeon.pcY = pcY;

  // Fill hardness array
  int i, j;
  for (i = 0; i < DNGN_SIZE_Y; i++)
  {
    for (j = 0; j < DNGN_SIZE_X; j++)
    {
      uint8_t h;
      fread(&h, 1, 1, f);
      dungeon.hardness[i][j] = h;
    }
  }

  // Calculate num rooms
  int numRooms = (fileSize - 1702) / 4;
  dungeon.numRooms = numRooms;

  // Generate rooms
  int r = 0;
  while (!feof(f) && r < numRooms)
  {
    fread(&dungeon.rooms[r].x, 1, 1, f);
    fread(&dungeon.rooms[r].y, 1, 1, f);
    fread(&dungeon.rooms[r].dx, 1, 1, f);
    fread(&dungeon.rooms[r].dy, 1, 1, f);

    r++;
  }

  free(path);
}

void saveDungeon()
{
  FILE *f;
  char *path = malloc(strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1);
  strcpy(path, getenv("HOME"));
  strcat(path, "/.rlg327/dungeon");

  f = fopen(path, "w");

  if (!f)
  {
    fprintf(stderr, "Failed to open %s\n", path);
    return;
  }

  char semantic[12] = "RLG327-F2018";
  fwrite(semantic, 12, 1, f);

  uint32_t version = 0;
  version = htobe32(version);
  fwrite(&version, 4, 1, f);

  uint32_t fileSize = 1702 + (dungeon.numRooms * 4);
  fileSize = htobe32(fileSize);
  fwrite(&fileSize, 4, 1, f);

  fwrite(&dungeon.pcX, 1, 1, f);
  fwrite(&dungeon.pcY, 1, 1, f);

  int i, j;
  for (i = 0; i < DNGN_SIZE_Y; i++)
  {
    for (j = 0; j < DNGN_SIZE_X; j++)
    {
      uint8_t h = dungeon.hardness[i][j];
      fwrite(&h, 1, 1, f);
    }
  }

  int r;
  for (r = 0; r < dungeon.numRooms; r++)
  {
    uint8_t x = dungeon.rooms[r].x;
    uint8_t y = dungeon.rooms[r].y;
    uint8_t dx = dungeon.rooms[r].dx;
    uint8_t dy = dungeon.rooms[r].dy;

    fwrite(&x, 1, 1, f);
    fwrite(&y, 1, 1, f);
    fwrite(&dx, 1, 1, f);
    fwrite(&dy, 1, 1, f);
  }

  free(path);
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
	      //dungeon.terrain[i][j] = DNGN_ROOM;
	      dungeon.hardness[i][j] = 0;
	    }
	}

      dungeon.rooms[dungeon.numRooms].x = x;
      dungeon.rooms[dungeon.numRooms].y = y;
      dungeon.rooms[dungeon.numRooms].dx = dx;
      dungeon.rooms[dungeon.numRooms].dy = dy;

      dungeon.numRooms++;
    }
  }

  // Selection sort rooms in closest-to-previous order
  int i;
  for (i = 0; i < dungeon.numRooms - 1; i++)
  {
    int minIndex = i + 1;
    int minDistance = distance(dungeon.rooms[i], dungeon.rooms[i + 1]);

    int j;
    for (j = i + 1; j < dungeon.numRooms; j++)
    {
      int dist = distance(dungeon.rooms[i], dungeon.rooms[j]);
      if (dist < minDistance)
      {
	minDistance = dist;
	minIndex = j;
      }
    }
    
    // Swap with lowest found
    struct room tmp = (struct room){dungeon.rooms[i + 1].x, dungeon.rooms[i + 1].y,
				    dungeon.rooms[i + 1].dx, dungeon.rooms[i + 1].dy};
    dungeon.rooms[i + 1] = (struct room){dungeon.rooms[minIndex].x, dungeon.rooms[minIndex].y,
				     dungeon.rooms[minIndex].dx, dungeon.rooms[minIndex].dy};
    dungeon.rooms[minIndex] = (struct room){tmp.x, tmp.y,
				     tmp.dx, tmp.dy};
  }
}

// Find distance between center of rooms
int distance(struct room room1, struct room room2)
{
  int x1 = room1.x + (room1.dx / 2);
  int y1 = room1.y + (room1.dy / 2);
  int x2 = room2.x + (room2.dx / 2);
  int y2 = room2.y + (room2.dy / 2);

  int dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
  return dist;
}

void generatePaths()
{
  int i;
  for (i = 0; i < dungeon.numRooms - 1; i++)
  {
    int x1 = dungeon.rooms[i].x + (dungeon.rooms[i].dx / 2);
    int y1 = dungeon.rooms[i].y + (dungeon.rooms[i].dy / 2);
    int x2 = dungeon.rooms[i + 1].x + (dungeon.rooms[i + 1].dx / 2);
    int y2 = dungeon.rooms[i + 1].y + (dungeon.rooms[i + 1].dy / 2);
    
    while (x1 != x2)
    {
      if (strcmp(dungeon.terrain[y1][x1], DNGN_ROOM) != 0)
      {
	//dungeon.terrain[y1][x1] =  DNGN_PATH;
	dungeon.hardness[y1][x1] = 0;
      }

      if (x1 > x2)
	x1--;
      else
	x1++;
    }

    while (y1 != y2)
    {
      if (strcmp(dungeon.terrain[y1][x1], DNGN_ROOM) != 0)
      {
	//dungeon.terrain[y1][x1] = DNGN_PATH;
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
  // Print cooridors and walls
  int y, x;
  for (y = 0; y < DNGN_SIZE_Y; y++)
  {
    for (x = 0; x < DNGN_SIZE_X; x++)
    {
      if (dungeon.hardness[y][x] == 0)
      {
	dungeon.terrain[y][x] = DNGN_PATH;
      }
      else if (dungeon.hardness[y][x] == 255)
      {
	dungeon.terrain[y][x] = DNGN_EDGE;
      }
    }
  }

  // Print rooms
  int r;
  for (r = 0; r < dungeon.numRooms; r++)
  {
    int y, x;
    for (y = dungeon.rooms[r].y; y < dungeon.rooms[r].y + dungeon.rooms[r].dy; y++)
    {
      for (x = dungeon.rooms[r].x; x < dungeon.rooms[r].x + dungeon.rooms[r].dx; x++)
      {
	dungeon.terrain[y][x] = DNGN_ROOM;
      }
    }
  }

  // Print characters
  
  int i, j;
  for (i = 0; i < DNGN_SIZE_Y; i++)
  {
    for (j = 0; j < DNGN_SIZE_X; j++)
    {
      printf("%s", dungeon.terrain[i][j]);
    }

    printf("\n");
  }
}
