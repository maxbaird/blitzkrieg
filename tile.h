#ifndef __TILE_H__
#define __TILE_H__

#include <stdbool.h>

typedef struct coordinate{
  unsigned int x;
  unsigned int y;
}Coordinate;

typedef struct tile{
  int id;
  char letter;
  Coordinate coordinate;

  struct tile *N;
  struct tile *S;
  struct tile *E;
  struct tile *W;
  struct tile *NE;
  struct tile *SE;
  struct tile *SW;
  struct tile *NW;
}Tile;

Tile* makeTiles(size_t n);
void clearTiles(Tile *t);

#endif /* __TILE_H__ */
