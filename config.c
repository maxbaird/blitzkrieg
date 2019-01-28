#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "config.h"

#define FILENAME "config.conf"
#define LINE_BUFFER 128

#define LONGEST_WORD_LENGTH                             16
#define MAX_WORD_COLUMNS_PER_ROW                        16

/* Defaults */
#define DEFAULT_MAX_WORD_LENGTH                         9
#define DEFAULT_MIN_WORD_LENGTH                         2
#define DEFAULT_MAX_WORDS_PER_ROW                       10
#define DEFAULT_WORD_COLUMNS_PER_ROW                    16
#define DEFAULT_SORT_DESCENDING                         true
#define DEFAULT_REMOVE_MULTIPLE_COLUMN_DUPLICATES       false
#define DEFAULT_LEXIS_FILE_NAME                         "lexis"

static Config config = {DEFAULT_MAX_WORD_LENGTH,
                        DEFAULT_MIN_WORD_LENGTH,
                        DEFAULT_MAX_WORDS_PER_ROW,
                        DEFAULT_WORD_COLUMNS_PER_ROW,
                        DEFAULT_SORT_DESCENDING,
                        DEFAULT_REMOVE_MULTIPLE_COLUMN_DUPLICATES,
                        DEFAULT_LEXIS_FILE_NAME
                        };

static const char* MAX_WORD_LENGTH = "MAX_WORD_LENGTH";
static const char* MIN_WORD_LENGTH = "MIN_WORD_LENGTH";
static const char* MAX_WORDS_PER_ROW = "MAX_WORDS_PER_ROW";
static const char* WORD_COLUMNS_PER_ROW = "WORD_COLUMNS_PER_ROW";
static const char* SORT_DESCENDING = "SORT_DESCENDING";
static const char* REMOVE_MULTIPLE_COLUMN_DUPLICATES = "REMOVE_MULTIPLE_COLUMN_DUPLICATES";
static const char* LEXIS_FILE_NAME = "LEXIS_FILE_NAME";

/*
 * Taken from: https://android.googlesource.com/platform/bionic/+/fe6338d/libc/string/strcasestr.c
 * Find the first occurrence of find in s, ignore case.
 */
char *strcasestr(const char *s, const char *find)
{
  char c, sc;
  size_t len;
  if ((c = *find++) != 0) {
    c = (char)tolower((unsigned char)c);
    len = strlen(find);
    do {
      do {
        if ((sc = *s++) == 0)
          return (NULL);
      } while ((char)tolower((unsigned char)sc) != c);
    } while (strncasecmp(s, find, len) != 0);
    s--;
  }
  return ((char *)s);
}

static void sanitizeLine(char *str){
  char* i = str;
  char* j = str;

  while(*j != 0){
    *i = *j++;
    if(!isspace(*i)){
      i++;
    }
  }
  *i = 0;
}

static bool readVal(char *line, char *format, void *val){
  char *token = NULL;
  const char delim[] = "=";
  int ret = 0;

  token = strtok(line, delim);
  token = strtok(NULL, delim); //Get value

  if(token == NULL){
    return false;
  }

  ret = sscanf(token, format, val);

  if(ret != 1){
    return false;
  }

  return true;
}

static int getBoolean(char *str, bool *b){
  if(strcmp(str, "TRUE") == 0){
    *b = true;
    return 0;
  }else if(strcmp(str, "FALSE") == 0){
    *b = false;
    return 0;
  }
  return -1;
}

