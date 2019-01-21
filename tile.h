#ifndef __TILE_H__
#define __TILE_H__

#include <stdbool.h>

#define PATH_AMT 8

typedef struct coordinate{
  unsigned int x;
  unsigned int y;
}Coordinate;

typedef struct path{
  bool visited;
  bool complete;
}Path;

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
