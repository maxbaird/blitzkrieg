#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "finder.h"
#include "board.h"
#include "lexis.h"

/*static*/ bool canMove(Tile *t, int *tilePath){
  if(t == NULL) return false;
  //if(!t->visited) return true;
  int i = 0;
  if(tilePath != NULL){
  for(i = 0; i < 17; i++){
    if(tilePath[i] == t->id){
      return false;
    }
  }
  }
  return true;
}

static void traverse(Tile *t, char *letters, int *path){
  char *str = calloc(17, sizeof(char)); //Replace 17 with dimension of board plus 1
  int *tilePath = calloc(17, sizeof(int));

  if(str == NULL){
    fprintf(stderr, "Failed to allocate memory for board traversal!\n");
    exit(EXIT_FAILURE);
  }

  if(tilePath == NULL){
    fprintf(stderr, "Failed to allocate memory for tile path!\n");
    exit(EXIT_FAILURE);
  }

  memcpy(tilePath, path, sizeof(int) * 17);
  tilePath[t->id] = t->id;

  strcpy(str, letters);
  str[strlen(letters)] = t->letter;

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

void initFinder(){
  loadLexis();
}

void findWords(Board *board){
  //size_t numTiles = board->dimension.height * board->dimension.width;
  //size_t i = 0;

  Tile *tile = board->tiles;
  tile->visited=true;

  char letters[2] = {tile->letter, '\0'};
  int tilePath[] = {tile->id};

  if(canMove(tile->N, NULL)){traverse(tile->N, letters, tilePath);}
  tile->visited=true;
  if(canMove(tile->S, NULL)){traverse(tile->S, letters, tilePath);}
  tile->visited=true;
  if(canMove(tile->E, NULL)){traverse(tile->E, letters, tilePath);}
  tile->visited=true;
  if(canMove(tile->W, NULL)){traverse(tile->W, letters, tilePath);}
  tile->visited=true;
  if(canMove(tile->NE, NULL)){traverse(tile->NE, letters, tilePath);}
  tile->visited=true;
  if(canMove(tile->SE, NULL)){traverse(tile->SE, letters, tilePath);}
  tile->visited=true;
  if(canMove(tile->SW, NULL)){traverse(tile->SW, letters, tilePath);}
  tile->visited=true;
  if(canMove(tile->NW, NULL)){traverse(tile->NW, letters, tilePath);}

  fprintf(stdout, "After traversing\n");
  //for(i = 0; i < numTiles; i++){
    //tile++;
 // }
}

void unloadFinder(){
  unloadLexis();
}
