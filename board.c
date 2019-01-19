#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "tile.h"

static void setPaths(Board *b, Tile *t){
  int height = b->dimension.height;
  int width = b->dimension.width;

  int X = t->coordinate.x;
  int Y = t->coordinate.y;

  size_t idx = (height * Y) + X;

  int N = idx - width;
  int S = idx + height;
  int E = idx + 1;
  int W = idx - 1;
  int NE = idx - (width - 1);
  int SE = idx + (width + 1);
  int SW = idx + (width - 1);
  int NW = idx - (width + 1);

  t->N = (N < 0) ? NULL : &b->tiles[N];
  t->S = (Y+1 >= height) ? NULL : &b->tiles[S];
  t->E = (X+1 >= width) ? NULL : &b->tiles[E];
  t->W = (W < 0) ? NULL : &b->tiles[W];
  t->NE = (N < 0 || X+1 >= width) ? NULL : &b->tiles[NE];
  t->SE = (Y+1 >= height || X+1 >= width) ? NULL : &b->tiles[SE];
  t->SW = (Y+1 >= height || W < 0) ? NULL : &b->tiles[SW];
  t->NW = (N < 0 || W < 0) ? NULL : &b->tiles[NW];
}

Board *makeBoard(Tile *tiles, size_t height, size_t width){
  Board *board = malloc(sizeof(Board));

  if(board == NULL){
    fprintf(stderr, "Failed to make board!\n");
    exit(EXIT_FAILURE);
  }

  board->dimension.height = height;
  board->dimension.width = width;
  board->tiles = tiles;

  size_t i = 0;
  size_t j = 0;
  //size_t idx = 0;
  Tile *currentTile = tiles;

  for(i = 0; i < height; i++){
    for(j = 0; j < width; j++){
      //idx = (width * i) + j;
      currentTile->coordinate.x = j;
      currentTile->coordinate.y = i;
      setPaths(board, currentTile);
      currentTile++;
    }
  }

  return board;
}

void placeLetters(Board *b, char *letters){
  size_t i = 0;

  for(i = 0; i < b->dimension.height * b->dimension.width; i++){
      b->tiles[i].letter = letters[i];
  }
}

void clearBoard(Board *b){
  free(b);
}
