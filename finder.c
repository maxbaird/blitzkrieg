#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blitzkrieg.h"
#include "finder.h"
#include "board.h"
#include "lexis.h"

static Board *BOARD;
static bool INITIALIZED;
static size_t TILE_COUNT;

typedef struct path{
  int root;
  int *traversePath;
}Path;

static void checkInit(){
  if(!INITIALIZED){
    fprintf(stdout, "Finder not initialized\n");
    exit(EXIT_FAILURE);
  }
}

static bool canMove(Tile *t, Path *tilePath){
  if(t == NULL) return false;
  size_t i = 0;
  for(i = 0; i < TILE_COUNT; i++){
    if(tilePath->traversePath[i] == t->id){
      return false;
    }
  }
  return true;
}

static void traverse(Tile *t, char *letters, Path *path, int depth){
  char *str = calloc(MAX_WORD_LEN+1, sizeof(char)); //Allocate one extra for \0
  Path *tilePath = malloc(sizeof(Path));
  tilePath->traversePath = malloc(TILE_COUNT * sizeof(int));

  if(str == NULL || tilePath == NULL){
    fprintf(stderr, "Failed to allocate memory for board traversal!\n");
    exit(EXIT_FAILURE);
  }

  if(tilePath->traversePath == NULL){
    fprintf(stderr, "Failed to allocate memory for board traversal!\n");
    exit(EXIT_FAILURE);
  }

  memset(tilePath->traversePath, -1, sizeof(int) * TILE_COUNT);

  if(path == NULL){ // path will be NULL for initial call
    tilePath->root = t->id; //Keep track of the starting tile
  }else{
    memcpy(tilePath->traversePath, path->traversePath, sizeof(int) * TILE_COUNT);
    tilePath->root = path->root;
  }

  tilePath->traversePath[t->id] = t->id;

  if(letters != NULL){
    strcpy(str, letters);
    str[strlen(letters)] = t->letter;
  }else{
    str[0]=t->letter; //Handle initial traversal call
  }

  if(isWord(str)){
    addWord(str, tilePath->root);
  }

  if(depth <= MAX_WORD_LEN){ //Avoids finding all 16 combinations
    if(canMove(t->N, tilePath)){traverse(t->N, str, tilePath, depth+1);}
    if(canMove(t->S, tilePath)){traverse(t->S, str, tilePath, depth+1);}
    if(canMove(t->E, tilePath)){traverse(t->E, str, tilePath, depth+1);}
    if(canMove(t->W, tilePath)){traverse(t->W, str, tilePath, depth+1);}
    if(canMove(t->NE, tilePath)){traverse(t->NE, str, tilePath, depth+1);}
    if(canMove(t->SE, tilePath)){traverse(t->SE, str, tilePath, depth+1);}
    if(canMove(t->SW, tilePath)){traverse(t->SW, str, tilePath, depth+1);}
    if(canMove(t->NW, tilePath)){traverse(t->NW, str, tilePath, depth+1);}
  }

  free(str);
  free(tilePath->traversePath);
  free(tilePath);
}

void initFinder(Board *board){
  BOARD = board;
  TILE_COUNT = getBoardSize(board);
  loadLexis();

  INITIALIZED = true;
}

void findWords(){
  checkInit();

  size_t i = 0;
  Tile *tile = BOARD->tiles;

  for(i = 0; i < TILE_COUNT; i++){
    traverse(tile, NULL, NULL, 0);
    tile++;
  }
}

void unloadFinder(){
  checkInit();
  unloadLexis();
  INITIALIZED = false;
}
