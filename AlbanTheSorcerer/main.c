#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>

#include "dungeonManager.h" // Generate, save, load
#include "gradientMap.h"
#include "colors.h"

struct dungeon dungeon = {};

int main(int argc, char *argv[])
{
  printf(C_PURPLE "~\t\t" C_CYAN "    *~*~*~*~*~{ Alban The Sorcerer }~*~*~*~*~*\n" C_RESET);

  bool save = false;
  bool load = false;

  // Parse arguments
  if (argc > 1)
  {
    if (strcmp(argv[1], "--save") == 0)
    {
      save = true;
    }
    else if (strcmp(argv[1], "--load") == 0)
    {
      load = true;
    }

    if (argc > 2 && strcmp(argv[2], "--save") == 0)
    {
      save = true;
    }
    else if (argc > 2 && strcmp(argv[2], "--load") == 0)
    {
      load = true;
    } 
  }

  dungeon = (struct dungeon){};
  initializeDungeon();

  // If Load, call load
  if (load)
  {
    loadDungeon();
  }
  else
  {
    generateRooms();
    generatePaths();
  }

  if (save)
  {
    saveDungeon();
  }

  displayDungeon();

  int i, j;
  for (i = 1; i < DNGN_SIZE_Y - 1; i++)
  {
    for (j = 1; j < DNGN_SIZE_X - 1; j++)
    {
      pair_t p1, p2;
      p1[1] = dungeon.pcY;
      p1[0] = dungeon.pcX;
      p2[1] = i;
      p2[0] = j;

      if (dungeon.hardness[i][j] == 0)
      {
	generateDistanceMap(&dungeon, p2, p1, dungeon.terrainDIST_NT);
      }

      //generateDistanceMap(&dungeon, p2, p1, dungeon.terrainDIST_T);
    }
  }

  

  displayDungeon();

  printf(C_PURPLE "~\n~\n" C_RESET);

  return 0;
}

