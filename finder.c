#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blitzkrieg.h"
#include "finder.h"
#include "board.h"
#include "lexis.h"

//#define BUFFER_DEFAULT_SIZE  512

//typedef struct word{
//  int rootIdx;
//  size_t len;
//  char word[MAX_WORD_LEN+1];
//}Word;
//
//typedef struct wordColumn{
//  size_t tileIndex;
//  Word *words;
//}WordColumn;

static Board *BOARD;
static bool INITIALIZED;
static size_t TILE_COUNT;
//static WordColumn *WORD_COLUMN;
//static Word *WORDS;
//static size_t BUFFSIZE;
//static size_t WORD_COUNT;

typedef struct path{
  int root;
  int *traversePath;
}Path;

//static void reset(){
//  WORD_COUNT = 0;
//  BUFFSIZE = BUFFER_DEFAULT_SIZE;
//
//  WORDS = realloc(WORDS, BUFFSIZE * sizeof(Word));
//
//  if(WORDS == NULL){
//    fprintf(stderr, "Resetting words failed!\n");
//    exit(EXIT_FAILURE);
//  }
//  memset(WORDS, 0, BUFFSIZE * sizeof(Word));
//}

//static int compareWords(const void *w1, const void *w2){
//  Word *word1 = (Word *)w1;
//  Word *word2 = (Word *)w2;
//
//  return (int)word1->len - (int)word2->len;
//}

//static void sortAndPrint(){
//  qsort(WORDS, WORD_COUNT, sizeof(Word), compareWords);
//
//  size_t i = 0;
//
//  for(i = 0; i < WORD_COUNT; i++){
//    fprintf(stdout, "%s \t: [%d]\n", WORDS[i].word, WORDS[i].rootIdx);
//  }
//}

//static bool wordExists(char *str){
//  size_t i = 0;
//
//  for(i = 0; i < WORD_COUNT; i++){
//    if(strcmp(str, WORDS[i].word) == 0){
//      return true;
//    }
//  }
//  return false;
//}

//static void addWord(char *str, int root){
//  if(wordExists(str)){ //Skip adding duplicates
//    return;
//  }
//
//  if(WORD_COUNT >= BUFFSIZE - 1){ //Determine if more space for words is required
//    BUFFSIZE += BUFFSIZE;
//    WORDS = realloc(WORDS, BUFFSIZE * sizeof(Word));
//    if(WORDS == NULL){
//      fprintf(stderr, "Failed to create more space for words!\n");
//      exit(EXIT_FAILURE);
//    }
//  }
//
//  size_t len = strlen(str);
//
//  if(len >= 2){ //Ignore one letter words
//    WORDS[WORD_COUNT].rootIdx = root;
//    WORDS[WORD_COUNT].len = len;
//    strcpy(WORDS[WORD_COUNT].word, str);
//    WORD_COUNT++;
//  }
//}

static void checkInit(){
  if(!INITIALIZED){
    fprintf(stdout, "Finder not initialized\n");
    exit(EXIT_FAILURE);
  }
}

static bool canMove(Tile *t, Path *tilePath){
  if(t == NULL) return false;
  size_t i = 0;
  for(i = 0; i < TILE_COUNT; i++){
    if(tilePath->traversePath[i] == t->id){
      return false;
    }
  }
  return true;
}

static void traverse(Tile *t, char *letters, Path *path, int depth){
  char *str = calloc(MAX_WORD_LEN+1, sizeof(char)); //Allocate one extra for \0
  Path *tilePath = malloc(sizeof(Path));
  tilePath->traversePath = malloc(TILE_COUNT * sizeof(int));

  if(str == NULL || tilePath == NULL){
    fprintf(stderr, "Failed to allocate memory for board traversal!\n");
    exit(EXIT_FAILURE);
  }

  if(tilePath->traversePath == NULL){
    fprintf(stderr, "Failed to allocate memory for board traversal!\n");
    exit(EXIT_FAILURE);
  }

  memset(tilePath->traversePath, -1, sizeof(int) * TILE_COUNT);

  if(path == NULL){ // path will be NULL for initial call
    tilePath->root = t->id; //Keep track of the starting tile
  }else{
    memcpy(tilePath->traversePath, path->traversePath, sizeof(int) * TILE_COUNT);
    tilePath->root = path->root;
  }

  tilePath->traversePath[t->id] = t->id;

  if(letters != NULL){
    strcpy(str, letters);
    str[strlen(letters)] = t->letter;
  }else{
    str[0]=t->letter; //Handle initial traversal call
  }

  if(isWord(str)){
    addWord(str, tilePath->root);
  }

  if(depth < MAX_WORD_LEN){ //Avoids finding all 16 combinations
    if(canMove(t->N, tilePath)){traverse(t->N, str, tilePath, depth+1);}
    if(canMove(t->S, tilePath)){traverse(t->S, str, tilePath, depth+1);}
    if(canMove(t->E, tilePath)){traverse(t->E, str, tilePath, depth+1);}
    if(canMove(t->W, tilePath)){traverse(t->W, str, tilePath, depth+1);}
    if(canMove(t->NE, tilePath)){traverse(t->NE, str, tilePath, depth+1);}
    if(canMove(t->SE, tilePath)){traverse(t->SE, str, tilePath, depth+1);}
    if(canMove(t->SW, tilePath)){traverse(t->SW, str, tilePath, depth+1);}
    if(canMove(t->NW, tilePath)){traverse(t->NW, str, tilePath, depth+1);}
  }

  free(str);
  free(tilePath->traversePath);
  free(tilePath);
}

void initFinder(Board *board){
  BOARD = board;
  TILE_COUNT = getBoardSize(board);
  loadLexis();

//  BUFFSIZE = BUFFER_DEFAULT_SIZE;
//  WORD_COUNT = 0;
//
//  WORD_COLUMN = malloc(TILE_COUNT * sizeof(WordColumn));
//
//  if(WORD_COLUMN == NULL){
//    fprintf(stderr, "Failed to allocate memory for word column\n");
//    exit(EXIT_FAILURE);
//  }
//
//  WORDS = calloc(BUFFSIZE, sizeof(Word));
//
//  if(WORDS == NULL){
//    fprintf(stderr, "Failed to allocate memory for found words\n");
//    exit(EXIT_FAILURE);
//  }

  INITIALIZED = true;
}

void findWords(){
  checkInit();

  size_t i = 0;
  Tile *tile = BOARD->tiles;

  for(i = 0; i < TILE_COUNT; i++){
    traverse(tile, NULL, NULL, 0);
    tile++;
  }

//  sortAndPrint();
//  reset();
}

void unloadFinder(){
  checkInit();
  unloadLexis();
//  free(WORDS);
  INITIALIZED = false;
}
