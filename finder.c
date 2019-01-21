#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "finder.h"
#include "board.h"
#include "lexis.h"

static Board *BOARD;
static bool INITIALIZED;
static size_t TILE_COUNT;

static void checkInit(){
  if(!INITIALIZED){
    fprintf(stdout, "Finder not initialized\n");
    exit(EXIT_FAILURE);
  }
}

static bool canMove(Tile *t, int *tilePath){
  if(t == NULL) return false;
  size_t i = 0;
  for(i = 0; i < TILE_COUNT; i++){
    if(tilePath[i] == t->id){
      return false;
    }
  }
  return true;
}

static void traverse(Tile *t, char *letters, int *path){
  char *str = calloc(TILE_COUNT+1, sizeof(char)); //Allocate one extra for \0
  int *tilePath = calloc(TILE_COUNT, sizeof(int));

  if(str == NULL || tilePath == NULL){
    fprintf(stderr, "Failed to allocate memory for board traversal!\n");
    exit(EXIT_FAILURE);
  }

  if(path != NULL){ // path will be NULL for initial call
    memcpy(tilePath, path, sizeof(int) * TILE_COUNT);
  }

  tilePath[t->id] = t->id;

  if(letters != NULL){
    strcpy(str, letters);
    str[strlen(letters)] = t->letter;
  }else{
    str[0]=t->letter; //Handle initial traversal call
  }

  if(isWord(str)){
    fprintf(stdout, "%s:%zu\n", str, strlen(str));
  }

  if(canMove(t->N, tilePath)){traverse(t->N, str, tilePath);}
  if(canMove(t->S, tilePath)){traverse(t->S, str, tilePath);}
  if(canMove(t->E, tilePath)){traverse(t->E, str, tilePath);}
  if(canMove(t->W, tilePath)){traverse(t->W, str, tilePath);}
  if(canMove(t->NE, tilePath)){traverse(t->NE, str, tilePath);}
  if(canMove(t->SE, tilePath)){traverse(t->SE, str, tilePath);}
  if(canMove(t->SW, tilePath)){traverse(t->SW, str, tilePath);}
  if(canMove(t->NW, tilePath)){traverse(t->NW, str, tilePath);}

  free(str);
  free(tilePath);
}

void initFinder(Board *board){
  BOARD = board;
  TILE_COUNT = BOARD->dimension.height * BOARD->dimension.width;
  loadLexis();
  INITIALIZED = true;
}

void findWords(){
  checkInit();

  size_t i = 0;
  Tile *tile = BOARD->tiles;

  for(i = 0; i < TILE_COUNT; i++){
    traverse(tile, NULL, NULL);
    tile++;
  }
}

void unloadFinder(){
  checkInit();
  unloadLexis();
  INITIALIZED = false;
}
