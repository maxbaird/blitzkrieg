#include <stdio.h>
#include <stdlib.h>
#include "tile.h"
#include "board.h"
#include "dictionary.h"

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

#define WORD_AMT 6
#define WORD_LEN 32
//heh heh heh...

int main(){
  char word_arr[WORD_AMT][WORD_LEN] = {"this", "is", "aaa", "test", "hello", "xob"};
  AvlTree dictionary = MakeEmpty(NULL);

  int i;
  for(i = 0; i < WORD_AMT; i++){
    dictionary = Insert((void *)&word_arr[i], dictionary);
  }

  fprintf(stdout, "Found: %s\n", (char *)Retrieve(Find("xob", dictionary)));

  //Tile *tiles = makeTiles(HEIGHT * WIDTH);
  //Board *board = makeBoard(tiles, HEIGHT, WIDTH);

  //placeLetters(board, "abcdefghijklmnop");

  //size_t i = 0;
  //for(i = 0; i < HEIGHT * WIDTH; i++){
  //  printTile(&board->tiles[i]);
  //}

  //clearTiles(tiles);
  //clearBoard(board);
  //
  MakeEmpty(dictionary);

  return EXIT_SUCCESS;
}
