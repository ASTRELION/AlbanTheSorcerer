#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "gradientMap.h"
#include "heap.h"

typedef struct path {
  heap_node_t *hn;
  uint8_t pos[2];
} path_t;

static int32_t dist_cmp(const void *key, const void *with) 
{
  return ((int32_t) dungeon.mapNoTunnel[((path_t *) key)->pos[dim_y]]
                                        [((path_t *) key)->pos[dim_x]] -
          (int32_t) dungeon.mapNoTunnel[((path_t *) with)->pos[dim_y]]
                                        [((path_t *) with)->pos[dim_x]]);
}

static int32_t tunnel_cmp(const void *key, const void *with) 
{
  return ((int32_t) dungeon.mapTunnel[((path_t *) key)->pos[dim_y]]
                                      [((path_t *) key)->pos[dim_x]] -
          (int32_t) dungeon.mapTunnel[((path_t *) with)->pos[dim_y]]
                                      [((path_t *) with)->pos[dim_x]]);
}

void genMapNoTunnel(struct dungeon *d)
{
  static corridor_path_t p[DNGN_SIZE_Y][DNGN_SIZE_X], *c;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x, y;

  if (!initialized) 
  {
    //dungeon = d;

    for (y = 0; y < DNGN_SIZE_Y; y++) 
    {
      for (x = 0; x < DNGN_SIZE_X; x++) 
      {
        p[y][x].pos[dim_y] = y;
        p[y][x].pos[dim_x] = x;
      }
    }

    initialized = 1;
  }
  
  for (y = 0; y < DNGN_SIZE_Y; y++) 
  {
    for (x = 0; x < DNGN_SIZE_X; x++) 
    {
      dungeon.mapNoTunnel[y][x] = 255;
    }
  }

  dungeon.mapNoTunnel[dungeon.pcY][dungeon.pcX] = 0;

  heap_init(&h, dist_cmp, NULL);

  for (y = 0; y < DNGN_SIZE_Y; y++) 
  {
    for (x = 0; x < DNGN_SIZE_X; x++) 
    {
      if (dungeon.hardness[y][x] == 0)
      {
        p[y][x].hn = heap_insert(&h, &p[y][x]);
      } 
    }
  }
  
  while ((c = heap_remove_min(&h))) 
  {
    c->hn = NULL;

    // Up left
    if ((p[c->pos[dim_y] - 1][c->pos[dim_x] - 1].hn) && 
	(dungeon.mapNoTunnel[c->pos[dim_y] - 1][c->pos[dim_x] - 1] > dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1))
    {
      dungeon.mapNoTunnel[c->pos[dim_y] - 1][c->pos[dim_x] - 1] = dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1;
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y] - 1][c->pos[dim_x] - 1].hn);
    }
    // Up
    if ((p[c->pos[dim_y] - 1][c->pos[dim_x]].hn) &&
        (dungeon.mapNoTunnel[c->pos[dim_y] - 1][c->pos[dim_x]] > dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1)) 
    {
      dungeon.mapNoTunnel[c->pos[dim_y] - 1][c->pos[dim_x]] = dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1;
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y] - 1][c->pos[dim_x]    ].hn);
    }
    // Up Right
    if ((p[c->pos[dim_y] - 1][c->pos[dim_x] + 1].hn) &&
        (dungeon.mapNoTunnel[c->pos[dim_y] - 1][c->pos[dim_x] + 1] > dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1)) 
    {
      dungeon.mapNoTunnel[c->pos[dim_y] - 1][c->pos[dim_x] + 1] = dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1;
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y] - 1][c->pos[dim_x] + 1].hn);
    }
    // Left
    if ((p[c->pos[dim_y]][c->pos[dim_x] - 1].hn) &&
        (dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x] - 1] > dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1))
    {
      dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x] - 1] = dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1;
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y]][c->pos[dim_x] - 1].hn);
    }
    // Right
    if ((p[c->pos[dim_y]][c->pos[dim_x] + 1].hn) &&
        (dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x] + 1] > dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1)) 
    {
      dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x] + 1] = dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1;
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y]    ][c->pos[dim_x] + 1].hn);
    }
    // Down Left
    if ((p[c->pos[dim_y] + 1][c->pos[dim_x] - 1].hn) &&
        (dungeon.mapNoTunnel[c->pos[dim_y] + 1][c->pos[dim_x] - 1] > dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1)) 
    {
      dungeon.mapNoTunnel[c->pos[dim_y] + 1][c->pos[dim_x] - 1] = dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1;
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y] + 1][c->pos[dim_x] - 1].hn);
    }
    // Down
    if ((p[c->pos[dim_y] + 1][c->pos[dim_x]].hn) &&
        (dungeon.mapNoTunnel[c->pos[dim_y] + 1][c->pos[dim_x]] > dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1))
    {
      dungeon.mapNoTunnel[c->pos[dim_y] + 1][c->pos[dim_x]] = dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1;
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y] + 1][c->pos[dim_x]].hn);
    }
    // Down Right
    if ((p[c->pos[dim_y] + 1][c->pos[dim_x] + 1].hn) &&
        (dungeon.mapNoTunnel[c->pos[dim_y] + 1][c->pos[dim_x] + 1] > dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1)) 
    {
      dungeon.mapNoTunnel[c->pos[dim_y] + 1][c->pos[dim_x] + 1] = dungeon.mapNoTunnel[c->pos[dim_y]][c->pos[dim_x]] + 1;
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y] + 1][c->pos[dim_x] + 1].hn);
    }
  }

  heap_delete(&h);
}

