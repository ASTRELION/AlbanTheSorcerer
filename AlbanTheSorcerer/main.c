#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

struct room
{
  int x;
  int y;
  int width;
  int length;
};

void addRoomsToBoard();
void printBoard();

#define SIZE_Y 21
#define SIZE_X 80
#define BOARD_EDGE '-'
#define BOARD_ROCK ' '
#define BOARD_ROOM '.'
#define BOARD_PATH '#'

char board[SIZE_Y][SIZE_X];
struct room rooms[20];

int main(int argc, char *argv[])
{
  printf("~\t\t    *~*~*~*~* [ Alban The Sorcerer ] *~*~*~*~*\n");

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
  printBoard();

  printf("~\n~\n");

  return 0;
}

void addRoomsToBoard()
{
  int seed = time(NULL);
  srand(seed);

  int fails = 0;
  int rms = 0;
  
  while (fails < 20 && rms < 10)
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

      int r;
      for (r = 0; r < 20; r++)
      {
        if ((x + dx >= rooms[r].x - 1 && x <= rooms[r].x + dx + 1) &&
	    (y + dy >= rooms[r].y - 1 && y <= rooms[r].y + dy + 1))
	{
	  validRoom = false;
	  break;
	}
      }

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

	rooms[rms].x = x;
	rooms[rms].y = y;
	rooms[rms].width = dx;
	rooms[rms].length = dy;

	f = false;
	rms++;
	break;
      }
    }

    if (f)
      fails++;
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
