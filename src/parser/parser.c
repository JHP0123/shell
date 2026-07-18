#include "../../include/parser.h"

// 역할: 사용자의 input을 받아 token화를 한다
// token을 받아 Token[] 배열을 동적으로 만든다. 초반에는 token은 무조건 NULL이어야 한다
// Token[] 배열은 한번 만들면 계속 재사용한다. shell이 종료되면 main에서 마지막에 딱 한번 정리한다
// Token.value는 매번 free한다. 
// input은 사용자가 입력한 명령어
// pipe_cnt는 명령어의 pipe 개수를 갱신
// token_capacity는 default=10. 토큰이 10 이상이면 +10해서 Token[] 배열의 크기를 더 크게 재할당한다
void tokenize(char *input, Token **token, int *pipe_cnt, int *token_capacity)
{
    // pipe의 개수
    *pipe_cnt = 0;

    // 최대 토큰 수(token_capacity)를 초과하면 realloc를 한다
    int count = 0;

    //" "안에 있는 경우 
    bool in_quote = false;

    // WORD TOKEN화할 때 WORD의 크기가 필요
    int word_size = 0;

    // shell을 처음 실행하거나, 전 명령어에 대한 token malloc 과정에 error가 나서
    // error_exit으로 가서 정리 과정을 거치게 되면 *token은 NULL이므로 
    // 동적 배열 할당을 해준다
    if(*token == NULL)
    {
        *token = (Token *)malloc(sizeof(Token) * *token_capacity);
        for(int i = 0; i < *token_capacity; i++)
        {
            (*token)[i].type = null;
            (*token)[i].value = NULL;
        }
    }
    // 이미 *token에 배열이 할당이 되어있으면, Token.value을 free 정리해서
    // 다음 명령어의 토큰들을 넣을 준비를 한다.
    else if(*token != NULL)
    {
        for(int i = 0; i < *token_capacity; i++)
        {
            free((*token)[i].value);
            (*token)[i].value = NULL;
        }
    }

    char *curr = input;
    char *start = curr;

    while(*curr != '\0')
    {
        if(in_quote == false)
        {
            if(*curr == '|')
            {
                // TOKEN화
                (*token)[count].type = PIPE;
                (*token)[count].value = malloc(sizeof(char) * 2);
                (*token)[count].value[0] = '|';
                (*token)[count].value[1] = '\0';

                count++;
                curr++;
                (*pipe_cnt)++;
                if(isspace(*curr) == 0)
                    start = curr;
            }
            else if(*curr == '<' || *curr == '>')
            {
                // < 나 > 에 따라 TOKEN화
                (*token)[count].value = malloc(sizeof(char) * 2);
                if(*curr == '<')
                {
                    (*token)[count].type = REDIR_IN;
                    (*token)[count].value[0] = '<';
                }
                else if(*curr == '>')
                {
                    (*token)[count].type = REDIR_OUT;
                    (*token)[count].value[0] = '>';
                }
                (*token)[count].value[1] = '\0';

                count++;
                curr++;
                if(isspace(*curr) == 0)
                    start = curr;
            }
            else if(*curr == '&')
            {
                // TOKEN화
                (*token)[count].type = BG;
                (*token)[count].value = malloc(sizeof(char) * 2);
                (*token)[count].value[0] = '&';
                (*token)[count].value[1] = '\0';

                count++;
                curr++;
                if(isspace(*curr) == 0)
                    start = curr;
            }
            else if(*curr == '"' || *curr == '\'')
            {
                in_quote = true;
                curr++;
            }
            else if(isspace(*curr))
            {
                curr++;
                if (isspace(*curr) == 0)
                    start = curr;
            }
            else    // 일반 문자인 경우
            {
                curr++;
                if(isspace(*curr) || *curr == '|' || *curr == '<' ||
                *curr == '>' || *curr == '&' || *curr == '\0')
                {
                    // WORD TOKEN화
                    word_size = curr - start;
                    (*token)[count].value = malloc(sizeof(char) * (word_size + 1));
                    (*token)[count].type = WORD;
                    memcpy((*token)[count].value, start, word_size);
                    (*token)[count].value[word_size] = '\0';

                    count++;
                }
            }
        }
        else if(in_quote == true)
        {
            if(*curr == '"' || *curr == '\'')
            {
                // TOKEN화
                word_size = curr - start - 1;
                (*token)[count].value = malloc(sizeof(char) * (word_size + 1));
                (*token)[count].type = WORD;
                memcpy((*token)[count].value, start + 1, word_size);
                (*token)[count].value[word_size] = '\0';

                count++;
                in_quote = false;
                curr++;
            }
            else
            {
                curr++;
            }
        }

        // count == capacity이면 realloc을 해준다.
        if(count == *token_capacity)
        {
            *token_capacity += 10;
            Token *temp = (Token *)realloc(*token, sizeof(Token) * *token_capacity);

            // realloc 실패하면 모두 free
            if(temp == NULL)
            {
                perror("tokenize() realloc 실패, 메모리 free");
                for(int i = 0; i < *token_capacity - 10; i++)
                    free((*token)[i].value);
                free((*token));
                (*token) = NULL;
                *token_capacity = 10;
                return;
            }
            
            // 새로 생긴 공간 초기화
            for(int i = *token_capacity - 10; i < *token_capacity; i++)
            {
                temp[i].type = null;
                temp[i].value = NULL;
            }

            *token = temp;
        }
    }

    // END TOKEN화 하기
    // END TOKEN은 value 값을 지정하지 않아도 됨
    (*token)[count].type = END;
    count++;
    return;

// 동적 할당된 token.value 자원들 free. token 배열 자체는 재사용할거라 free X.
// token 배열은 main의 while을 벗어나면 free함.
error_exit:
    if((*token) != NULL)
    {
        for(int i = 0; i < count; i++)
        {
            if((*token)[i].value != NULL)
                free((*token)[i].value);
        }
    }
    return;
}

