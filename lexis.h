#ifndef __LEXIS_H__
#define __LEXIS_H__

#include <stdbool.h>

#define MAX_WORD_LEN 8

void loadLexis();
bool isWord(char *letters);
void unloadLexis();

#endif /* __LEXIS_H__ */
