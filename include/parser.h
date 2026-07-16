#ifndef PARSER_H
#define PARSER_H
#include "data_structure.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

void tokenize(char *input, Token **token, int *pipe_cnt, int *token_capacity);
Pipeline *parser(Token **tokens, int pipe_cnt);

#endif