// token들을 분석해서 Command 배열로 변환
// return: 성공하면 0, 실패하면-1
// pipeline은 처음 실행할 때 무조건 NULL이어야 함
int parser(Pipeline **pipeline, Token **tokens, int pipe_cnt)
{
    // 처음 shell을 실행할 때 pipeline 배열 초기화
    if(*pipeline == NULL)
    {
        *pipeline = (Pipeline *)malloc(sizeof(Pipeline));
        if(*pipeline == NULL)
        {
            fprintf(stderr, "Initial pipeline(NULL) memory allocation fail: %s", strerror(errno));
            goto error_exit;
        }
        (*(*pipeline)).cmd_count = pipe_cnt + 1;
        (*(*pipeline)).bg = false;
        (*(*pipeline)).commands = (Command *)malloc(sizeof(Command) * (*(*pipeline)).cmd_count);
        if((*(*pipeline)).commands == NULL)
        {
            fprintf(stderr, "Initial pipeline->commands(NULL) memory allocation fail: %s", strerror(errno));
            goto error_exit;
        }

        // Command 멤버 변수 초기화
        for(int i = 0; i < (*(*pipeline)).cmd_count; i++)
        {
            (*(*pipeline)).commands[i].argv_capacity = 10;
            (*(*pipeline)).commands[i].argv = (char **)malloc(sizeof(char *) * (*(*pipeline)).commands[i].argv_capacity);
            if((*(*pipeline)).commands[i].argv == NULL)
            {
                fprintf(stderr, "pipeline->commands[%d]->argv memory allocation fail: %s", i, strerror(errno));
                goto error_exit;
            }
            for(int j = 0; j < (*(*pipeline)).commands[i].argv_capacity; j++)
                (*(*pipeline)).commands[i].argv[j] = NULL;
            (*(*pipeline)).commands[i].argc = 0;
            (*(*pipeline)).commands[i].input_fd = -1;
            (*(*pipeline)).commands[i].output_fd = -1;
            (*(*pipeline)).commands[i].redir = -1;
            (*(*pipeline)).commands[i].redir_file = NULL;
        }
    }
    // 2번째 명령어 실행 시
    // Command[]과 argv[]은 재사용 및 크기 재설정
    // argv[i]에 할당된 베열은 매번 free
    else if(*pipeline != NULL)
    {
        (*(*pipeline)).bg = false;
        int last_cmd_count = (*(*pipeline)).cmd_count;
        (*(*pipeline)).cmd_count = pipe_cnt + 1;
        // 현재 명령어의 수가 현재 명령어의 수보다 많을 때
        // Command[] 크기 재설정 및 초기화 필요
        if((*(*pipeline)).cmd_count > last_cmd_count)
        {
            Command *temp = (Command *)realloc((*(*pipeline)).commands, sizeof(Command) * (*(*pipeline)).cmd_count);
            // 실패 시
            if(temp == NULL)
            {
                fprintf(stderr, "Command[] 재할당 realloc() 실패: %s", strerror(errno));
                goto error_exit;
            }
            (*(*pipeline)).commands = temp;

            // 초기화
            for(int i = 0; i < (*(*pipeline)).cmd_count; i++)
            {
                // 전에 할당된 Command[] 요소들 초기화
                if(i < last_cmd_count)
                {
                    for(int j = 0; j <= (*(*pipeline)).commands[i].argc; j++)
                    {
                        free((*(*pipeline)).commands[i].argv[j]);
                        (*(*pipeline)).commands[i].argv[j] = NULL;       
                    }
                    if((*(*pipeline)).commands[i].redir_file != NULL)
                    {
                        free((*(*pipeline)).commands[i].redir_file);
                        (*(*pipeline)).commands[i].redir_file = NULL;
                    }
                }
                // realloc으로 새로 생긴 Command[] 요소 초기화
                else if(i >= last_cmd_count)
                {
                    (*(*pipeline)).commands[i].argv_capacity = 10;
                    (*(*pipeline)).commands[i].argv = (char **)malloc(sizeof(char *) * (*(*pipeline)).commands[i].argv_capacity);
                    if((*(*pipeline)).commands[i].argv == NULL)
                    {
                        fprintf(stderr, "Memory allocation for reallocted pipeline->commands->argv[%d] fail: %s", i, strerror(errno));
                        goto error_exit;
                    }
                    for(int j = 0; j < (*(*pipeline)).commands[i].argv_capacity; j++)
                        (*(*pipeline)).commands[i].argv[j] = NULL;
                    (*(*pipeline)).commands[i].redir_file = NULL;
                }
                (*(*pipeline)).commands[i].argc = 0;
                (*(*pipeline)).commands[i].input_fd = -1;
                (*(*pipeline)).commands[i].output_fd = -1;
                (*(*pipeline)).commands[i].redir = -1;
            }
        }
        // 새로운 명령어가 이전 명령어보다 수가 적을 때 초기화 과정
        // Command[] 배열 크기 재설정 하지 않고 멤버변수만 초기화
        else if((*(*pipeline)).cmd_count <= last_cmd_count)
        {
            for(int i = 0; i < last_cmd_count; i++)
            {
                for(int j = 0; j <= (*(*pipeline)).commands[i].argc; j++)
                {
                    free((*(*pipeline)).commands[i].argv[j]);
                    (*(*pipeline)).commands[i].argv[j] = NULL;
                }
                if((*(*pipeline)).commands[i].redir_file != NULL)
                    free((*(*pipeline)).commands[i].redir_file);
                (*(*pipeline)).commands[i].redir_file = NULL;
                (*(*pipeline)).commands[i].argc = 0;
                (*(*pipeline)).commands[i].input_fd = -1;
                (*(*pipeline)).commands[i].output_fd = -1;
                (*(*pipeline)).commands[i].redir = -1;
            }
        }
    }

    int token_index = 0;        // token 하나를 처리하면 +1
    int command_index = 0;      // pipe을 만나면 +1
    size_t token_size = 0;

    while((*tokens)[token_index].type != END)
    {
        // WORD TOKEN인 경우
        if((*tokens)[token_index].type == WORD)
        {
            // argv에 token size만큼 동적 배열
            token_size = strlen((*tokens)[token_index].value);
            (*(*pipeline)).commands[command_index].argv[(*(*pipeline)).commands[command_index].argc] = 
                                (char *)malloc(sizeof(char) * (token_size + 1)); // +1은 \0을 위한 것
            if((*(*pipeline)).commands[command_index].argv[(*(*pipeline)).commands[command_index].argc] == NULL)
            {
                fprintf(stderr, "pipeline->commands->argv[%d] memory allocation for WORD token fail: %s",
                        (*(*pipeline)).commands[command_index].argc,
                        strerror(errno));
                goto error_exit;
            }
            
            // token string을 copy
            for(int i = 0; i < token_size; i++)
            {
                (*(*pipeline)).commands[command_index].argv[(*(*pipeline)).commands[command_index].argc][i] =
                                (*tokens)[token_index].value[i];
            }
            (*(*pipeline)).commands[command_index].argv[(*(*pipeline)).commands[command_index].argc][token_size] = '\0';
            
            // argc + 1
            (*(*pipeline)).commands[command_index].argc++;

            // argc가 capacity와 같으면 10만큼 더 realloc
            if((*(*pipeline)).commands[command_index].argc == (*(*pipeline)).commands[command_index].argv_capacity)
            {
                (*(*pipeline)).commands[command_index].argv_capacity += 10;
                char **temp = realloc((*(*pipeline)).commands[command_index].argv,
                                sizeof(char *) * (*(*pipeline)).commands[command_index].argv_capacity);
                if(temp == NULL)
                {
                    fprintf(stderr, "Rellocation failed at Command[%d].argv: %s", command_index, strerror(errno));
                    goto error_exit;
                }
                (*(*pipeline)).commands[command_index].argv = temp;

                // 재할당된 공가 초기화
                for(int i = (*(*pipeline)).commands[command_index].argc;
                                i < (*(*pipeline)).commands[command_index].argv_capacity;
                                i++)
                    (*(*pipeline)).commands[command_index].argv[i] = NULL;
            }
        }

        // PIPE TOKEN인 경우
        else if((*tokens)[token_index].type == PIPE)
        {
            (*(*pipeline)).commands[command_index].argv[(*(*pipeline)).commands[command_index].argc] = (char *)malloc(sizeof(char));
            if((*(*pipeline)).commands[command_index].argv[(*(*pipeline)).commands[command_index].argc] == NULL)
            {
                fprintf(stderr, "Memory allocation at pipeline->commands[%d]->argv[%d] for | token failed: %s",
                        command_index, (*(*pipeline)).commands[command_index].argc, strerror(errno));
                goto error_exit;
            }
            (*(*pipeline)).commands[command_index].argv[(*(*pipeline)).commands[command_index].argc][0] = '\0';
            (*(*pipeline)).commands[command_index].output_fd = 0;
            command_index++;
            (*(*pipeline)).commands[command_index].input_fd = 0;
        }

        // REDIR_IN/OUT TOKEN인 경우
        else if(((*tokens)[token_index].type == REDIR_IN) || 
                    ((*tokens)[token_index].type == REDIR_OUT))
        {
            if((*tokens)[token_index].type == REDIR_IN)
                (*(*pipeline)).commands[command_index].redir = 1;

            else if((*tokens)[token_index].type == REDIR_OUT)
                (*(*pipeline)).commands[command_index].redir = 0;

            // redirection 뒤의 file이름 token으로 옮김
            token_index++;  
            
            // filename을 redir_file로 copy
            token_size = strlen((*tokens)[token_index].value);
            (*(*pipeline)).commands[command_index].redir_file = (char *)malloc(sizeof(char) * (token_size + 1));
            if((*(*pipeline)).commands[command_index].redir_file == NULL)
            {
                fprintf(stderr, "Memory allocation at pipeline->commands[%d]->redir_file failed: %s",
                        command_index, strerror(errno));
                goto error_exit;
            }

            for(int i = 0; i < token_size; i++)
                (*(*pipeline)).commands[command_index].redir_file[i] = (*tokens)[token_index].value[i];
            (*(*pipeline)).commands[command_index].redir_file[token_size] = '\0';
        }

        // BG TOKEN인 경우
        else if((*tokens)[token_index].type == BG)
            (*(*pipeline)).bg = true;

        token_index++;
    }

    // END TOKEN이므로 argv[argc] = NULL해주기
    (*(*pipeline)).commands[command_index].argv[(*(*pipeline)).commands[command_index].argc] = (char *)malloc(sizeof(char));
    if((*(*pipeline)).commands[command_index].argv[(*(*pipeline)).commands[command_index].argc] == NULL)
    {
        fprintf(stderr, "Memory allocation for END token failed: %s", strerror(errno));
        goto error_exit;
    }
    (*(*pipeline)).commands[command_index].argv[(*(*pipeline)).commands[command_index].argc][0] = '\0';

    return 0;

error_exit:
    if(*pipeline != NULL)
    {
        if((*(*pipeline)).commands != NULL)
        {
            for(int i = 0; i < (*(*pipeline)).cmd_count; i++)
            {
                if((*(*pipeline)).commands[i].argv != NULL)
                {
                    for(int j = 0; j < (*(*pipeline)).commands[i].argv_capacity; j++)
                    {
                        // *pipeline -> Pipeline -> Command -> argv -> argv[j] free
                        if((*(*pipeline)).commands[i].argv[j] != NULL)
                            free((*(*pipeline)).commands[i].argv[j]);
                    }
                    // *pipeline -> Pipeline -> Command -> argv free
                    free((*(*pipeline)).commands[i].argv);
                }
                // redir_file free
                if((*(*pipeline)).commands[i].redir_file != NULL)
                    free((*(*pipeline)).commands[i].redir_file);
            }
            // *pipeline -> Pipeline -> Command free
            free((*(*pipeline)).commands);
        }
        // *pipeline -> Pipeline free
        free(*pipeline);
        // *pipeline = NULL
        *pipeline = NULL;
    }
    return -1;
}