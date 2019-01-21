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

  if(canMove(t->NW)){traverse(t->NW);}
  if(canMove(t->N)){traverse(t->N);}
  if(canMove(t->S)){traverse(t->S);}
  //if(canMove(t->E)){traverse(t->E);}
  //if(canMove(t->W)){traverse(t->W);}
  //if(canMove(t->NE)){traverse(t->NE);}
  //if(canMove(t->SE)){traverse(t->SE);}
  //if(canMove(t->SW)){traverse(t->SW);}
  //t->visited = false;
}

void initFinder(){
  loadLexis();
}

void findWords(Board *board){
  //size_t numTiles = board->dimension.height * board->dimension.width;
  //size_t i = 0;

  Tile *tile = board->tiles;
  //fprintf(stdout, "This is the letter %c\n", tile->letter);
  traverse(tile);
  fprintf(stdout, "After traversing\n");
  //for(i = 0; i < numTiles; i++){
    //tile++;
 // }
}

void unloadFinder(){
  unloadLexis();
}
