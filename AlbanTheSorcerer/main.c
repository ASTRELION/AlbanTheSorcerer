#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

struct room
{
  int x;
  int y;
  int dx;
  int dy;
};

void addRoomsToBoard();
void generateCooridors();
void printBoard();

#define SIZE_Y 21
#define SIZE_X 80
#define MAX_ROOMS 10
#define BOARD_EDGE '-'
#define BOARD_ROCK ' '
#define BOARD_ROOM '.'
#define BOARD_PATH '#'

char board[SIZE_Y][SIZE_X];
struct room rooms[MAX_ROOMS];
int numRooms = 0;

int main(int argc, char *argv[])
{
  printf("~\t\t    *~*~*~*~*~{ Alban The Sorcerer }~*~*~*~*~*\n");

  // Initialize dungeon to blank
  int i, j;
  for (i = 0; i < SIZE_Y; i++)
  {
    for (j = 0; j < SIZE_X; j++)
    {
      if (i == 0 || i == SIZE_Y - 1 || j == 0 || j == SIZE_X - 1)
      {
	board[i][j] = BOARD_EDGE;
      }
      else
      {
	board[i][j] = BOARD_ROCK;
      }
    }
  }

  addRoomsToBoard();
  generateCooridors();
  printBoard();

  printf("~\n~\n");

  return 0;
}

void addRoomsToBoard()
{
  int seed = time(NULL);
  srand(seed);

  int fails = 0;
  
  while (fails < 5 && numRooms < MAX_ROOMS)
  {
    bool f = true;

    int t;
    for (t = 0; t < 2000; t++)
    {
      int x = (rand() % (SIZE_X - 3)) + 1; //  1-77 (valid x's)
      int y = (rand() % (SIZE_Y - 2)) + 1; // 1-18 (valid y's)
      int dx = (rand() % 15) + 3; // length (min 3)
      int dy = (rand() % 10) + 2; // width (min 2)

      bool validRoom = true;

      // Check if room is a valid placement
      int r;
      for (r = 0; r < MAX_ROOMS; r++)
      {
        if ((x <= rooms[r].x + rooms[r].dx && rooms[r].x <= x + dx + 1) &&
	    (y + dy >= rooms[r].y && rooms[r].y + rooms[r].dy >= y))
	{
	  validRoom = false;
	  break;
	}
      }

      // Add room to dungeon
      if (validRoom && x + dx < SIZE_X - 2 && y + dy < SIZE_Y - 2)
      {
	int i, j;
	for (i = y; i < y + dy; i++)
	{
	  for (j = x; j < x + dx; j++)
	  {
	    board[i][j] = BOARD_ROOM;
	  }
	}

	rooms[numRooms].x = x;
	rooms[numRooms].y = y;
	rooms[numRooms].dx = dx;
	rooms[numRooms].dy = dy;

	f = false;
	numRooms++;
	break;
      }
    }

    if (f)
      fails++;
  }
}

void generateCooridors()
{
  int i;
  for (i = 0; i < numRooms - 1; i++)
  {
    int x1 = rooms[i].x;
    int y1 = rooms[i].y;
    int x2 = rooms[i + 1].x;
    int y2 = rooms[i + 1].y;
    
    while (x1 != x2)
    {
      if (board[y1][x1] != BOARD_ROOM)
	board[y1][x1] = BOARD_PATH;

      if (x1 > x2)
	x1--;
      else
	x1++;
    }

    while (y1 != y2)
    {
      if (board[y1][x1] != BOARD_ROOM)
	board[y1][x1] = BOARD_PATH;

      if (y1 > y2)
	y1--;
      else
	y1++;
    }
  }
}

void printBoard()
{
  int i, j;
  for (i = 0; i < SIZE_Y; i++)
  {
    for (j = 0; j < SIZE_X; j++)
    {
      printf("%c", board[i][j]);
    }

    printf("\n");
  }
}
