#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexis.h"
#include "avltree.h"
#include "config.h"

#define LEXIS_PATH "lexis"
#define BUFFER_SIZE 1024

static AvlTree lexisTree;

void loadLexis(){
  Config config = getConfig();
  lexisTree = MakeEmpty(NULL);
  FILE *fp = NULL;
  char word[BUFFER_SIZE];
  char *tmp = NULL;
  size_t len = 0;

  fp = fopen(LEXIS_PATH, "r");

  if(fp == NULL){
    fprintf(stderr, "Failed to open lexis file \"%s\"\n", LEXIS_PATH);
    exit(EXIT_FAILURE);
  }

  while(fscanf(fp, "%s", word) == 1){
    len = strlen(word);

    if(len <= config.MAX_WORD_LENGTH){
      tmp = malloc((config.MAX_WORD_LENGTH+1) * sizeof(char));

      if(tmp == NULL){
        fprintf(stderr, "Failed to allocate memory when loading lexis\n");
        exit(EXIT_FAILURE);
      }

      strncpy(tmp, word, config.MAX_WORD_LENGTH);
      lexisTree = Insert((void *)tmp, lexisTree);
    }
  }
  fclose(fp);
}

bool isWord(char *letters){
  return !(Find((void *)letters, lexisTree) == NULL);
}

void unloadLexis(){
  MakeEmpty(lexisTree);
}
