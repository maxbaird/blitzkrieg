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

#define DEFAULT_BUFFER_SIZE 32

typedef struct word{
  size_t len;
  char word[MAX_WORD_LEN+1];
}Word;

typedef struct wordColumn{
  size_t tileIndex;
  size_t wordCount;
  size_t buffSize;
  Word *words;
}WordColumn;

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

  if(len < MAX_LETTERS){
    fprintf(stderr, "%d letters needed; %zu entered!\n", MAX_LETTERS, len);
    return false;
  }

  for(i = 0; i < len; i++){
    input[i] = tolower(input[i]);

    if(!isalpha(input[i])){
      fprintf(stderr, "invalid character '%c'. Only letters allowed.\n", input[i]);
      consumeNewline();
      return false;
    }
  }
  return true;
}

static void printWords(Board *board){
  WordColumn *wc = WORD_COLUMNS;
  size_t i = 0;
  size_t j = 0;

  for(i = 0; i < getBoardSize(board); i++){
    fprintf(stdout, "[%zu]\n", wc->tileIndex + 1);

    for(j = 0; j < wc->wordCount; j++){
      fprintf(stdout, "%s\n", wc->words[j].word);
    }
    fprintf(stdout, "\n");
    wc++; //Go to next column of words
  }
}

static void reset(Board *board){
  WordColumn *wc = WORD_COLUMNS;
  size_t i = 0;
  size_t boardSize = getBoardSize(board);

  for(i = 0; i < boardSize; i++){
    wc->tileIndex = 0;
    wc->buffSize = DEFAULT_BUFFER_SIZE;
    wc->words = realloc(wc->words, DEFAULT_BUFFER_SIZE * sizeof(Word));
    wc->wordCount = 0;

    if(wc->words == NULL){
      fprintf(stderr, "Out of memory resetting word memory\n");
      exit(EXIT_FAILURE);
    }
    memset(wc->words, 0, DEFAULT_BUFFER_SIZE * sizeof(Word));

    wc++; //Go to next column of words
  }
}

static void freeWordColumns(Board *board){
  WordColumn *wc = WORD_COLUMNS;
  size_t i = 0;
  size_t boardSize = getBoardSize(board);

  for(i = 0; i < boardSize; i++){
   free(wc->words);
   wc++; //Move to next word column
  }

  free(WORD_COLUMNS);
}

static void start(Board *board){
  char letters[MAX_LETTERS+1] = {'\0'};
  char *res = NULL;

  for(;;){
    fprintf(stdout, "\n%s", PROMPT);

    res = fgets(letters, MAX_LETTERS+1, stdin);

    if(res == NULL){break;}
    if(!inputValid(letters)){continue;}

    placeLetters(board, letters);
    findWords();
    printWords(board);
    reset(board);
    consumeNewline();
  }

  fprintf(stdout, "\n");
}

static void init(Tile **tiles, Board **board){
  *tiles = makeTiles(HEIGHT * WIDTH);
  *board = makeBoard(*tiles, HEIGHT, WIDTH);
  size_t i = 0;
  size_t boardSize = getBoardSize(*board);

  WORD_COLUMNS = malloc(boardSize * sizeof(WordColumn));

  if(WORD_COLUMNS == NULL){
    fprintf(stderr, "Failed to allocate memory for word column\n");
    exit(EXIT_FAILURE);
  }

  for(i = 0; i < boardSize; i++){
    WORD_COLUMNS[i].tileIndex = 0;
    WORD_COLUMNS[i].buffSize = DEFAULT_BUFFER_SIZE;
    WORD_COLUMNS[i].words = calloc(DEFAULT_BUFFER_SIZE, sizeof(Word));
    WORD_COLUMNS[i].wordCount = 0;

    if(WORD_COLUMNS[i].words == NULL){
      fprintf(stderr, "Failed to allocate memory for word\n");
      exit(EXIT_FAILURE);
    }
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

  if(wordExists(str, wc)){ //Avoid adding duplicates
    return;
  }

  if(wc->wordCount >= wc->buffSize - 1){
    wc->buffSize  += wc->buffSize;
    wc->words = realloc(wc->words, wc->buffSize * sizeof(Word));

    if(wc->words == NULL){
      fprintf(stderr, "Failed to create more space for words!\n");
      exit(EXIT_FAILURE);
    }
  }

  size_t len = strlen(str);

  if(len <= 2){ //Ignore one letter words
    return;
  }

  wc->tileIndex = rootTileIdx;
  wc->words[wc->wordCount].len = len;
  strcpy(wc->words[wc->wordCount].word, str);
  wc->wordCount++;
}

void blitzkrieg(){
  Tile *tiles = NULL;
  Board *board = NULL;
  init(&tiles, &board);

  //if(tiles == NULL){
  //  fprintf(stdout, "Some thing wrong\n");
  //}else{
  //  fprintf(stdout, "tiles All good\n");
  //}

  //if(board == NULL){
  //  fprintf(stdout, "Something wrong here\n");
  //}else{
  //  fprintf(stdout, "board All good\n");
  //}
  initFinder(board);

  start(board);
  //placeLetters(board, "itsnuelatkudadls");
  //findWords();

  freeWordColumns(board);
  clearTiles(tiles);
  clearBoard(board);
  unloadFinder();
}
