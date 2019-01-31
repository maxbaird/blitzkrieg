#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include "blitzkrieg.h"
#include "tile.h"
#include "board.h"
#include "lexis.h"
#include "finder.h"
#include "print.h"
#include "config.h"

#define HEIGHT 4
#define WIDTH 4
#define MAX_LETTERS HEIGHT * WIDTH

#define WELCOME_FILE_NAME "welcome"
#define WELCOME_BUFFER_SIZE 2048

#define PROMPT ">> "

#define DEFAULT_BUFFER_SIZE 32

static Config config;
static WordColumn *WORD_COLUMNS;

static void consumeNewline(){
  int c = 0;
  //Consume newline that causes next call to fgets to skip
  while((c = getchar()) != '\n' && c != EOF);
}

static bool inputValid(char *input){
  size_t i = 0;
  size_t len = 0;
  char *p = NULL;

  //Replace newline with terminator
  if((p=strchr(input, '\n')) != NULL){
    *p = '\0';
  }

  len = strlen(input);

  if(len == 0){
    return false;
  }

  if(len < MAX_LETTERS){
    fprintf(stderr, "%d letters needed; %zu entered!\n", MAX_LETTERS, len);
    return false;
  }

  for(i = 0; i < len; i++){
    input[i] = tolower(input[i]);

    if(!isalpha(input[i])){
      fprintf(stderr, "Invalid character '%c'. Only letters allowed.\n", input[i]);
      consumeNewline();
      return false;
    }
  }
  return true;
}

static void reset(Board *board){
  WordColumn *wc = WORD_COLUMNS;
  size_t i = 0;
  size_t j = 0;
  size_t boardSize = getBoardSize(board);

  for(i = 0; i < boardSize; i++){
    wc->tileIndex = 0;

    for(j = DEFAULT_BUFFER_SIZE; j < wc->buffSize; j++){
      //Free allocations beyond the default buffer size
      free(wc->words[j].word);
    }

    wc->buffSize = DEFAULT_BUFFER_SIZE;

    wc->words = realloc(wc->words, DEFAULT_BUFFER_SIZE * sizeof(Word));
    wc->wordCount = 0;

    if(wc->words == NULL){
      fprintf(stderr, "Out of memory resetting word memory\n");
      exit(EXIT_FAILURE);
    }

    wc++; //Go to next column of words
  }
}

static void freeWordColumns(Board *board){
  WordColumn *wc = WORD_COLUMNS;
  size_t i = 0;
  size_t j = 0;
  size_t boardSize = getBoardSize(board);

  for(i = 0; i < boardSize; i++){
   for(j = 0; j < wc->buffSize; j++){
    free(wc->words[j].word);
   }
   free(wc->words);
   wc++; //Move to next word column
  }

  free(WORD_COLUMNS);
}

static void displayWelcome(){
  FILE *fp = fopen(WELCOME_FILE_NAME, "r");
  char msg[WELCOME_BUFFER_SIZE] = {'\0'};

  if(fp == NULL){
    fprintf(stderr, "Error opening welcome message file!\n");
    return;
  }

  fprintf(stdout, "\n");
  while(fgets(msg, WELCOME_BUFFER_SIZE, fp) != NULL){
    fprintf(stdout, "%s", msg);
  }

  fclose(fp);
}

static void start(Board *board){
  char letters[MAX_LETTERS+1] = {'\0'};
  char *res = NULL;

  displayWelcome();

  for(;;){
    fprintf(stdout, "\n%s", PROMPT);

    res = fgets(letters, MAX_LETTERS+1, stdin);

    if(res == NULL){break;}
    if(!inputValid(letters)){continue;}

    placeLetters(board, letters);
    findWords();
    printWords(board, WORD_COLUMNS);
    reset(board);
    consumeNewline();
  }

  fprintf(stdout, "Bye!\n");
}

static void init(Tile **tiles, Board **board){
  config = getConfig();
  *tiles = makeTiles(HEIGHT * WIDTH);
  *board = makeBoard(*tiles, HEIGHT, WIDTH);
  size_t i = 0;
  size_t j = 0;
  size_t boardSize = getBoardSize(*board);

  WORD_COLUMNS = malloc(boardSize * sizeof(WordColumn));

  if(WORD_COLUMNS == NULL){
    fprintf(stderr, "Failed to allocate memory for word column\n");
    exit(EXIT_FAILURE);
  }

  WordColumn *wc = WORD_COLUMNS;

  for(i = 0; i < boardSize; i++){
    wc->tileIndex = 0;
    wc->buffSize = DEFAULT_BUFFER_SIZE;
    wc->wordCount = 0;
    wc->longestWordLen = 0;
    wc->words = calloc(DEFAULT_BUFFER_SIZE, sizeof(Word));

    if(wc->words == NULL){
      fprintf(stderr, "Failed to allocate memory for word\n");
      exit(EXIT_FAILURE);
    }

    for(j = 0; j < DEFAULT_BUFFER_SIZE; j++){ //Allocate space for each word
      wc->words[j].word = calloc(config.MAX_WORD_LENGTH+1, sizeof(char));

      if(wc->words[j].word == NULL){
        fprintf(stderr, "Failed to allocate memory for words\n");
        exit(EXIT_FAILURE);
      }
    }

    wc++; //Move to next word column
  }
}

static bool wordExists(char *str, WordColumn *wc){
  size_t i = 0;

  for(i = 0; i < wc->wordCount; i++){
    if(strcmp(str, wc->words[i].word) == 0){
      return true;
    }
  }
  return false;
}

void addWord(char *str, int rootTileIdx){
  WordColumn *wc = &WORD_COLUMNS[rootTileIdx];

  size_t i = 0;
  if(wordExists(str, wc)){ //Avoid adding duplicates
    return;
  }

  if(wc->wordCount == wc->buffSize){
    wc->buffSize  += DEFAULT_BUFFER_SIZE;
    wc->words = realloc(wc->words, wc->buffSize * sizeof(Word));

    if(wc->words == NULL){
      fprintf(stderr, "Failed to create more space for words!\n");
      exit(EXIT_FAILURE);
    }

    for(i = (wc->buffSize - DEFAULT_BUFFER_SIZE); i < wc->buffSize; i++){
      wc->words[i].word = calloc(config.MAX_WORD_LENGTH+1, sizeof(char));
      if(wc->words[i].word == NULL){
        fprintf(stdout, "Failed to allocate more memory for words\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  size_t len = strlen(str);

  if(len < config.MIN_WORD_LENGTH){
    return;
  }

  wc->tileIndex = rootTileIdx;
  wc->words[wc->wordCount].len = len;
  strcpy(wc->words[wc->wordCount].word, str);

  if(wc->longestWordLen < len){
    //Keep track of longest word
    //This is useful for printing word columns later
    wc->longestWordLen = len;
  }

  wc->wordCount++;
}

void blitzkrieg(){
  Tile *tiles = NULL;
  Board *board = NULL;
  loadConfig();
  config = getConfig();
  init(&tiles, &board);
  initFinder(board);

  start(board);

  freeWordColumns(board);
  clearTiles(tiles);
  clearBoard(board);
  unloadFinder();
}
