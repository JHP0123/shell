#include "../include/prompt.h"
#include <unistd.h>
#include "../include/parser.h"

int main()
{
    char *cwd = NULL;
    char *input = NULL;
    size_t cwd_size = 0, input_size = 0;
    ssize_t line_size = 0;
    Token *tokens = NULL;

    while(1)
    {
        // Print prompt and get input
        print_prompt(cwd, &cwd_size);
        get_command(&input, &input_size, &line_size);
        input[line_size - 1] = '\0';
        
        tokenize(input, &tokens);
        
        printf("%s", input);
        free(input);
        free(tokens);
        break;
    }
}