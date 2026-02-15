#ifndef CSV_PARSE_H
#define CSV_PARSE_H

#include "mode.h"

// Maximum line and token sizes
#define MAX_LINE 200000
#define MAX_TOKENS 5000

// CSV parsing functions
void parseRoadmapCSV(const char* filename);
void parseMetroCSV(const char* filename);
void parseBusCSV(const char* filename, Mode busMode);

#endif
