#include <stdio.h>
#include <stdlib.h>
#include "board.h"

static void setPaths(Board *b, Tile *t, size_t idx){
  int height = b->dimension.height;
  int width = b->dimension.width;

  int N = idx - width;
  int S = idx + height;
  int E = idx + 1;
  int W = idx - 1;
  int NE = idx - (width - 1);
  int SE = idx + (width + 1);
  int SW = idx + (width - 1);
  int NW = idx - (width + 1);

  t->N = (N < 0) ? NULL : &b->tiles[N];
  t->S = (S > height) ? NULL : &b->tiles[S];
  t->E = (E > width) ? NULL : &b->tiles[E];
  t->W = (W < 0) ? NULL : &b->tiles[W];
  t->NE = (N < 0 || E > width) ? NULL : &b->tiles[NE];
  t->SE = (S > height || E > width) ? NULL : &b->tiles[SE];
  t->SW = (S > height || W < 0) ? NULL : &b->tiles[SW];
  t->NW = (N < 0 || W < 0) ? NULL : &b->tiles[NW];
}

Board *makeBoard(Tile *tiles, size_t n, int height, int width){
  Board *board = malloc(sizeof(Board));

  if(board == NULL){
    fprintf(stderr, "Failed to make board!\n");
    exit(EXIT_FAILURE);
  }

  board->dimension.height = height;
  board->dimension.width = width;
  board->tiles = tiles;

  size_t i = 0;

  for(i = 0; i < n; i++){
    setPaths(board, &board->tiles[i], i);
  }

  return board;
}

void clearBoard(Board *b){
  free(b);
}
