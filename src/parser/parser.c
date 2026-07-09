#include "../../include/parser.h"

void tokenize(char *input, Token **token)
{
    // 최대 토큰 수를 초과하면 realloc를 한다
    int count = 0, capacity = 10; 
    //" "안에 있는 경우 
    bool in_quote = false;

    if(*token == NULL)
        *token = (Token *)malloc(sizeof(Token) * capacity);

    char *curr = input;
    char *start = curr;

    while(*curr != '\0')
    {
        if(in_quote = false)
        {
            if(*curr == '|')
            {
                // TOKEN화
                count++;
                curr++;
                if(isspace(*curr) == 0)
                    start = curr;
            }
            else if(*curr == '<' || *curr == '>')
            {
                // < 나 > 에 따라 TOKEN화
                count++;
                curr++;
                if(isspace(*curr) == 0)
                    start = curr;
            }
            else if(*curr == '&')
            {
                // TOKEN화
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
                    count++;
                }
            }
        }
        else if(in_quote = true)
        {
            if(*curr == '"' || *curr == '\'')
            {
                // TOKEN화
                count++;
                in_quote = false;
                curr++;
            }
            else
            {
                curr++;
            }
        }
    }
}
