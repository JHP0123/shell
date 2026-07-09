#ifndef PARSER_H
#define PARSER_H
#include "data_structure.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

void tokenize(char *input, Token **token);

#endif