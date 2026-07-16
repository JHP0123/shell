#include "../../include/executor.h"

int create_pipe(Pipeline *pipeline, int ***pipes, int *pipe_capacity)
{
    // pipe의 개수
    int pipe_cnt = (*pipeline).cmd_count - 1;

    // pipe 호출을 위해 2차원 배열 [pipe_cnt][2] 생성
    if((*pipes) == NULL)
        (*pipes) = (int **)malloc(sizeof(int *) * *pipe_capacity);

    // pipe의 개수가 *pipes에 동적할당된 요소의 개수 pipe_capacity보다 크면 10만큼 더 realloc한다
    if(pipe_cnt > *pipe_capacity)
    {
        *pipe_capacity += 10;
        int **temp = realloc((*pipes), sizeof(int *) * *pipe_capacity);
        if(temp == NULL)
        {
            fprintf(stderr, "[ERROR] pipes realloc failed: %s\n", strerror(errno));
            goto error_exit;
        }
        (*pipes) = temp;
    }
    
    if((*pipes) == NULL)
    {
        perror("malloc to pipes failed at create_pipe(): ");
        goto error_exit;
    }

        // 초기화
    for(int i = 0; i < pipe_cnt; i++)
        (*pipes)[i] = NULL;

    for(int i = 0; i < pipe_cnt; i++)
    {
        (*pipes)[i] = (int *)malloc(sizeof(int) * 2);
        if((*pipes)[i] == NULL)
        {
            perror("malloc to pipes[i] failed at create_pipe(): ");
            goto error_exit;
        }
        // 초기화
        (*pipes)[i][0] = -1;
        (*pipes)[i][1] = -1;
    }

    // pipe 생성
    for(int i = 0; i < pipe_cnt; i++)
    {
        if(pipe((*pipes)[i]) == -1)
        {
            fprintf(stderr, "[ERROR] pipes[%d] pipe() error: %s\n", i, strerror(errno));
            goto error_exit;
        }
    }

    return pipe_cnt;

error_exit:
    for(int i = 0; i < pipe_cnt; i++)
    {
        if((*pipes)[i][0] != -1 && (*pipes)[i][1] != -1)
        {
            close((*pipes)[i][0]);
            close((*pipes)[i][1]);
        }
        free((*pipes)[i]);
    }
    free((*pipes));
    *pipe_capacity = 10;
    return -1;
}

// int fork_process(Pipeline *pipeline, Joblist *joblist)
// {
//     int child_n = (*pipeline).cmd_count;
//     (*joblist).job_count++;
// }