static bool populateConfig(char *line, char **err){
  char str[64] = {'\0'};
  int ret = 0;
  bool successfulRead = true;

  if(strcasestr(line, MAX_WORD_LENGTH) != NULL){
    successfulRead = readVal(line, "%zu", &config.MAX_WORD_LENGTH);
    *err = successfulRead ? NULL : strdup(MAX_WORD_LENGTH);
  }

  if(strcasestr(line, MIN_WORD_LENGTH) != NULL){
    successfulRead = readVal(line, "%zu", &config.MIN_WORD_LENGTH);
    *err = successfulRead ? NULL : strdup(MIN_WORD_LENGTH);
  }

  if(strcasestr(line, MAX_WORDS_PER_ROW) != NULL){
    successfulRead = readVal(line, "%zu", &config.MAX_WORDS_PER_ROW);
    *err = successfulRead ? NULL : strdup(MAX_WORDS_PER_ROW);
  }

  if(strcasestr(line, WORD_COLUMNS_PER_ROW) != NULL){
    successfulRead = readVal(line, "%zu", &config.WORD_COLUMNS_PER_ROW);
    *err = successfulRead ? NULL : strdup(WORD_COLUMNS_PER_ROW);
  }

  if(strcasestr(line, SORT_DESCENDING) != NULL){
    successfulRead = readVal(line, "%s", str);
    if(successfulRead){
      ret = getBoolean(str, &config.SORT_DESCENDING);
      if(ret != 0){
        successfulRead = false;
      }
    }
    *err = successfulRead ? NULL : strdup(SORT_DESCENDING);
  }

  if(strcasestr(line, REMOVE_MULTIPLE_COLUMN_DUPLICATES) != NULL){
    successfulRead = readVal(line, "%s", str);
    if(successfulRead){
      ret = getBoolean(str, &config.REMOVE_MULTIPLE_COLUMN_DUPLICATES);
      if(ret != 0){
        successfulRead = false;
      }
    }
    *err = successfulRead ? NULL : strdup(REMOVE_MULTIPLE_COLUMN_DUPLICATES);
  }

  if(strcasestr(line, LEXIS_FILE_NAME) != NULL){
    successfulRead = readVal(line, "%s", &config.LEXIS_FILE_NAME);
    *err = successfulRead ? NULL : strdup(LEXIS_FILE_NAME);
  }

  return successfulRead;
}

static void restoreDefaultConfig(const char* value){
  if(strcmp(value, MAX_WORD_LENGTH) == 0){
    config.MAX_WORD_LENGTH = DEFAULT_MAX_WORD_LENGTH;
  }
  if(strcmp(value, MIN_WORD_LENGTH) == 0){
    config.MIN_WORD_LENGTH = DEFAULT_MIN_WORD_LENGTH;
  }
  if(strcmp(value, MAX_WORDS_PER_ROW) == 0){
    config.MAX_WORDS_PER_ROW = DEFAULT_MAX_WORDS_PER_ROW;
  }
  if(strcmp(value, WORD_COLUMNS_PER_ROW) == 0){
    config.WORD_COLUMNS_PER_ROW = DEFAULT_WORD_COLUMNS_PER_ROW;
  }
  if(strcmp(value, SORT_DESCENDING) == 0){
    config.SORT_DESCENDING = DEFAULT_SORT_DESCENDING;
  }
  if(strcmp(value, REMOVE_MULTIPLE_COLUMN_DUPLICATES) == 0){
    config.REMOVE_MULTIPLE_COLUMN_DUPLICATES = DEFAULT_REMOVE_MULTIPLE_COLUMN_DUPLICATES;
  }
  if(strcmp(value, LEXIS_FILE_NAME) == 0){
    strcpy(config.LEXIS_FILE_NAME, DEFAULT_LEXIS_FILE_NAME);
  }
}

static void printConfig(){ //TODO just for testing, remove later
  fprintf(stdout, "MAX_WORD_LENGTH = %zu\n", config.MAX_WORD_LENGTH);
  fprintf(stdout, "MIN_WORD_LENGTH = %zu\n", config.MIN_WORD_LENGTH);
  fprintf(stdout, "MAX_WORDS_PER_ROW = %zu\n", config.MAX_WORDS_PER_ROW);
  fprintf(stdout, "WORD_COLUMNS_PER_ROW = %zu\n", config.WORD_COLUMNS_PER_ROW);
  fprintf(stdout, "SORT_DESCENDING = %s\n", config.SORT_DESCENDING?"True":"False");
  fprintf(stdout, "REMOVE_MULTIPLE_COLUMN_DUPLICATES = %s\n", config.REMOVE_MULTIPLE_COLUMN_DUPLICATES?"True":"False");
  fprintf(stdout, "LEXIS_FILE_NAME = %s\n", config.LEXIS_FILE_NAME);
}

