//heh heh heh...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tile.h"
#include "board.h"
#include "lexis.h"
#include "finder.h"

#define HEIGHT 4
#define WIDTH 4
#define MAX_LETTERS 16
#define PROMPT ">> "

static void consumeNewline(){
  int c = 0;
  while((c = getchar()) != '\n' && c != EOF);
}

static bool inputValid(char *input){
  size_t i = 0;
  size_t len = 0;

  len = strlen(input);

  if(len < MAX_LETTERS){
    fprintf(stderr, "%d letters needed; %zu entered!\n", MAX_LETTERS, len);
    return false;
  }

  for(i = 0; i < len; i++){
    input[i] = tolower(input[i]);

    if(!isalpha(input[i])){
      fprintf(stderr, "invalid character '%c'. Only letters allowed.\n", input[i]);
      return false;
    }
  }
  return true;
}

static void blitzkrieg(Board *board){
  char letters[MAX_LETTERS+1] = {'\0'};
  char *res = NULL;
  char *p = NULL;

  for(;;){
    fprintf(stdout, "\n%s", PROMPT);

    res = fgets(letters, MAX_LETTERS+1, stdin);

    //Replace newline with terminator
    if((p=strchr(letters, '\n')) != NULL){
      *p = '\0';
    }

    if(res == NULL){break;}

    if(inputValid(letters)){
      placeLetters(board, letters);
      findWords();
    }

    //Consume newline that causes next call to fgets to skip
    consumeNewline();
  }
  fprintf(stdout, "\n");
}

int main(){
  Tile *tiles = makeTiles(HEIGHT * WIDTH);
  Board *board = makeBoard(tiles, HEIGHT, WIDTH);
  initFinder(board);

  blitzkrieg(board);
  //placeLetters(board, "itsnuelatkudadls");
  //findWords();

  clearTiles(tiles);
  clearBoard(board);
  unloadFinder();

  return EXIT_SUCCESS;
}
