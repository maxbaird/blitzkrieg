#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include "config.h"

#define FILENAME "config.conf"
#define LINE_BUFFER                                     128
#define FORMAT_BUFFER                                   8

#define LONGEST_WORD_LENGTH                             16
#define MAX_WORD_COLUMNS_PER_ROW                        16

/* Defaults */
#define DEFAULT_MAX_WORD_LENGTH                         9
#define DEFAULT_MIN_WORD_LENGTH                         2
#define DEFAULT_MAX_WORDS_PER_ROW                       10
#define DEFAULT_WORD_COLUMNS_PER_ROW                    16
#define DEFAULT_SORT_DESCENDING                         true
#define DEFAULT_ENABLE_HIGHLIGHTING                     true
#define DEFAULT_HIGHLIGHT_LETTERS                       "ab"//"xqzj"
#define DEFAULT_LEXIS_FILE_PATH                         "lexis"

static Config config = {DEFAULT_MAX_WORD_LENGTH,
                        DEFAULT_MIN_WORD_LENGTH,
                        DEFAULT_MAX_WORDS_PER_ROW,
                        DEFAULT_WORD_COLUMNS_PER_ROW,
                        DEFAULT_SORT_DESCENDING,
                        DEFAULT_ENABLE_HIGHLIGHTING,
                        DEFAULT_HIGHLIGHT_LETTERS,
                        DEFAULT_LEXIS_FILE_PATH};

static const char* MAX_WORD_LENGTH        = "MAX_WORD_LENGTH";
static const char* MIN_WORD_LENGTH        = "MIN_WORD_LENGTH";
static const char* MAX_WORDS_PER_ROW      = "MAX_WORDS_PER_ROW";
static const char* WORD_COLUMNS_PER_ROW   = "WORD_COLUMNS_PER_ROW";
static const char* SORT_DESCENDING        = "SORT_DESCENDING";
static const char* ENABLE_HIGHLIGHTING    = "ENABLE_HIGHLIGHTING";
static const char* HIGHLIGHT_LETTERS      = "HIGHLIGHT_LETTERS";
static const char* LEXIS_FILE_PATH        = "LEXIS_FILE_PATH";

typedef struct configFound{
  bool MAX_WORD_LENGTH;
  bool MIN_WORD_LENGTH;
  bool MAX_WORDS_PER_ROW;
  bool WORD_COLUMNS_PER_ROW;
  bool SORT_DESCENDING;
  bool ENABLE_HIGHLIGHTING;
  bool HIGHLIGHT_LETTERS;
  bool LEXIS_FILE_PATH;
}ConfigFound;

static ConfigFound configFound = {false, false, false, false, false, false, false, false};

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

