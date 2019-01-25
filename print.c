#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "print.h"
#include "board.h"
#include "blitzkrieg.h"

#define RESET       "\033[0m"              /* Reset to default */
#define BLACK       "\033[30m"             /* Black */
#define RED         "\033[31m"             /* Red */
#define GREEN       "\033[32m"             /* Green */
#define YELLOW      "\033[33m"             /* Yellow */
#define BLUE        "\033[34m"             /* Blue */
#define MAGENTA     "\033[35m"             /* Magenta */
#define CYAN        "\033[36m"             /* Cyan */
#define WHITE       "\033[37m"             /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define WORDS_PER_ROW 10

static size_t padding(size_t len){
  return (MAX_WORD_LEN - len) + 2;
}

static void printColumnHeaders(size_t start, size_t end){
  size_t i = 0;
  char str[8] = {'\0'};

  for(i = start; i < end; i++){
    sprintf(str, "[%zu]", i+1);
    fprintf(stdout, BOLDCYAN"[%zu]%*s"RESET, i+1, (int)padding(strlen(str)),"");
    memset(str, 0, sizeof str);
  }
  fprintf(stdout, "\n");
}

static size_t longestList(size_t start, size_t end, WordColumn *wc){
  size_t i = 0;
  size_t idx = 0;

  for(i = start; i < end; i++){
    if(wc[i].wordCount > idx){
      idx = wc[i].wordCount;
    }
  }
  return idx;
}

static int compareWords(const void *w1, const void *w2){
  Word *word1 = (Word *)w1;
  Word *word2 = (Word *)w2;

  return (int)word2->len - (int)word1->len;
}

static void sortColumns(size_t boardSize, WordColumn *wc){
  size_t i = 0;

  for(i = 0; i < boardSize; i++){
    qsort(wc->words, wc->wordCount, sizeof(Word), compareWords);
    wc++;
  }
}

void printWords(Board *board, WordColumn *wc){
  size_t colsPerLine = 16; //pass this as argument to this function

  //WordColumn *wc = WORD_COLUMNS;
  size_t i = 0;
  size_t j = 0;
  size_t k = 0;
  size_t rowCount = 0;
  size_t longestColumn = 0;
  size_t boardSize = getBoardSize(board);
  size_t colHeaderStart = 0;
  size_t colHeaderEnd = colHeaderStart + colsPerLine;

  sortColumns(boardSize, wc);

  for(i = 0; i < boardSize; i+=colsPerLine){
    printColumnHeaders(colHeaderStart, colHeaderEnd);
    longestColumn = longestList(colHeaderStart, colHeaderEnd, wc);

    for(j = 0; j < longestColumn; j++){
      if(rowCount == WORDS_PER_ROW){
        rowCount = 0;
        fprintf(stdout, "\n");
        printColumnHeaders(colHeaderStart, colHeaderEnd);
      }rowCount++;
      for(k = colHeaderStart; k < colHeaderEnd; k++){
        if(wc[k].wordCount > j){
          size_t len = strlen(wc[k].words[j].word);
          fprintf(stdout, "%s%*s", wc[k].words[j].word, (k==(colHeaderEnd)-1) ? 0: (int)padding(len),"");
        }else{
          fprintf(stdout, "%*s", (int)padding(0),"");
        }
      }
      fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");

    colHeaderStart += colsPerLine;
    colHeaderEnd = colHeaderStart + colsPerLine;
    colHeaderEnd = colHeaderEnd >= boardSize ? boardSize : colHeaderEnd;
  }
}
