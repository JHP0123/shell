#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum
{
    null,       // 초기화 값
    WORD,
    PIPE,       // |
    REDIR_IN,   // <
    REDIR_OUT,  // >
    BG,         // &
    END         // NULL
} TokenType;
// 이렇게 해야 각 단어 별 특성을 파악하고 공백에 대해서도 쉽게 처리할 수 있음

typedef enum
{
    WORKING,
    TERMINATED,
    STOPPED
} Jobstatus;

typedef struct 
{
    char *value;
    TokenType type;
} Token;


typedef struct
{
    char **argv;
    int argv_capacity;  
    int argc;
    int input_fd;       // 기본: stdin
    int output_fd;      // 기본: stdout
    int redir;          // -1: default, 0: >, 1: <
    char *redir_file;
} Command;

typedef struct
{
    Command *commands;  // 명령어들의 배열
    int cmd_count;      // 명령어의 개수
    bool bg;            // background job(&)이면 true, foreground job이면 false
} Pipeline;

typedef struct 
{
    pid_t pgid;
    pid_t *plist;
    unsigned job_no;
    Jobstatus status;
    unsigned process_count;
} Job;

typedef struct
{
    Job *job_list;
    int job_count;
} Joblist;


#endif