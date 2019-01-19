#ifndef __BOARD_H__
#define __BOARD_H__

#include "tile.h"

typedef struct dimension{
  int height;
  int width;
}Dimension;

typedef struct board{
  Tile *tiles;
  Dimension dimension;
}Board;

Board *makeBoard(Tile *tiles, size_t height, size_t width);
void clearBoard(Board *b);

#endif /* __BOARD_H__ */
