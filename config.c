#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "config.h"

#define FILENAME "config.conf"
#define LINE_BUFFER 128

//typedef enum type{intVal, boolVal, stringVal}Type;

//typedef struct config{
//  size_t MAX_WORD_LENGTH;
//  size_t MIN_WORD_LENGTH;
//  size_t MAX_WORDS_PER_ROW;
//  size_t WORD_COLUMNS_PER_ROW;
//  bool SORT_DESCENDING;
//  bool REMOVE_MULTIPLE_COLUMN_DUPLICATES;
//  char LEXIS_FILE[FILE_NAME_LENGTH];
//}Config;

static const char* MAX_WORD_LENGTH = "MAX_WORD_LENGTH";
static const char* MIN_WORD_LENGTH = "MIN_WORD_LENGTH";
static const char* MAX_WORDS_PER_ROW = "MAX_WORDS_PER_ROW";
static const char* WORD_COLUMNS_PER_ROW = "WORD_COLUMNS_PER_ROW";
static const char* SORT_DESCENDING = "SORT_DESCENDING";
static const char* REMOVE_MULTIPLE_COLUMN_DUPLICATES = "REMOVE_MULTIPLE_COLUMN_DUPLICATES";
static const char* LEXIS_FILE_NAME = "LEXIS_FILE_NAME";

static void sanitizeLine(char *str){
  char* i = str;
  char* j = str;

  while(*j != 0){
    *i = *j++;
    *i = toupper(*i);
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

  if(ret < 1){
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

static bool populateConfig(Config *config, char *line, char **err){
  char str[64] = {'\0'};
  int ret = 0;
  bool successfulRead = true;

  if(strstr(line, MAX_WORD_LENGTH) != NULL){
    successfulRead = readVal(line, "%zu", &config->MAX_WORD_LENGTH);
    *err = successfulRead ? NULL : strdup(MAX_WORD_LENGTH);
  }
  if(strstr(line, MIN_WORD_LENGTH) != NULL){
    successfulRead = readVal(line, "%zu", &config->MIN_WORD_LENGTH);
    *err = successfulRead ? NULL : strdup(MIN_WORD_LENGTH);
  }
  if(strstr(line, MAX_WORDS_PER_ROW) != NULL){
    successfulRead = readVal(line, "%zu", &config->MAX_WORDS_PER_ROW);
    *err = successfulRead ? NULL : strdup(MAX_WORDS_PER_ROW);
  }
  if(strstr(line, WORD_COLUMNS_PER_ROW) != NULL){
    successfulRead = readVal(line, "%zu", &config->WORD_COLUMNS_PER_ROW);
    *err = successfulRead ? NULL : strdup(WORD_COLUMNS_PER_ROW);
  }
  if(strstr(line, SORT_DESCENDING) != NULL){
    successfulRead = readVal(line, "%s", str);
    if(successfulRead){
      ret = getBoolean(str, &config->SORT_DESCENDING);
      if(ret != 0){
        successfulRead = false;
      }
    }
    *err = successfulRead ? NULL : strdup(SORT_DESCENDING);
  }
  if(strstr(line, REMOVE_MULTIPLE_COLUMN_DUPLICATES) != NULL){
    successfulRead = readVal(line, "%s", str);
    if(successfulRead){
      ret = getBoolean(str, &config->REMOVE_MULTIPLE_COLUMN_DUPLICATES);
      if(ret != 0){
        successfulRead = false;
      }
    }
    *err = successfulRead ? NULL : strdup(REMOVE_MULTIPLE_COLUMN_DUPLICATES);
  }
  if(strstr(line, LEXIS_FILE_NAME) != NULL){
    successfulRead = readVal(line, "%s", &config->LEXIS_FILE_NAME);
    *err = successfulRead ? NULL : strdup(LEXIS_FILE_NAME);
  }
  return successfulRead;
}

Config getConfig(){
  Config config = {9, 10, 16, true, "lexis"};
  FILE *fp = fopen(FILENAME, "r");
  char line[LINE_BUFFER] = {'\0'};
  bool success = true;
  char *readErr = NULL;

  if(fp == NULL){
    fprintf(stderr, "Failed to read config file \"%s\"\n", FILENAME);
    fprintf(stderr, "Using default configuration\n");
    return config;
  }

  while(fgets(line, LINE_BUFFER, fp) != NULL){
    sanitizeLine(line);

    if(line[0] == '#' || strlen(line) == 0){ //Ignore commented and empty lines
      continue;
    }

    success = populateConfig(&config, line, &readErr);

    if(!success){
      fprintf(stderr, "Error reading \"%s\" config. Default value will be used.\n", readErr);
      free(readErr);
    }
  }
  fprintf(stdout, "%zu\n", config.MAX_WORD_LENGTH);
  fprintf(stdout, "%zu\n", config.MIN_WORD_LENGTH);
  fprintf(stdout, "%zu\n", config.MAX_WORDS_PER_ROW);
  fprintf(stdout, "%zu\n", config.WORD_COLUMNS_PER_ROW);
  fclose(fp);
}

int main(){
  getConfig();
  return EXIT_SUCCESS;
}