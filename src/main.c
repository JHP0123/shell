#include "../include/prompt.h"
#include <unistd.h>
#include "../include/parser.h"
#include "../include/executor.h"

int main()
{
    char *cwd = NULL;
    char *input = NULL;
    int index = 0;
    size_t cwd_size = 0, input_size = 0;
    ssize_t line_size = 0;

    TokenType check_type = null;
    Token *tokens = NULL;
    int token_capacity = 10;
    Pipeline *pipeline = NULL;

    int pipe_cnt = -1;
    int **pipes = NULL;
    int pipe_capacity = 10;

    Joblist joblist;
    joblist.job_count = 0;
    joblist.job_list = NULL;
    int job_capacity = 10;

    while(1)
    {
        // printf prompt and get input
        print_prompt(&cwd, &cwd_size);
        get_command(&input, &input_size, &line_size);
        if(*input == '\n')
        {
            free(input);    // 다시 입력을 받아야 하므로 input을 free하고
            input = NULL;   // 초기화한다
            printf("It is a newline character\n");
            continue;
        }
        // - 을 입력하면 while을 빠져나가고 shell의 종료 수순으로 들어간다
        else if(input[0] == '-')
            break;
        
        // tokenize
        input[line_size - 1] = '\0';
        tokenize(input, &tokens, &pipe_cnt, &token_capacity);

        // tokenize를 실패하면 prompt를 출력하고 다시 입력을 받으러 간다
        if(tokens == NULL)
            continue;
        
        // test tokenize()
        index = 0;
        check_type = tokens[index].type;
        while(check_type != END)
        {
            printf("%s\n", tokens[index].value);
            index++;
            check_type = tokens[index].type;
        }
        printf("tokenize test\n");

        // input에 새로운 입력을 받기 위해 free
        free(input);
        input = NULL;
    }

    // shell에서 명령어를 정상적으로 실행하다가 -(exit)을 하게 되면
    // Token tokens[]의 char *value 배열 메모리 free 한다
    if(tokens != NULL)
    {
        index = 0;
        check_type = tokens[index].type;
        while(check_type != END)
        {
            free(tokens[index].value);
            printf("tokens[%d] freed\n", index);
            index++;
            check_type = tokens[index].type;
        }
    }

    // Token tokens[] 배열 free
    free(tokens);

    // shell을 종료하면 input 배열 free
    free(input);
}























//     while(1)
//     {
//         // Print prompt and get input
//         print_prompt(&cwd, &cwd_size);
//         get_command(&input, &input_size, &line_size);        
//         printf("%s", input);        
//         if(*input == '\n')
//         {
//             printf("It is a newline character\n");
//             break;
//         }

//         // tokenize
//         input[line_size - 1] = '\0';
//         tokenize(input, &tokens, &pipe_cnt, &token_capacity); 

//         index = 0;
//         TokenType check_type = tokens[index].type;
//         while(check_type != END)
//         {
//             printf("%s\n", tokens[index].value);
//             index++;
//             check_type = tokens[index].type;
//         }

//         // parsing
//         pipeline = parser(&tokens, pipe_cnt);

//         // print pipeline
//         if(pipeline != NULL)
//         {
//             for(int i = 0; i < (*pipeline).cmd_count; i++)
//             {
//                 for(int j = 0; j < ((*pipeline).commands)[i].argc + 1; j++)
//                 {
//                     if((*pipeline).commands[i].argv[j][0] == '\0')
//                         printf("NULL ");
//                     printf("%s ", (*pipeline).commands[i].argv[j]);
//                 }
//                 printf("%d %d %d %d ", (*pipeline).commands[i].argc,
//                                            (*pipeline).commands[i].input_fd,
//                                            (*pipeline).commands[i].output_fd,
//                                            (*pipeline).commands[i].redir);
//                 if((*pipeline).commands[i].redir_file == NULL)
//                     printf("NULL\n");
//                 else
//                     printf("%s\n", (*pipeline).commands[i].redir_file);
//             }
//         }

//         // pipe 생성 테스트
//         pipe_cnt = create_pipe(pipeline, &pipes, &pipe_capacity);
//         index = 0;
//         if(pipe_cnt >= 0)
//         {
//             for(; index < pipe_cnt; index++)
//             {
//                 printf("pipe[%d][0]: %d     pipe[%d][1]: %d\n",
//                     index, pipes[index][0], index, pipes[index][1]);
//             }
//         }
//         else
//         {
//             printf("create_pipe() failed\n");
//             // while 시작으로 다시 돌아가야 됨.
//         }

//         // 자원 정리
//         // pipes fd close 정리
//         if(pipe_cnt >= 0)
//         {
//             for(int i = 0; i < pipe_cnt; i++)
//             {
//                 if(pipes[i][0] != -1 && pipes[i][1] != -1)
//                 {
//                     close(pipes[i][0]);
//                     close(pipes[i][1]);
//                 }
//                 free(pipes[i]);
//             }
//         }

//         // token.value 정리
//         index = 0;
//         check_type = tokens[index].type;
//         while(check_type != END)
//         {
//             free(tokens[index].value);
//             index++;
//             check_type = tokens[index].type;
//         }

//         free(tokens[index].value);
//         free(input);
//         for(int i = 0; i < (*pipeline).cmd_count; i++)
//         {
//             for(int j = 0; j < ((*pipeline).commands)[i].argc; j++)
//             {
//                 free(((*pipeline).commands)[i].argv[j]);
//             }
//             free(((*pipeline).commands)[i].argv);
//             free(((*pipeline).commands)[i].redir_file);
//         }
//         free((*pipeline).commands);
//         free(pipeline);

//         break;
//     }

//     // pipes 배열 정리
//     free(pipes);
//     printf("pipe array free() complete\n");

//     free(tokens);
//     printf("tokens array free completed\n");
// }