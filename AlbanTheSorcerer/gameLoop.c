#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "gameLoop.h"
#include "dungeonManager.h"

void startGameLoop()
{
  gameLoop();
}

void gameLoop()
{
  pcMove();
  
  displayDungeon();
  usleep(250000);
  gameLoop();
}

void pcMove()
{
  if (dungeon.pcX > 1 || dungeon.pcY < DNGN_SIZE_Y - 2)
  {
    if (dungeon.pcX > 1)
    {
      dungeon.pcX--;
    }

    if (dungeon.pcY < DNGN_SIZE_Y - 2)
    {
      dungeon.pcY++;
    }
  }
  else
  {
    dungeon.pcY--;
  }

  dungeon.hardness[dungeon.pcY][dungeon.pcX] = 0;
}
