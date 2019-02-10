#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexis.h"
#include "trie.h"
#include "config.h"

#define BUFFER_SIZE 512
#define FORMAT_SIZE 8

static Trie lexisTrie;

void loadLexis(){
  Config config = getConfig();
  lexisTrie = TrieMakeEmpty(NULL);

  FILE *fp = NULL;
  char word[BUFFER_SIZE+1] = {'\0'};
  char *tmp = NULL;
  size_t len = 0;
  char fmt[FORMAT_SIZE] = {'\0'};
  int c = 0;

  fp = fopen(config.LEXIS_FILE_PATH, "r");

  if(fp == NULL){
    fprintf(stderr, "Failed to open lexis file \"%s\"\n", config.LEXIS_FILE_PATH);
    exit(EXIT_FAILURE);
  }

  tmp = calloc((config.MAX_WORD_LENGTH+1), sizeof(char));

  if(tmp == NULL){
    fprintf(stderr, "Failed to allocate memory when loading lexis\n");
    exit(EXIT_FAILURE);
  }

  snprintf(fmt, FORMAT_SIZE, "%c%zus", '%', (size_t)BUFFER_SIZE);

  while(fscanf(fp, fmt, word) == 1){
    len = strlen(word);

    if(len <= config.MAX_WORD_LENGTH){
      strncpy(tmp, word, config.MAX_WORD_LENGTH);
      TrieInsert(lexisTrie, tmp);
    }

    /*Consume extra characters that could not fit in word buffer */
    while((c = fgetc(fp)) != EOF && c != '\n');
  }

  fclose(fp);
  free(tmp);
}

bool isWord(char *letters, size_t length){
  return TrieSearch(lexisTrie, letters, length);
}

void unloadLexis(){
  lexisTrie = TrieFree(lexisTrie);
}
