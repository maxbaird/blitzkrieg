#include <stdio.h>
#include <stdlib.h>
#include "tile.h"
#include "board.h"

#define HEIGHT 4
#define WIDTH 4

void printTile(Tile *t){
  fprintf(stdout, "(%u,%u): %c\n", t->coordinate.x, t->coordinate.y, t->letter);

  if(t->N != NULL)
    fprintf(stdout, "N: %c ", t->N->letter);
  if(t->S != NULL)
    fprintf(stdout, "S: %c ", t->S->letter);
  if(t->E != NULL)
    fprintf(stdout, "E: %c ", t->E->letter);
  if(t->W != NULL)
    fprintf(stdout, "W: %c ", t->W->letter);
  if(t->NE != NULL)
    fprintf(stdout, "NE: %c ", t->NE->letter);
  if(t->SE != NULL)
    fprintf(stdout, "SE: %c ", t->SE->letter);
  if(t->SW != NULL)
    fprintf(stdout, "SW: %c ", t->SW->letter);
  if(t->NW != NULL)
    fprintf(stdout, "NW: %c ", t->NW->letter);

  fprintf(stdout, "\n");
}

//heh heh heh...
int main(){
  Tile *tiles = makeTiles(HEIGHT * WIDTH);
  Board *board = makeBoard(tiles, HEIGHT, WIDTH);

  placeLetters(board, "abcdefghijklmnop");

  size_t i = 0;
  for(i = 0; i < HEIGHT * WIDTH; i++){
    printTile(&board->tiles[i]);
  }

  clearTiles(tiles);
  clearBoard(board);

  return EXIT_SUCCESS;
}
