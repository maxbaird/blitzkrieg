#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexis.h"
#include "avltree.h"

#define WORD_AMT 6
#define WORD_LEN 32

#define LEXIS "lexis"
#define MAX_WORD_LEN 16
#define BUFFER_SIZE 128

static AvlTree lexis;


void loadLexis(){
  lexis = MakeEmpty(NULL);
  FILE *fp = NULL;
  char word[BUFFER_SIZE];
  char *tmp = NULL;
  size_t len = 0;
  size_t count = 0;

  fp = fopen(LEXIS, "r");

  if(fp == NULL){
    fprintf(stderr, "Failed to open lexis file \"%s\"\n", LEXIS);
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
      lexis = Insert((void *)tmp, lexis);
      count++;
    }
  }
  fprintf(stdout, "Loaded %zu words\n", count);
  fclose(fp);
}

bool isWord(char *letters){
  fprintf(stdout, "Looking for %s\n", letters);
  //if(Find((void *)letters, lexis) == NULL){
  //  fprintf(stdout, "Found nothing\n");
  //}else{
  //fprintf(stdout, "Found: %s\n", (char *)Retrieve(Find((void *)letters, lexis)));
  //}
  return !(Find((void *)letters, lexis) == NULL);
}

void unloadLexis(){
  MakeEmpty(lexis);
}
