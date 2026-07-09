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
        printf("%s", input);        
        if(*input == '\n')
        {
            printf("It is a newline character\n");
            break;
        }
        input[line_size - 1] = '\0';
        tokenize(input, &tokens); 

        int index = 0;
        TokenType check_type = tokens[index].type;
        while(check_type != END)
        {
            printf("%s\n", tokens[index].value);
            free(tokens[index].value);
            index++;
            check_type = tokens[index].type;
        }

        free(tokens[index].value);
        free(input);
        free(tokens);
        break;
    }
}