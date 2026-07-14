#include "../../include/prompt.h"

void print_prompt(char **cwd, size_t *cwd_size)
{
    *cwd = getcwd(*cwd, *cwd_size);
    if(*cwd == NULL)
    {
        perror("getwcwd에서 에러 발생: ");
        return;
    }
    printf("%s$ ", *cwd);
    free(*cwd);
}

void get_command(char **input, size_t *input_size, ssize_t *line_size)
{
    *line_size = getline(input, input_size, stdin);
}