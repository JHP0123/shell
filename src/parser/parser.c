#include "../../include/parser.h"

void tokenize(char *input, Token **token, int *pipe_cnt)
{
    *pipe_cnt = 0;
    // 최대 토큰 수를 초과하면 realloc를 한다
    int count = 0, capacity = 10; 
    //" "안에 있는 경우 
    bool in_quote = false;
    // WORD TOKEN화할 때 WORD의 크기가 필요
    int word_size = 0;

    if(*token == NULL)
        *token = (Token *)malloc(sizeof(Token) * capacity);

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
        if(count == capacity)
        {
            capacity += 10;
            Token *temp = (Token *)realloc(*token, sizeof(Token) * capacity);
            if(temp == NULL)
            {
                perror("realloc 실패, 메모리 free 과정 필요");
                // 모두 free하는 과정이 필요하다
                // free(NULL)은 safe하다
                return;
            }

            *token = temp;
        }
    }

    if(count >= capacity)
    {
        capacity += 10;
        Token *temp = (Token *)realloc(*token, capacity);
        if(temp == NULL)
        {
            perror("realloc 실패, 메모리 free 과정 필요");
            // 모두 free하는 과정이 필요하다
            // free(NULL)은 safe하다
            return;
        }
        *token = temp;
    }

    // END TOKEN화 하기
    (*token)[count].value = malloc(sizeof(char));
    (*token)[count].type = END;
    (*token)[count].value[0] = '\0';
    count++;
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
            command_index++;
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