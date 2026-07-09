#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

typedef enum
{
    WORD,
    PIPE,       // |
    REDIR_IN,   // <
    REDIR_OUT,  // >
    BG,         // &
    END         // NULL
} TokenType;
// 이렇게 해야 각 단어 별 특성을 파악하고 공백에 대해서도 쉽게 처리할 수 있음

typedef struct 
{
    char *value;
    TokenType type;
} Token;


typedef struct
{
    char **argv;
    int argc;
    int input_fd;       // 기본: stdin
    int output_fd;      // 기본: stdout
    char *redir_file;
} Command;


#endif