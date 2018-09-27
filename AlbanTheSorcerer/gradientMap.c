#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "gradientMap.h"
#include "heap.h"

static int32_t corridor_path_cmp(const void *key, const void *with)
{
  return ((corridor_path_t *) key)->cost - ((corridor_path_t *) with)->cost;
}

void generateDistanceMap(struct dungeon *d, pair_t from, pair_t to, char mapping[DNGN_SIZE_Y][DNGN_SIZE_X])
{
  static corridor_path_t path[DNGN_SIZE_Y][DNGN_SIZE_X], *p;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x, y;

  if (!initialized) 
  {
    for (y = 0; y < DNGN_SIZE_Y; y++) 
    {
      for (x = 0; x < DNGN_SIZE_X; x++) 
      {
        path[y][x].pos[dim_y] = y;
        path[y][x].pos[dim_x] = x;
      }
    }

    initialized = 1;
  }
  
  for (y = 0; y < DNGN_SIZE_Y; y++) 
  {
    for (x = 0; x < DNGN_SIZE_X; x++) 
    {
      path[y][x].cost = INT_MAX;
    }
  }

  path[from[dim_y]][from[dim_x]].cost = 0;

  heap_init(&h, corridor_path_cmp, NULL);

  for (y = 0; y < DNGN_SIZE_Y; y++) 
  {
    for (x = 0; x < DNGN_SIZE_X; x++) 
    {
      if (dungeon.hardness[y][x] != 255)
      {
        path[y][x].hn = heap_insert(&h, &path[y][x]);
      } 
      else 
      {
        path[y][x].hn = NULL;
      }
    }
  }
  
  while ((p = heap_remove_min(&h))) 
  {
    p->hn = NULL;

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x]].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x]].cost > p->cost + hardnesspair(p->pos)))
    {
      path[p->pos[dim_y] - 1][p->pos[dim_x]].cost = p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] - 1][p->pos[dim_x]].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x]].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x]].hn);
    }

    if ((path[p->pos[dim_y]][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y]][p->pos[dim_x] - 1].cost > p->cost + hardnesspair(p->pos))) 
    {
      path[p->pos[dim_y]][p->pos[dim_x] - 1].cost = p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y]][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]][p->pos[dim_x] - 1].hn);
    }

    if ((path[p->pos[dim_y]][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y]][p->pos[dim_x] + 1].cost > p->cost + hardnesspair(p->pos)))
    {
      path[p->pos[dim_y]][p->pos[dim_x] + 1].cost = p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y]][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]][p->pos[dim_x] + 1].hn);
    }

    if ((path[p->pos[dim_y] + 1][p->pos[dim_x]].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x]].cost > p->cost + hardnesspair(p->pos))) 
    {
      path[p->pos[dim_y] + 1][p->pos[dim_x]].cost = p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] + 1][p->pos[dim_x]].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x]].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x]].hn);
    }

    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost > p->cost + hardnesspair(p->pos))) 
    {
      path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost = p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn);
    }

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x]].cost > p->cost + hardnesspair(p->pos))) 
    {
      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost = p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn);
    }

    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost > p->cost + hardnesspair(p->pos))) 
    {
      path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost = p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn);
    }

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost > p->cost + hardnesspair(p->pos))) 
    {
      path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost = p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn);
    }
  }

  int n = 0;

  for (x = to[dim_x], y = to[dim_y];
          (x != from[dim_x]) || (y != from[dim_y]);
           p = &path[y][x], x = p->from[dim_x], y = p->from[dim_y]) 
  {
    if (dungeon.hardness[y][x] == 0)
    {
      mapping[y][x] = (n++ % 10) + '0';
    }

    mapping[from[dim_y]][from[dim_x]] = (n % 10) + '0';
  }

  heap_delete(&h);
}
