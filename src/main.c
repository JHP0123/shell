#include "../include/prompt.h"
#include <unistd.h>
#include "../include/parser.h"
#include "../include/executor.h"

int main()
{
    char *cwd = NULL;
    char *input = NULL;
    int pipe_cnt = 0;
    int index = 0;
    size_t cwd_size = 0, input_size = 0;
    ssize_t line_size = 0;
    Token *tokens = NULL;
    Pipeline *pipeline = NULL;

    int create_pipe_success = -1;
    int **pipes = NULL;

    while(1)
    {
        // Print prompt and get input
        print_prompt(&cwd, &cwd_size);
        get_command(&input, &input_size, &line_size);        
        printf("%s", input);        
        if(*input == '\n')
        {
            printf("It is a newline character\n");
            break;
        }

        // tokenize
        input[line_size - 1] = '\0';
        tokenize(input, &tokens, &pipe_cnt); 

        index = 0;
        TokenType check_type = tokens[index].type;
        while(check_type != END)
        {
            printf("%s\n", tokens[index].value);
            index++;
            check_type = tokens[index].type;
        }

        // parsing
        pipeline = parser(&tokens, pipe_cnt);

        // print pipeline
        if(pipeline != NULL)
        {
            for(int i = 0; i < (*pipeline).cmd_count; i++)
            {
                for(int j = 0; j < ((*pipeline).commands)[i].argc + 1; j++)
                {
                    if((*pipeline).commands[i].argv[j][0] == '\0')
                        printf("NULL ");
                    printf("%s ", (*pipeline).commands[i].argv[j]);
                }
                printf("%d %d %d %d ", (*pipeline).commands[i].argc,
                                           (*pipeline).commands[i].input_fd,
                                           (*pipeline).commands[i].output_fd,
                                           (*pipeline).commands[i].redir);
                if((*pipeline).commands[i].redir_file == NULL)
                    printf("NULL\n");
                else
                    printf("%s\n", (*pipeline).commands[i].redir_file);
            }
        }

        // pipe 생성 테스트
        create_pipe_success = create_pipe(pipeline, &pipes);
        index = 0;
        if(create_pipe_success == 1)
        {
            for(; index < (*pipeline).cmd_count - 1; index++)
            {
                printf("pipe[%d][0]: %d     pipe[%d][1]: %d\n",
                    index, pipes[index][0], index, pipes[index][1]);
                close(pipes[index][0]);
                close(pipes[index][1]);
            }
        }
        else
            printf("create_pipe() failed\n");
        
        // 자원 정리
        index = 0;
        check_type = tokens[index].type;
        while(check_type != END)
        {
            free(tokens[index].value);
            index++;
            check_type = tokens[index].type;
        }

        free(tokens[index].value);
        free(input);
        free(tokens);
        for(int i = 0; i < (*pipeline).cmd_count; i++)
        {
            for(int j = 0; j < ((*pipeline).commands)[i].argc; j++)
            {
                free(((*pipeline).commands)[i].argv[j]);
            }
            free(((*pipeline).commands)[i].argv);
            free(((*pipeline).commands)[i].redir_file);
        }
        free((*pipeline).commands);
        free(pipeline);

        break;
    }
}