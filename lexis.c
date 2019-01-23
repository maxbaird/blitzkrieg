#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexis.h"
#include "avltree.h"

#define LEXIS_PATH "lexis"
#define BUFFER_SIZE 128

static AvlTree lexisTree;

void loadLexis(){
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

  while(fgets(word, BUFFER_SIZE, fp) != NULL){
    //Remove newline
    char *pos = NULL;
    if((pos = strchr(word, '\n')) != NULL){
      *pos='\0';
    }

    len = strlen(word);
    if(len <= MAX_WORD_LEN){
      tmp = malloc((len+1) * sizeof(char));

      if(tmp == NULL){
        fprintf(stderr, "Failed to allocate memory when loading lexis\n");
        exit(EXIT_FAILURE);
      }

      sscanf(word, "%s", tmp);
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