#define tunnelMovementCost(x, y) ((d->hardness[y][x] / 85) + 1)

void genMapTunnel(struct dungeon *d)
{
  static corridor_path_t p[DNGN_SIZE_Y][DNGN_SIZE_X], *c;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x, y;
  int size;

  if (!initialized) 
  {
    initialized = 1;

    for (y = 0; y < DNGN_SIZE_Y; y++) 
    {
      for (x = 0; x < DNGN_SIZE_X; x++) 
      {
        p[y][x].pos[dim_y] = y;
        p[y][x].pos[dim_x] = x;
      }
    }
  }
  
  for (y = 0; y < DNGN_SIZE_Y; y++) 
  {
    for (x = 0; x < DNGN_SIZE_X; x++) 
    {
      dungeon.mapTunnel[y][x] = 255;
    }
  }

  dungeon.mapTunnel[dungeon.pcY][dungeon.pcX] = 0;

  heap_init(&h, tunnel_cmp, NULL);

  for (y = 0; y < DNGN_SIZE_Y; y++) 
  {
    for (x = 0; x < DNGN_SIZE_X; x++) 
    {
      if (dungeon.hardness[y][x] != 255)
      {
        p[y][x].hn = heap_insert(&h, &p[y][x]);
      } 
    }
  }

  size = h.size;
  while ((c = heap_remove_min(&h))) 
  {
    if (--size != h.size)
    {
      exit(1);
    }

    c->hn = NULL;

    // Up left
    if ((p[c->pos[dim_y] - 1][c->pos[dim_x] - 1].hn) && 
	(dungeon.mapTunnel[c->pos[dim_y] - 1][c->pos[dim_x] - 1] > 
	 dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y])))
    {
      dungeon.mapTunnel[c->pos[dim_y] - 1][c->pos[dim_x] - 1] = 
	dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y]);
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y] - 1][c->pos[dim_x] - 1].hn);
    }
    // Up
    if ((p[c->pos[dim_y] - 1][c->pos[dim_x]].hn) &&
        (dungeon.mapTunnel[c->pos[dim_y] - 1][c->pos[dim_x]] > 
	 dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y]))) 
    {
      dungeon.mapTunnel[c->pos[dim_y] - 1][c->pos[dim_x]] = 
	dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y]);
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y] - 1][c->pos[dim_x]    ].hn);
    }
    // Up Right
    if ((p[c->pos[dim_y] - 1][c->pos[dim_x] + 1].hn) &&
        (dungeon.mapTunnel[c->pos[dim_y] - 1][c->pos[dim_x] + 1] > 
	 dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y]))) 
    {
      dungeon.mapTunnel[c->pos[dim_y] - 1][c->pos[dim_x] + 1] = 
	dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y]);
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y] - 1][c->pos[dim_x] + 1].hn);
    }
    // Left
    if ((p[c->pos[dim_y]][c->pos[dim_x] - 1].hn) &&
        (dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x] - 1] > 
	 dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y])))
    {
      dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x] - 1] = 
	dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y]);
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y]][c->pos[dim_x] - 1].hn);
    }
    // Right
    if ((p[c->pos[dim_y]][c->pos[dim_x] + 1].hn) &&
        (dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x] + 1] > 
	 dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y]))) 
    {
      dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x] + 1] = 
	dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y]);
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y]    ][c->pos[dim_x] + 1].hn);
    }
    // Down Left
    if ((p[c->pos[dim_y] + 1][c->pos[dim_x] - 1].hn) &&
        (dungeon.mapTunnel[c->pos[dim_y] + 1][c->pos[dim_x] - 1] > 
	 dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y]))) 
    {
      dungeon.mapTunnel[c->pos[dim_y] + 1][c->pos[dim_x] - 1] = 
	dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y]);
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y] + 1][c->pos[dim_x] - 1].hn);
    }
    // Down
    if ((p[c->pos[dim_y] + 1][c->pos[dim_x]].hn) &&
        (dungeon.mapTunnel[c->pos[dim_y] + 1][c->pos[dim_x]] > 
	 dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y])))
    {
      dungeon.mapTunnel[c->pos[dim_y] + 1][c->pos[dim_x]] = 
	dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y]);
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y] + 1][c->pos[dim_x]].hn);
    }
    // Down Right
    if ((p[c->pos[dim_y] + 1][c->pos[dim_x] + 1].hn) &&
        (dungeon.mapTunnel[c->pos[dim_y] + 1][c->pos[dim_x] + 1] > 
	 dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y]))) 
    {
      dungeon.mapTunnel[c->pos[dim_y] + 1][c->pos[dim_x] + 1] = 
	dungeon.mapTunnel[c->pos[dim_y]][c->pos[dim_x]] + tunnelMovementCost(c->pos[dim_x], c->pos[dim_y]);
      heap_decrease_key_no_replace(&h, p[c->pos[dim_y] + 1][c->pos[dim_x] + 1].hn);
    }
  }

  heap_delete(&h);
}
