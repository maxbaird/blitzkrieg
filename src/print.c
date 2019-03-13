#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "print.h"
#include "board.h"
#include "blitzkrieg.h"
#include "config.h"

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

#define SPACE_BETWEEN_COLUMNS 2 //Space between word columns

static Config config;
static size_t LEN_LONGEST_WORD;

static size_t pad(WordColumn *wc, size_t len){
  if(wc->wordCount == 0){
    wc->longestWordLen = 4;
  }
  return (wc->longestWordLen - len) + SPACE_BETWEEN_COLUMNS;
}

static void printColumnHeaders(WordColumn *wc, size_t start, size_t end){
  size_t i = 0;
  char str[8] = {'\0'};

  for(i = start; i < end; i++){
    sprintf(str, "[%zu]", i+1);
    if(config.ENABLE_HIGHLIGHTING){
      fprintf(stdout, BOLDCYAN"[%zu]%*s"RESET, i+1, (int)pad(&wc[i], strlen(str)),"");
    }else{
      fprintf(stdout, "[%zu]%*s", i+1, (int)pad(&wc[i], strlen(str)),"");
    }
    memset(str, 0, sizeof str);
  }
  fprintf(stdout, "\n");
}

static size_t getLongestColumn(size_t start, size_t end, WordColumn *wc){
  size_t i = 0;
  size_t idx = 0;

  for(i = start; i < end; i++){
    if(wc[i].wordCount > idx){
      idx = wc[i].wordCount;
    }
  }
  return idx; //Return the index of the longest column
}

static int compareWords(const void *w1, const void *w2){
  Word *word1 = (Word *)w1;
  Word *word2 = (Word *)w2;

  if(config.SORT_DESCENDING){
    return (int)word2->len - (int)word1->len;
  }
  return (int)word1->len - (int)word2->len;
}

static void sortColumns(size_t boardSize, WordColumn *wc){
  size_t i = 0;

  for(i = 0; i < boardSize; i++){
    qsort(wc->words, wc->wordCount, sizeof(Word), compareWords);
    wc++;
  }
}

static void strlenLongestWord(WordColumn *wc, size_t boardSize){
  size_t i = 0;
  size_t len = 0;

  for(i = 0; i < boardSize; i++){
    if(wc->longestWordLen > len){
      len = wc->longestWordLen;
    }
    wc++; //Next column of words
  }
  LEN_LONGEST_WORD = len;
}

static void printWord(WordColumn *wc, Word *w, bool endColumn){
  bool highlightWord = false;
  size_t i = 0;
  size_t len = strlen(config.HIGHLIGHT_LETTERS);

   for(i = 0; i < len; i++){
     if(strchr(w->word, config.HIGHLIGHT_LETTERS[i]) != NULL){
       highlightWord = true;
       break;
     }
   }

  if(highlightWord && config.ENABLE_HIGHLIGHTING){
    fprintf(stdout, BOLDRED"%s%*s"RESET, w->word, endColumn ? 0: (int)pad(wc, w->len),"");
  }else{
    fprintf(stdout, "%s%*s", w->word, endColumn ? 0: (int)pad(wc, w->len),"");
  }
}

void printWords(Board *board, WordColumn *wc){
  config = getConfig();
  size_t i = 0;
  size_t j = 0;
  size_t k = 0;
  size_t numPrintedRows = 0;
  size_t longestColumn = 0;
  size_t boardSize = getBoardSize(board);
  size_t colHeaderStart = 0;
  size_t colHeaderEnd = colHeaderStart + config.WORD_COLUMNS_PER_ROW;

  strlenLongestWord(wc, boardSize);//Update longest word
  sortColumns(boardSize, wc); //Print longest words first

  for(i = 0; i < boardSize; i+=config.WORD_COLUMNS_PER_ROW){
    printColumnHeaders(wc, colHeaderStart, colHeaderEnd);

    //This is later used as a loop control to ensure that
    //all words of the longest column is printed
    longestColumn = getLongestColumn(colHeaderStart, colHeaderEnd, wc);
    numPrintedRows = 0;

    for(j = 0; j < longestColumn; j++){
      if(numPrintedRows == config.MAX_WORDS_PER_ROW){ //Reprint column headers if necessary
        numPrintedRows = 0;
        fprintf(stdout, "\n"); //Go to new line before reprinting column headers
        printColumnHeaders(wc, colHeaderStart, colHeaderEnd);
      }

      numPrintedRows++;

      //This loops prints a word under each column
      for(k = colHeaderStart; k < colHeaderEnd; k++){
        if(wc[k].wordCount > j){
          printWord(&wc[k], &wc[k].words[j], k==(colHeaderEnd-1));
        }else{//If the current column has no more words, print nothing
          fprintf(stdout, "%*s", (int)pad(&wc[k], 0),"");
        }
      }
      fprintf(stdout, "\n"); //Move to new line to print another row of words
    }
    fprintf(stdout, "\n"); //Force remaining of columns onto new line

    colHeaderStart += config.WORD_COLUMNS_PER_ROW; //More start column up by number of columns to print per line
    colHeaderEnd = colHeaderStart + config.WORD_COLUMNS_PER_ROW; //Move end column up also
    colHeaderEnd = colHeaderEnd >= boardSize ? boardSize : colHeaderEnd; //Keep the end to the size of the board
  }

  for(i = 0; i < (LEN_LONGEST_WORD + SPACE_BETWEEN_COLUMNS) * config.WORD_COLUMNS_PER_ROW; i++){
    fprintf(stdout, "+");
  }

  fprintf(stdout, "\n\n");
}
