#ifndef __TILE_H__
#define __TILE_H__

#include <stdbool.h>

typedef struct direction{
  bool N;
  bool S;
  bool E;
  bool W;
  bool NE;
  bool SE;
  bool SW;
  bool NW;
}Direction;

typedef struct coordinate{
  unsigned int x;
  unsigned int y;
}Coordinate;

typedef struct tile{
  char letter;
  Coordinate coordinate;
  Direction direction;
}Tile;

void makeTile(){

}

#endif /* __TILE_H__ */
