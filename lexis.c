#include <stdio.h>
#include <stdlib.h>
#include "lexis.h"
#include "avltree.h"

#define WORD_AMT 6
#define WORD_LEN 32

static AvlTree lexis;

void loadLexis(){
  char word_arr[WORD_AMT][WORD_LEN] = {"this", "is", "aaa", "test", "hello", "xob"};
  lexis = MakeEmpty(NULL);

  int i;
  for(i = 0; i < WORD_AMT; i++){
    lexis = Insert((void *)&word_arr[i], lexis);
  }

  fprintf(stdout, "Found: %s\n", (char *)Retrieve(Find("xob", lexis)));
}

bool isWord(char *letters){
  (void)letters; // avoid warning for now

  return true;
}

void unloadLexis(){
  MakeEmpty(lexis);
}