static void validateConfig(){
  if(config.MAX_WORD_LENGTH <= 0 || config.MAX_WORD_LENGTH > LONGEST_WORD_LENGTH){
    fprintf(stderr, "%s must be between 0 and %d. Defaulting to %d.\n",
        MAX_WORD_LENGTH,
        LONGEST_WORD_LENGTH,
        DEFAULT_MAX_WORD_LENGTH);

    config.MAX_WORD_LENGTH = DEFAULT_MAX_WORD_LENGTH;
  }

  if(config.MIN_WORD_LENGTH <= 0 || config.MIN_WORD_LENGTH >= LONGEST_WORD_LENGTH){
    fprintf(stderr, "%s must be between 0 and %d. Defaulting to %d.\n",
        MIN_WORD_LENGTH,
        LONGEST_WORD_LENGTH,
        DEFAULT_MIN_WORD_LENGTH);

    config.MIN_WORD_LENGTH = DEFAULT_MIN_WORD_LENGTH;
  }

  if(config.MIN_WORD_LENGTH >= config.MAX_WORD_LENGTH){
    fprintf(stderr, "%s must be less than %s. Defaulting to %d.\n",
        MIN_WORD_LENGTH,
        MAX_WORD_LENGTH,
        DEFAULT_MIN_WORD_LENGTH);

    config.MIN_WORD_LENGTH = DEFAULT_MIN_WORD_LENGTH;
  }

  if(config.MAX_WORDS_PER_ROW <= 0){
    fprintf(stderr, "%s cannot be less than 0. Defaulting to %d.\n",
        MAX_WORDS_PER_ROW,
        DEFAULT_MAX_WORDS_PER_ROW);

    config.MAX_WORDS_PER_ROW = DEFAULT_MAX_WORDS_PER_ROW;
  }

  if(config.WORD_COLUMNS_PER_ROW <= 0 || config.WORD_COLUMNS_PER_ROW > MAX_WORD_COLUMNS_PER_ROW){
    fprintf(stderr, "%s must be between 0 and %d. Defaulting to %d.\n",
        WORD_COLUMNS_PER_ROW,
        MAX_WORD_COLUMNS_PER_ROW,
        DEFAULT_WORD_COLUMNS_PER_ROW);

    config.WORD_COLUMNS_PER_ROW = DEFAULT_WORD_COLUMNS_PER_ROW;
  }
  fprintf(stdout, "\n");
}

void readConfig(){
  FILE *fp = fopen(FILENAME, "r");
  char line[LINE_BUFFER] = {'\0'};
  bool success = true;
  char *readErr = NULL;

  if(fp == NULL){
    fprintf(stderr, "Failed to read config file \"%s\"\n", FILENAME);
    fprintf(stderr, "Using default configuration\n");
    return;
  }

  while(fgets(line, LINE_BUFFER, fp) != NULL){
    sanitizeLine(line);

    if(line[0] == '#' || line[0] == '\n' || strlen(line) == 0){ //Ignore commented and empty lines
      continue;
    }

    success = populateConfig(line, &readErr);

    if(!success){
      fprintf(stderr, "Error reading \"%s\" value. Default configuration value will be used.\n", readErr);
      fflush(stderr);
      restoreDefaultConfig(readErr);
      free(readErr);
    }
  }
  fclose(fp);

  validateConfig();
}

Config getConfig(){
  return config;
}

int main(){
  readConfig();
  printConfig();
  return EXIT_SUCCESS;
}
