#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "finder.h"
#include "board.h"
#include "lexis.h"

/*static*/ bool canMove(Tile *t){
  if(t == NULL) return false;
  if(!t->visited) return true;
  return false;
}

/*static*/ bool movesLeft(Tile *t){
  if(t->N != NULL){ if(!t->N->visited) return true;}
  if(t->S != NULL){ if(!t->S->visited) return true;}
  if(t->E != NULL){ if(!t->E->visited) return true;}
  if(t->W != NULL){ if(!t->W->visited) return true;}
  if(t->NE != NULL){ if(!t->NE->visited) return true;}
  if(t->SE != NULL){ if(!t->SE->visited) return true;}
  if(t->SW != NULL){ if(!t->SW->visited) return true;}
  if(t->NW != NULL){ if(!t->NW->visited) return true;}
  return false;
}

///*static*/ void printPath(Tile *tile){
//  Tile *t = tile;
//  //fprintf(stdout, "t->root != null: %s\n", t->root != NULL ? "True" : "False");
//
//  while(t->root != NULL){
//    fprintf(stdout, "%c", t->letter);
//    t = t->root;
//  }
//
//  if(t->root == NULL){
//    fprintf(stdout, "%c", t->letter);
//  }
//}

static void traverse(Tile *t, char *letters){
  char *str = calloc(17, sizeof(char)); //Replace 17 with dimension of board plus 1

  if(str == NULL){
    fprintf(stderr, "Failed to allocate memory for board traversal!\n");
    exit(EXIT_FAILURE);
  }

  strcpy(str, letters);
  str[strlen(letters)] = t->letter;

  fprintf(stdout, "%s:%zu\n", str, strlen(str));
  t->visited=true;

  if(canMove(t->N)){traverse(t->N, str);}
  if(canMove(t->S)){traverse(t->S, str);}
  if(canMove(t->E)){traverse(t->E, str);}
  if(canMove(t->W)){traverse(t->W, str);}
  if(canMove(t->NE)){traverse(t->NE, str);}
  if(canMove(t->SE)){traverse(t->SE, str);}
  if(canMove(t->SW)){traverse(t->SW, str);}
  if(canMove(t->NW)){traverse(t->NW, str);}

  free(str);
}

void initFinder(){
  loadLexis();
}

//Move this into board
static void resetBoard(Board *board){
  size_t numTiles = board->dimension.height * board->dimension.width;
  size_t i = 0;

  Tile *tile = board->tiles;
  for(i = 0; i < numTiles; i++){
   tile->visited=false;;
   tile++;
  }
}

void findWords(Board *board){
  //size_t numTiles = board->dimension.height * board->dimension.width;
  //size_t i = 0;

  Tile *tile = board->tiles;
  tile->visited=true;

  char letters[2] = {tile->letter, '\0'};

  if(canMove(tile->N)){traverse(tile->N, letters);}
  resetBoard(board);
  tile->visited=true;
  if(canMove(tile->S)){traverse(tile->S, letters);}
  resetBoard(board);
  tile->visited=true;
  if(canMove(tile->E)){traverse(tile->E, letters);}
  resetBoard(board);
  tile->visited=true;
  if(canMove(tile->W)){traverse(tile->W, letters);}
  resetBoard(board);
  tile->visited=true;
  if(canMove(tile->NE)){traverse(tile->NE, letters);}
  resetBoard(board);
  tile->visited=true;
  if(canMove(tile->SE)){traverse(tile->SE, letters);}
  resetBoard(board);
  tile->visited=true;
  if(canMove(tile->SW)){traverse(tile->SW, letters);}
  resetBoard(board);
  tile->visited=true;
  if(canMove(tile->NW)){traverse(tile->NW, letters);}
  resetBoard(board);
  tile->visited=true;
  //fprintf(stdout, "This is the letter %c\n", tile->letter);
  fprintf(stdout, "After traversing\n");
  //for(i = 0; i < numTiles; i++){
    //tile++;
 // }
}

void unloadFinder(){
  unloadLexis();
}
