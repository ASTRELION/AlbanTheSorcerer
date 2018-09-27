#ifndef GRADIENTMAP_H
# define GRADIENTMAP_H

#include "dungeonManager.h"
#include "heap.h"

#define mappair(pair) (d->map[pair[dim_y]][pair[dim_x]])
#define mapxy(x, y) (d->map[y][x])
#define hardnesspair(pair) (d->hardness[pair[dim_y]][pair[dim_x]])
#define hardnessxy(x, y) (d->hardness[y][x])

typedef enum dim {
  dim_x,
  dim_y,
  num_dims
} dim_t;

typedef struct corridor_path {
  heap_node_t *hn;
  uint8_t pos[2];
  uint8_t from[2];
  int32_t cost;
} corridor_path_t;

typedef int16_t pair_t[2];
void generateDistanceMap(struct dungeon *d, pair_t from, pair_t to, char mapping[DNGN_SIZE_Y][DNGN_SIZE_X]);

#endif
