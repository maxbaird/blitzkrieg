#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tile.h"

Tile* makeTiles(size_t n){
  Tile *tiles = malloc(sizeof(Tile) * n);

  if(tiles == NULL){
    fprintf(stderr, "Failed to make tiles!\n");
    exit(EXIT_FAILURE);
  }

  size_t i = 0;

  for(i = 0; i < n; i++){
    tiles[i].id = i;
    tiles[i].letter = '\0';
    tiles[i].coordinate.x = 0;
    tiles[i].coordinate.y = 0;
    tiles[i].visited = false;
    tiles[i].N = NULL;
    tiles[i].S = NULL;
    tiles[i].E = NULL;
    tiles[i].W = NULL;
    tiles[i].NE = NULL;
    tiles[i].SE = NULL;
    tiles[i].SW = NULL;
    tiles[i].NW = NULL;
    memset(tiles[i].rootArray, 0, sizeof(bool) * 15);
  }

  return tiles;
}

void clearTiles(Tile *t){
  free(t);
}
