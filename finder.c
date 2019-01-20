#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "finder.h"
#include "board.h"
#include "lexis.h"

static void setValidPaths(Tile *t){
  int n = 0;
  if(t->N != NULL)n++;
  if(t->S != NULL)n++;
  if(t->E != NULL)n++;
  if(t->W != NULL)n++;
  if(t->NE != NULL)n++;
  if(t->SE != NULL)n++;
  if(t->SW != NULL)n++;
  if(t->NW != NULL)n++;

  t->paths = malloc(sizeof(Path) * n);

  if(t->paths == NULL){
    fprintf(stderr, "Failed to set valid paths\n");
    exit(EXIT_FAILURE);
  }
}

static char traverse(Tile *t){

  if(t->paths->complete){
    return t->letter;
  }

  setValidPaths(t);

  
}

void initFinder(){
  loadLexis();
}

void findWords(Board *board){
  size_t numTiles = board->dimension.height * board->dimension.width;
  size_t i = 0;

  Tile *tile = board->tiles;

  for(i = 0; i < numTiles; i++){
    traverse(tile);
    tile++;
  }
}

void unloadFinder(){
  unloadLexis();
}
