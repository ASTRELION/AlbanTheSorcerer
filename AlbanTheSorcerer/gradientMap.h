#ifndef GRADIENTMAP_H
# define GRADIENTMAP_H

#include "dungeonManager.h"
#include "heap.h"

typedef enum dim 
{
  dim_x,
  dim_y,
  num_dims
} dim_t;

typedef struct corridor_path 
{
  heap_node_t *hn;
  uint8_t pos[2];
  uint8_t from[2];
  int32_t cost;
} corridor_path_t;

typedef int16_t pair_t[2];
void genMapNoTunnel(struct dungeon *d);
void genMapTunnel(struct dungeon *d);

#endif
