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

static void traverse(Tile *t){
  //printPath(t);
  fprintf(stdout, "%c\n", t->letter);
  t->visited=true;

  //if(!movesLeft(t)){
  //  return;
  //}

  if(canMove(t->N)){traverse(t->N);}
  if(canMove(t->S)){traverse(t->S);}
  if(canMove(t->E)){traverse(t->E);}
  if(canMove(t->W)){traverse(t->W);}
  if(canMove(t->NE)){traverse(t->NE);}
  if(canMove(t->SE)){traverse(t->SE);}
  if(canMove(t->SW)){traverse(t->SW);}
  if(canMove(t->NW)){traverse(t->NW);}
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
  if(canMove(tile->N)){traverse(tile->N);}
  resetBoard(board);
  tile->visited=true;
  fprintf(stdout, "Traversing again\n");
  if(canMove(tile->S)){traverse(tile->S);}
  resetBoard(board);
  tile->visited=true;
  fprintf(stdout, "Traversing again\n");
  if(canMove(tile->E)){traverse(tile->E);}
  resetBoard(board);
  tile->visited=true;
  fprintf(stdout, "Traversing again\n");
  if(canMove(tile->W)){traverse(tile->W);}
  resetBoard(board);
  tile->visited=true;
  fprintf(stdout, "Traversing again\n");
  if(canMove(tile->NE)){traverse(tile->NE);}
  resetBoard(board);
  tile->visited=true;
  fprintf(stdout, "Traversing again\n");
  if(canMove(tile->SE)){traverse(tile->SE);}
  resetBoard(board);
  tile->visited=true;
  fprintf(stdout, "Traversing again\n");
  if(canMove(tile->SW)){traverse(tile->SW);}
  resetBoard(board);
  tile->visited=true;
  fprintf(stdout, "Traversing again\n");
  if(canMove(tile->NW)){traverse(tile->NW);}
  resetBoard(board);
  tile->visited=true;
  fprintf(stdout, "Traversing again\n");
  //fprintf(stdout, "This is the letter %c\n", tile->letter);
  fprintf(stdout, "After traversing\n");
  //for(i = 0; i < numTiles; i++){
    //tile++;
 // }
}

void unloadFinder(){
  unloadLexis();
}
