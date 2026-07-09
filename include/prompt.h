#ifndef PROMPT_H
#define PROMPT_H
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void print_prompt(char *cwd, size_t *cwd_size);
void get_command(char **input, size_t *input_size, ssize_t *line_size);

#endif