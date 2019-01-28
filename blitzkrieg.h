#ifndef __BLITZKRIEG_H__
#define __BLITZKRIEG_H__

#include "config.h"

typedef struct word{
  size_t len;
  char *word;
}Word;

typedef struct wordColumn{
  size_t tileIndex;
  size_t wordCount;
  size_t buffSize;
  size_t longestWordLen;
  Word *words;
}WordColumn;

void blitzkrieg(int argc, char *argv[]);
void addWord(char *str, int root);

#endif /* __BLITZKRIEG_H__ */
