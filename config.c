#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#define FILENAME "config.conf"
#define LINE_BUFFER 64

Config getConfig(){
  Config config = {9, 10, 16, true, "lexis"};
  FILE *fp = fopen(FILENAME, "r");
  char line[LINE_BUFFER] = {'\0'};

  if(fp == NULL){
    fprintf(stderr, "Failed to load config file \"%s\"\n", FILENAME);
    fprintf(stderr, "Using default configuration\n");
    return config;
  }

  while(fgets(line, LINE_BUFFER, fp) != NULL){
      //sscanf( 
  }

}
