#ifndef __PRINT_H__
#define __PRINT_H__

#include "blitzkrieg.h"
#include "board.h"

#define DEFAULT_COLUMNS_PER_LINE 16

void printWords(Board *board, WordColumn *wc, size_t colsPerLine);

#endif /* __PRINT_H__ */
