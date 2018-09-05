#include <stdio.h>

struct room
{
  int x;
  int y;
  int width;
  int length;
};

void addRoomsToBoard();
void printBoard();

#define SIZE_X 21
#define SIZE_Y 81
#define BOARD_EDGE '-'
#define BOARD_ROCK ' '
#define BOARD_ROOM '.'
#define BOARD_PATH '#'

char board[SIZE_X][SIZE_Y];

int main(int argc, char *argv[])
{
  printf("~\t\t    *~*~*~*~* [ Alban The Sorcerer ] *~*~*~*~*\n");

  // Initialize dungeon to blank
  int i, j;
  for (i = 0; i < SIZE_X; i++)
  {
    for (j = 0; j < SIZE_Y; j++)
    {
      if (i == 0 || i == SIZE_X - 1 || j == 0 || j == SIZE_Y - 1)
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
  
}

void printBoard()
{
  int i, j;
  for (i = 0; i < SIZE_X; i++)
  {
    for (j = 0; j < SIZE_Y; j++)
    {
      printf("%c", board[i][j]);
    }

    printf("\n");
  }
}
