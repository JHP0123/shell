#include "../../include/parser.h"

// Token **token 동적 배열은 재사용함. main의 while을 나가면 그때가서 free
// Token.value는 매 반복마다 free해주어야 함. 
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

Pipeline *parser(Token **tokens, int pipe_cnt)
{
    Pipeline *temp_pipeline = (Pipeline *)malloc(sizeof(Pipeline));
    (*temp_pipeline).cmd_count = pipe_cnt + 1;
    (*temp_pipeline).commands = (Command *)malloc(sizeof(Command) * (*temp_pipeline).cmd_count);
    (*temp_pipeline).bg = false;

    size_t capacity = 10;       // pipe를 만나면 10으로 초기화
    int token_index = 0;        // token 하나를 처리하면 +1
    int command_index = 0;      // pipe을 만나면 +1
    size_t token_size = 0;

    // commands 배열 초기화 작업 필요
    for(int i = 0; i < (*temp_pipeline).cmd_count; i++)
    {
        ((*temp_pipeline).commands)[i].argc = 0;
        ((*temp_pipeline).commands)[i].argv = (char **)malloc(sizeof(char *) * capacity);
        ((*temp_pipeline).commands)[i].input_fd = -1;
        ((*temp_pipeline).commands)[i].output_fd = -1;
        ((*temp_pipeline).commands)[i].redir = -1;
        ((*temp_pipeline).commands)[i].redir_file = NULL;
    }

    while((*tokens)[token_index].type != END)
    {
        // WORD TOKEN인 경우
        if((*tokens)[token_index].type == WORD)
        {
            // argv에 token size만큼 동적 배열
            token_size = strlen((*tokens)[token_index].value);
            ((*temp_pipeline).commands)[command_index].argv[((*temp_pipeline).commands)[command_index].argc] = 
                                (char *)malloc(sizeof(char) * (token_size + 1));
            
            // token string을 copy
            for(int i = 0; i < token_size; i++)
            {
                ((*temp_pipeline).commands)[command_index].argv[((*temp_pipeline).commands)[command_index].argc][i] =
                                (*tokens)[token_index].value[i];
            }
            ((*temp_pipeline).commands)[command_index].argv[((*temp_pipeline).commands)[command_index].argc][token_size] = '\0';
            
            // argc + 1
            ((*temp_pipeline).commands)[command_index].argc++;

            // argc가 capacity보다 같거나 크면 realloc
            if(((*temp_pipeline).commands)[command_index].argc >= capacity)
            {
                capacity += 10;
                char **temp = realloc(((*temp_pipeline).commands)[command_index].argv, capacity);
                if (temp == NULL)
                {
                    perror("realloc failed at parser()");
                    // 모두 free하는 과정이 필요
                    // free(NULL)은 safe하다
                    return NULL;
                }
                ((*temp_pipeline).commands)[command_index].argv = temp;
            }
        }

        // PIPE TOKEN인 경우
        else if((*tokens)[token_index].type == PIPE)
        {
            ((*temp_pipeline).commands)[command_index].argv[((*temp_pipeline).commands)[command_index].argc]
                    = (char *)malloc(sizeof(char));
            ((*temp_pipeline).commands)[command_index].argv[((*temp_pipeline).commands)[command_index].argc][0]
                    = '\0';
            capacity = 10;
            ((*temp_pipeline).commands)[command_index].output_fd = 0;
            command_index++;
            ((*temp_pipeline).commands)[command_index].input_fd = 0;
        }

        // REDIR_IN/OUT TOKEN인 경우
        else if(((*tokens)[token_index].type == REDIR_IN) || 
                    ((*tokens)[token_index].type == REDIR_OUT))
        {
            if((*tokens)[token_index].type == REDIR_IN)
                ((*temp_pipeline).commands)[command_index].redir = 1;

            else if((*tokens)[token_index].type == REDIR_OUT)
                ((*temp_pipeline).commands)[command_index].redir = 0;

            // redirection 뒤의 file이름 token으로 옮김
            token_index++;  
            
            // filename을 redir_file로 copy
            token_size = strlen((*tokens)[token_index].value);
            ((*temp_pipeline).commands)[command_index].redir_file =
                        (char *)malloc(sizeof(char) * (token_size + 1));

            for(int i = 0; i < token_size; i++)
            {
                ((*temp_pipeline).commands)[command_index].redir_file[i] = 
                        (*tokens)[token_index].value[i];
            }
            ((*temp_pipeline).commands)[command_index].redir_file[token_size] = '\0';
        }

        // BG TOKEN인 경우
        else if((*tokens)[token_index].type == BG)
        {
            (*temp_pipeline).bg = true;
        }

        token_index++;
    }

    // END TOKEN이므로 argv[argc] = NULL해주기
    ((*temp_pipeline).commands)[command_index].argv[((*temp_pipeline).commands)[command_index].argc]
            = (char *)malloc(sizeof(char));
    ((*temp_pipeline).commands)[command_index].argv[((*temp_pipeline).commands)[command_index].argc][0]
            = '\0';

    return temp_pipeline;
}