static void removeSpaces(char *str){
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

static void convertToLower(char *s){
  for(;*s;++s){
    *s = tolower(*s);
  }
}

static int getBoolean(char *str, bool *b){
  convertToLower(str);

  if(strcmp(str, "true") == 0){
    *b = true;
    return 0;
  }else if(strcmp(str, "false") == 0){
    *b = false;
    return 0;
  }
  return -1;
}

static void buildFormatStr(char *fmt, char *fmt_str, size_t fmt_len, size_t width){
  snprintf(fmt, fmt_len, "%c%zu%s", '%', width, fmt_str);
}

static bool populateConfig(char *line, char **err){
  char str[64] = {'\0'};
  char format[FORMAT_BUFFER] = {'\0'};
  int ret = 0;
  bool successfulRead = true;

  if(strcasestr(line, MAX_WORD_LENGTH) != NULL){
    configFound.MAX_WORD_LENGTH = true;
    buildFormatStr(format, "zu", sizeof format, 2);
    successfulRead = readVal(line, format, &config.MAX_WORD_LENGTH);
    *err = successfulRead ? NULL : strdup(MAX_WORD_LENGTH);
    fprintf(stdout, "Max len: %zu\n", config.MAX_WORD_LENGTH);
  }

  if(strcasestr(line, MIN_WORD_LENGTH) != NULL){
    configFound.MIN_WORD_LENGTH = true;
    buildFormatStr(format, "zu", sizeof format, 2);
    successfulRead = readVal(line, format, &config.MIN_WORD_LENGTH);
    *err = successfulRead ? NULL : strdup(MIN_WORD_LENGTH);
  }

  if(strcasestr(line, MAX_WORDS_PER_ROW) != NULL){
    configFound.MAX_WORDS_PER_ROW = true;
    buildFormatStr(format, "zu", sizeof format, 2);
    successfulRead = readVal(line, format, &config.MAX_WORDS_PER_ROW);
    *err = successfulRead ? NULL : strdup(MAX_WORDS_PER_ROW);
  }

  if(strcasestr(line, WORD_COLUMNS_PER_ROW) != NULL){
    configFound.WORD_COLUMNS_PER_ROW = true;
    buildFormatStr(format, "zu", sizeof format, 2);
    successfulRead = readVal(line, format, &config.WORD_COLUMNS_PER_ROW);
    *err = successfulRead ? NULL : strdup(WORD_COLUMNS_PER_ROW);
  }

  if(strcasestr(line, SORT_DESCENDING) != NULL){
    configFound.SORT_DESCENDING = true;
    buildFormatStr(format, "s", sizeof format, 5);
    successfulRead = readVal(line, format, str);
    if(successfulRead){
      ret = getBoolean(str, &config.SORT_DESCENDING);
      if(ret != 0){
        successfulRead = false;
      }
    }
    *err = successfulRead ? NULL : strdup(SORT_DESCENDING);
  }

  if(strcasestr(line, ENABLE_HIGHLIGHTING) != NULL){
    configFound.ENABLE_HIGHLIGHTING = true;
    buildFormatStr(format, "s", sizeof format, 5);
    successfulRead = readVal(line, format, str);
    fprintf(stdout, "value read: %s\n", str);
    if(successfulRead){
      ret = getBoolean(str, &config.ENABLE_HIGHLIGHTING);
      if(ret != 0){
        successfulRead = false;
      }
    }
    *err = successfulRead ? NULL : strdup(ENABLE_HIGHLIGHTING);
  }

  if(strcasestr(line, HIGHLIGHT_LETTERS) != NULL){
    configFound.HIGHLIGHT_LETTERS = true;
    buildFormatStr(format, "s", sizeof format, HIGHLIGHT_LETTER_COUNT);

    successfulRead = readVal(line, format, &config.HIGHLIGHT_LETTERS);
    *err = successfulRead ? NULL : strdup(HIGHLIGHT_LETTERS);
  }

  if(strcasestr(line, LEXIS_FILE_PATH) != NULL){
    configFound.LEXIS_FILE_PATH = true;
    buildFormatStr(format, "s", sizeof format, PATH_LENGTH);

    successfulRead = readVal(line, format, &config.LEXIS_FILE_PATH);
    *err = successfulRead ? NULL : strdup(LEXIS_FILE_PATH);
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

  if(strcmp(value, ENABLE_HIGHLIGHTING) == 0){
    config.SORT_DESCENDING = DEFAULT_ENABLE_HIGHLIGHTING;
  }

  if(strcmp(value, HIGHLIGHT_LETTERS) == 0){
    strcpy(config.LEXIS_FILE_PATH, DEFAULT_HIGHLIGHT_LETTERS);
  }

  if(strcmp(value, LEXIS_FILE_PATH) == 0){
    strcpy(config.LEXIS_FILE_PATH, DEFAULT_LEXIS_FILE_PATH);
  }
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

  if(access(config.LEXIS_FILE_PATH, R_OK) == -1){
    fprintf(stderr, "Lexis file \"%s\" not found. Defaulting to \"%s\"\n",
        config.LEXIS_FILE_PATH,
        DEFAULT_LEXIS_FILE_PATH);
    strncpy(config.LEXIS_FILE_PATH, DEFAULT_LEXIS_FILE_PATH, PATH_LENGTH);
  }
  fprintf(stdout, "\n");
}

static void checkAllConfigsFound(){
  if(!configFound.MAX_WORD_LENGTH){
    fprintf(stderr, "%s not found; defaulting to %d\n", MAX_WORD_LENGTH, DEFAULT_MAX_WORD_LENGTH);
  }

  if(!configFound.MIN_WORD_LENGTH){
    fprintf(stderr, "%s not found; defaulting to %d\n", MIN_WORD_LENGTH, DEFAULT_MIN_WORD_LENGTH);
  }

  if(!configFound.MAX_WORDS_PER_ROW){
    fprintf(stderr, "%s not found; defaulting to %d\n", MAX_WORDS_PER_ROW, DEFAULT_MAX_WORDS_PER_ROW);
  }

  if(!configFound.WORD_COLUMNS_PER_ROW){
    fprintf(stderr, "%s not found; defaulting to %d\n", WORD_COLUMNS_PER_ROW, DEFAULT_WORD_COLUMNS_PER_ROW);
  }

  if(!configFound.SORT_DESCENDING){
    fprintf(stderr, "%s not found; defaulting to %s\n", SORT_DESCENDING, DEFAULT_SORT_DESCENDING ? "True" : "False");
  }

  if(!configFound.ENABLE_HIGHLIGHTING){
    fprintf(stderr, "%s not found; defaulting to %s\n", ENABLE_HIGHLIGHTING, DEFAULT_ENABLE_HIGHLIGHTING ? "True" : "False");
  }

  if(!configFound.LEXIS_FILE_PATH){
    fprintf(stderr, "%s not found; defaulting to \"%s\"\n", LEXIS_FILE_PATH, DEFAULT_LEXIS_FILE_PATH);
  }
}

void loadConfig(){
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
    removeSpaces(line);

    if(line[0] == '#' || line[0] == '\n'){ //Ignore commented and empty lines
      continue;
    }

    success = populateConfig(line, &readErr);

    if(!success){
      fprintf(stderr, "Error reading \"%s\" value. Default configuration value will be used.\n", readErr);
      restoreDefaultConfig(readErr);
      free(readErr);
    }
  }
  fclose(fp);

  checkAllConfigsFound();
  validateConfig();
}

Config getConfig(){
  return config;
}
