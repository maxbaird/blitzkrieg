#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "tile.h"

static void setPaths(Board *b, Tile *t){
  /* Copy values to local variables for readiblity later on */
  int height = b->dimension.height;
  int width = b->dimension.width;

  int X = t->coordinate.x;
  int Y = t->coordinate.y;

  size_t idx = (height * Y) + X; // Calculate the index of the current tile

  /* Since the board is perfect square of 4x4 tiles the index of the adjacent
   * tiles can be calculated by moving forward and backward by fixed amoutns
   * from the current tile.
   *
   * This method will of course generate invalid tile positions, so the
   * positions calculated here are verified before being assigned.
   */
  int N_idx = idx - width;
  int S_idx = idx + height;
  int E_idx = idx + 1;
  int W_idx = idx - 1;
  int NE_idx = idx - (width - 1);
  int SE_idx = idx + (width + 1);
  int SW_idx = idx + (width - 1);
  int NW_idx = idx - (width + 1);

  /* The next block of code does bounds checking for calculated tile indexes.
   * The calculated tile index must fall within the dimensions of the grid
   * before it is assigned to the cardinal position of the current tile.
   *
   * If an index falls outside of the grid, it is invalid and the corresponding
   * cardinal position is set to NULL.
   *
   * If the index is valid, the current cardinal position of the tile is assigned
   * a reference to tile actually at that position.
   */
  t->N = (N_idx < 0) ? NULL : &b->tiles[N_idx];
  t->S = (Y+1 >= height) ? NULL : &b->tiles[S_idx];
  t->E = (X+1 >= width) ? NULL : &b->tiles[E_idx];
  t->W = (W_idx < 0) ? NULL : &b->tiles[W_idx];
  t->NE = (N_idx < 0 || X+1 >= width) ? NULL : &b->tiles[NE_idx];
  t->SE = (Y+1 >= height || X+1 >= width) ? NULL : &b->tiles[SE_idx];
  t->SW = (Y+1 >= height || W_idx < 0) ? NULL : &b->tiles[SW_idx];
  t->NW = (N_idx < 0 || W_idx < 0) ? NULL : &b->tiles[NW_idx];
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
  Tile *currentTile = tiles;

  for(i = 0; i < height; i++){
    for(j = 0; j < width; j++){
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
