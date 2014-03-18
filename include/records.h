#ifndef RECORDS_INCLUDE
#define RECORDS_INCLUDE

#include "iniparser.h"
#include "stdio.h"

void getRecords(int* score, int* level, int* lines, char pseudo[20]);
void setRecords(int score, int level, int lines, const char* pseudo);

#endif
