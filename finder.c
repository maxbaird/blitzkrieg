#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "finder.h"
#include "board.h"
#include "lexis.h"

#define BUFFER_SIZE  512
#define MAX_WORD_LEN 17

typedef struct word{
  size_t len;
  char word[MAX_WORD_LEN];
}Word;

static Board *BOARD;
static bool INITIALIZED;
static size_t TILE_COUNT;
static Word *WORDS;
static size_t BUFFSIZE;
static size_t WORD_COUNT;

static void reset(){
  WORD_COUNT = 0;
  BUFFSIZE = BUFFER_SIZE;

  WORDS = realloc(WORDS, BUFFSIZE * sizeof(Word));

  if(WORDS == NULL){
    fprintf(stderr, "Resetting words failed!\n");
    exit(EXIT_FAILURE);
  }
  memset(WORDS, 0, BUFFSIZE * sizeof(Word));
}

static int compareWords(const void *w1, const void *w2){
  Word *word1 = (Word *)w1;
  Word *word2 = (Word *)w2;

  return (int)word2->len - (int)word1->len;
}

static void sortAndPrint(){
  qsort(WORDS, WORD_COUNT, sizeof(Word), compareWords);

  size_t i = 0;

  for(i = 0; i < WORD_COUNT; i++){
    fprintf(stdout, "%s\n", WORDS[i].word);
  }
}

static void addWord(char *str){
  if(WORD_COUNT >= BUFFSIZE - 1){
    BUFFSIZE += BUFFSIZE;
    WORDS = realloc(WORDS, BUFFSIZE * sizeof(Word));
    if(WORDS == NULL){
      fprintf(stderr, "Failed to create more space for words!\n");
      exit(EXIT_FAILURE);
    }
  }

  size_t len = strlen(str);

  if(len >= 2){ //Ignore one letter words
    WORDS[WORD_COUNT].len = strlen(str);
    strcpy(WORDS[WORD_COUNT].word, str);
  }else{
    WORD_COUNT--; //Reduce word count if current word is ignored
  }
}

static void checkInit(){
  if(!INITIALIZED){
    fprintf(stdout, "Finder not initialized\n");
    exit(EXIT_FAILURE);
  }
}

static bool canMove(Tile *t, int *tilePath){
  if(t == NULL) return false;
  size_t i = 0;
  for(i = 0; i < TILE_COUNT; i++){
    if(tilePath[i] == t->id){
      return false;
    }
  }
  return true;
}

static void traverse(Tile *t, char *letters, int *path){
  char *str = calloc(TILE_COUNT+1, sizeof(char)); //Allocate one extra for \0
  int *tilePath = calloc(TILE_COUNT, sizeof(int));

  if(str == NULL || tilePath == NULL){
    fprintf(stderr, "Failed to allocate memory for board traversal!\n");
    exit(EXIT_FAILURE);
  }

  if(path != NULL){ // path will be NULL for initial call
    memcpy(tilePath, path, sizeof(int) * TILE_COUNT);
  }

  tilePath[t->id] = t->id;

  if(letters != NULL){
    strcpy(str, letters);
    str[strlen(letters)] = t->letter;
  }else{
    str[0]=t->letter; //Handle initial traversal call
  }

  if(isWord(str)){
    addWord(str);
    WORD_COUNT++;
  }

  if(canMove(t->N, tilePath)){traverse(t->N, str, tilePath);}
  if(canMove(t->S, tilePath)){traverse(t->S, str, tilePath);}
  if(canMove(t->E, tilePath)){traverse(t->E, str, tilePath);}
  if(canMove(t->W, tilePath)){traverse(t->W, str, tilePath);}
  if(canMove(t->NE, tilePath)){traverse(t->NE, str, tilePath);}
  if(canMove(t->SE, tilePath)){traverse(t->SE, str, tilePath);}
  if(canMove(t->SW, tilePath)){traverse(t->SW, str, tilePath);}
  if(canMove(t->NW, tilePath)){traverse(t->NW, str, tilePath);}

  free(str);
  free(tilePath);
}

void initFinder(Board *board){
  BOARD = board;
  TILE_COUNT = BOARD->dimension.height * BOARD->dimension.width;
  loadLexis();

  BUFFSIZE = BUFFER_SIZE;
  WORD_COUNT = 0;

  WORDS = malloc(BUFFSIZE * sizeof(Word));

  if(WORDS == NULL){
    fprintf(stderr, "Failed to allocate memory for found words\n");
    exit(EXIT_FAILURE);
  }

  INITIALIZED = true;
}

void findWords(){
  checkInit();

  size_t i = 0;
  Tile *tile = BOARD->tiles;

  for(i = 0; i < TILE_COUNT; i++){
    traverse(tile, NULL, NULL);
    tile++;
  }
  sortAndPrint();
  reset();
}

void unloadFinder(){
  checkInit();
  unloadLexis();
  free(WORDS);
  INITIALIZED = false;
}
