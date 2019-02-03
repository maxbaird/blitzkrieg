#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdbool.h>

#define PATH_LENGTH 256
#define HIGHLIGHT_LETTER_COUNT 64

typedef struct config{
  size_t MAX_WORD_LENGTH;
  size_t MIN_WORD_LENGTH;
  size_t MAX_WORDS_PER_ROW;
  size_t WORD_COLUMNS_PER_ROW;
  bool SORT_DESCENDING;
  bool ENABLE_HIGHLIGHTING;
  char HIGHLIGHT_LETTERS[HIGHLIGHT_LETTER_COUNT];
  char LEXIS_FILE_PATH[PATH_LENGTH];
}Config;

void loadConfig();
Config getConfig();

#endif /* __CONFIG_H__ */
