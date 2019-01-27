#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdbool.h>

#define FILE_NAME_LENGTH 64

typedef struct config{
  size_t MAX_WORD_LENGTH;
  size_t MIN_WORD_LENGTH;
  size_t MAX_WORDS_PER_ROW;
  size_t WORD_COLUMNS_PER_ROW;
  bool SORT_DESCENDING;
  bool REMOVE_MULTIPLE_COLUMN_DUPLICATES;
  char LEXIS_FILE_NAME[FILE_NAME_LENGTH];
}Config;

void readConfig();
Config getConfig();

#endif /* __CONFIG_H__ */
