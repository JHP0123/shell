#include "../../include/executor.h"

int create_pipe(Pipeline *pipeline, int ***pipes)
{
    // pipe의 개수
    int pipe_cnt = (*pipeline).cmd_count - 1;

    // pipe 호출을 위해 2차원 배열 [pipe_cnt][2] 생성
    (*pipes) = (int **)malloc(sizeof(int *) * pipe_cnt);
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

    return 1;

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
    return -1;
}