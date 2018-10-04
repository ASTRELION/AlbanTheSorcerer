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
#include "gameLoop.h"
#include "colors.h"

struct dungeon dungeon = {};

int main(int argc, char *argv[])
{
  bool save = false;
  bool load = false;

  // Parse arguments
  if (argc > 1)
  {
    int i = 0;
    for (i = 1; i < argc; i++)
    {
      if (strcmp(argv[i], "--load") == 0)
      {
	load = true;
      }
      else if (strcmp(argv[i], "--save") == 0)
      {
	save = true;
      }
      else if (strcmp(argv[i], "--nummon") == 0 && 
	       (dungeon.numMonsters = atoi(argv[i + 1])))
      {
	printf("NumMonsters: %d\n", dungeon.numMonsters);
      }
    } 
  }

  if (!dungeon.numMonsters)
  {
    dungeon.numMonsters = 10;
  }

  // Set dungeon values to default
  initializeDungeon();

  // If Load, call load
  if (load)
  {
    loadDungeon();
    printf("Dungeon Loaded.\n");
  }
  else
  {
    generateRooms();
    generatePaths();
  }

  generateMonsters();

  // Generate distance maps (tunnel & non-tunnel)
  genMapNoTunnel(&dungeon);
  genMapTunnel(&dungeon);

  if (save)
  {
    saveDungeon();
    printf("Dungeon Saved.\n");
  }

  displayDungeon();

  startGameLoop();

  return 0;
}

