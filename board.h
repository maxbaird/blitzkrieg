#ifndef __BOARD_H__
#define __BOARD_H__

#include "tile.h"

typedef struct dimension{
  size_t height;
  size_t width;
}Dimension;

typedef struct board{
  Tile *tiles;
  Dimension dimension;
}Board;

Board *makeBoard(Tile *tiles, size_t height, size_t width);
void placeLetters(Board *b, char *letters);
void clearBoard(Board *b);

#endif /* __BOARD_H__ */
