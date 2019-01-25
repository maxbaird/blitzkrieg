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

/*static*/ bool inputValid(char *input){
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

static size_t padding(size_t len){
  return (MAX_WORD_LEN - len) + 2;
}

static void printColumnHeaders(size_t start, size_t end){
  size_t i = 0;
  char str[8] = {'\0'};

  for(i = start; i < end; i++){
    sprintf(str, "[%zu]", i+1);
    fprintf(stdout, "[%zu]%*s", i+1, (int)padding(strlen(str)),"");
    memset(str, 0, sizeof str);
  }
  fprintf(stdout, "\n");
}

static size_t longestList(size_t start, size_t end){
  WordColumn *wc = WORD_COLUMNS;
  size_t i = 0;
  size_t idx = 0;
  end = end >= 16 ? 16: end;

  for(i = start; i < end; i++){
    if(wc[i].wordCount > idx){
      idx = wc[i].wordCount;
    }
  }
  return idx;
}

static void printWords(Board *board){
  WordColumn *wc = WORD_COLUMNS;
  size_t i = 0;
  size_t j = 0;
  size_t k = 0;
  size_t longestColumn = 0;
  size_t boardSize = getBoardSize(board);
  size_t colsPerLine = 16; //pass this as argument to this function
  size_t colHeaderIdx = 0;

  for(i = 0; i < boardSize; i+=colsPerLine){
    printColumnHeaders(colHeaderIdx, colHeaderIdx+colsPerLine);
    longestColumn = longestList(colHeaderIdx, colHeaderIdx+colsPerLine);

    for(j = 0; j < longestColumn; j++){
      for(k = colHeaderIdx; k < colHeaderIdx+colsPerLine; k++){
        if(k == boardSize)break;
        if(wc[k].wordCount > j){
          size_t len = strlen(wc[k].words[j].word);
          fprintf(stdout, "%s%*s", wc[k].words[j].word, (k==(colHeaderIdx+colsPerLine)-1)?0:(int)padding(len),"");
        }else{
          fprintf(stdout, "%*s", (int)padding(0),"");
        }
      }
      fprintf(stdout, "\n");
    }
    colHeaderIdx += colsPerLine;
      fprintf(stdout, "\n");
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

    //placeLetters(board, "abcdefghijklmnop");
    placeLetters(board, letters);
    findWords();
    printWords(board);
    reset(board);
    consumeNewline();
    //break; //REMEMBER TO REMOVE THIS!!!
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

  WordColumn *wc = WORD_COLUMNS;

  for(i = 0; i < boardSize; i++){
    wc->tileIndex = 0;
    wc->buffSize = DEFAULT_BUFFER_SIZE;
    wc->words = calloc(DEFAULT_BUFFER_SIZE, sizeof(Word));
    wc->wordCount = 0;

    if(wc->words == NULL){
      fprintf(stderr, "Failed to allocate memory for word\n");
      exit(EXIT_FAILURE);
    }
    //memset(wc->words->word, 0, sizeof(char) * MAX_WORD_LEN+1);
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

  if(wordExists(str, wc)){ //Avoid adding duplicates
    return;
  }

  if(wc->wordCount >= (wc->buffSize-1)){
    wc->buffSize  += DEFAULT_BUFFER_SIZE;
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
  initFinder(board);

  start(board);
  //placeLetters(board, "itsnuelatkudadls");
  //findWords();

  freeWordColumns(board);
  clearTiles(tiles);
  clearBoard(board);
  unloadFinder();
}
