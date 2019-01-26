#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "blitzkrieg.h"
#include "finder.h"
#include "board.h"
#include "lexis.h"

static Board *BOARD;
static bool INITIALIZED;
static size_t BOARD_SIZE;

typedef struct path{
  int root;
  int *traversePath;
}Path;

typedef struct threadInfo{
  Tile *tile;
  char *letter;
  Path *path;
  int depth;
}ThreadInfo;

static void checkInit(){
  if(!INITIALIZED){
    fprintf(stdout, "Finder not initialized\n");
    exit(EXIT_FAILURE);
  }
}

static bool canMove(Tile *t, Path *tilePath){
  if(t == NULL) return false;
  size_t i = 0;
  for(i = 0; i < BOARD_SIZE; i++){
    if(tilePath->traversePath[i] == t->id){
      return false;
    }
  }
  return true;
}

static void traverse(Tile *t, char *letters, Path *path, int depth){
  char *str = calloc(MAX_WORD_LEN+1, sizeof(char)); //Allocate one extra for \0
  Path *tilePath = malloc(sizeof(Path));
  tilePath->traversePath = malloc(BOARD_SIZE * sizeof(int));

  if(str == NULL || tilePath == NULL){
    fprintf(stderr, "Failed to allocate memory for board traversal!\n");
    exit(EXIT_FAILURE);
  }

  if(tilePath->traversePath == NULL){
    fprintf(stderr, "Failed to allocate memory for board traversal!\n");
    exit(EXIT_FAILURE);
  }

  memset(tilePath->traversePath, -1, sizeof(int) * BOARD_SIZE);

  if(path == NULL){ // path will be NULL for initial call
    tilePath->root = t->id; //Keep track of the starting tile
  }else{
    memcpy(tilePath->traversePath, path->traversePath, sizeof(int) * BOARD_SIZE);
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

static void *threadTraverse(void *info){
  ThreadInfo *threadInfo = (ThreadInfo *)info;
  traverse(threadInfo->tile, threadInfo->letter, threadInfo->path, threadInfo->depth);
  pthread_exit(NULL);
}

void initFinder(Board *board){
  BOARD = board;
  BOARD_SIZE = getBoardSize(board);
  loadLexis();

  INITIALIZED = true;
}

void findWords(){
  checkInit();

  /* Blitzkrieg is not inherently thread-safe, that being said, it is
   * worthy to keep in mind that the threads used here only work because
   * they perform read-only operations on the binary tree list and each
   * thread updates its own array of found words.
   */

  int rc = 0;
  pthread_t *traverseThread = malloc(BOARD_SIZE * sizeof(pthread_t));
  ThreadInfo *threadInfo = malloc(BOARD_SIZE * sizeof(ThreadInfo));

  if(traverseThread == NULL || threadInfo == NULL){
    fprintf(stderr, "Failed to allocate space for threads!\n");
    exit(EXIT_FAILURE);
  }

  /* Initialize and set thread detached attribute */
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  size_t i = 0;
  Tile *tile = BOARD->tiles;

  for(i = 0; i < BOARD_SIZE; i++){
    /* Set up argument for next thread */
    threadInfo[i].tile = &tile[i];
    threadInfo[i].letter = NULL;
    threadInfo[i].path = NULL;
    threadInfo[i].depth = 0;

    rc = pthread_create(&traverseThread[i], &attr, threadTraverse, (void *)&threadInfo[i]);

    if(rc != 0){
      fprintf(stderr, "Error; return code from pthread_create() is %d\n", rc);
      exit(EXIT_FAILURE);
    }
  }

  /* Free attribute and wait for other threads */
  pthread_attr_destroy(&attr);
  for(i = 0; i < BOARD_SIZE; i++){
    rc = pthread_join(traverseThread[i], NULL);

    if(rc != 0){
      fprintf(stderr, "Error; return code from pthread_join() is %d\n", rc);
      exit(EXIT_FAILURE);
    }
  }

  free(traverseThread);
  free(threadInfo);
}

void unloadFinder(){
  checkInit();
  unloadLexis();
  INITIALIZED = false;
}
