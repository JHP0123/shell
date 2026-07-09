#include "../../include/parser.h"

void tokenize(char *input, Token **token)
{
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
            Token *temp = (Token *)realloc(*token, capacity);
            if(temp == NULL)
            {
                perror("realloc 실패, 메모리 free 과정 필요");
                return;
            }

            *token = temp;
        }
    }

    // END TOKEN화 하기
    (*token)[count].value = malloc(sizeof(char));
    (*token)[count].type = END;
    (*token)[count].value[0] = '\0';
    count++;
}
