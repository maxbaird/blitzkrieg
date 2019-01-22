#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tile.h"
#include "board.h"
#include "lexis.h"
#include "finder.h"

#define HEIGHT 4
#define WIDTH 4
#define MAX_LETTERS 16
#define PROMPT ">> "

//heh heh heh...

static void blitzkrieg(Board *board){
  char letters[MAX_LETTERS+1] = {'\0'};
  char *res = NULL;
  char *p = NULL;
  int c = 0;

  for(;;){
    fprintf(stdout, "\n%s", PROMPT);

    res = fgets(letters, MAX_LETTERS+1, stdin);

    //Replace newline with terminator
    if((p=strchr(letters, '\n')) != NULL){
      *p = '\0';
    }

    if(res == NULL){break;}

    if(strlen(letters) != MAX_LETTERS){
      fprintf(stderr, "%d letters needed; %zu entered!\n", MAX_LETTERS, strlen(letters));
      continue;
    }

    placeLetters(board, letters);
    findWords();

    //Consume newline that causes next call to fgets to skip
    while((c = getchar()) != '\n' && c != EOF);
  }
}

int main(){
  Tile *tiles = makeTiles(HEIGHT * WIDTH);
  Board *board = makeBoard(tiles, HEIGHT, WIDTH);
  initFinder(board);

  blitzkrieg(board);

  clearTiles(tiles);
  clearBoard(board);
  unloadFinder();

  return EXIT_SUCCESS;
}
