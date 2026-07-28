#include "../../include/executor.h"

int create_pipe(int ***pipes, int *pipe_cnt, int *pipe_capacity)
{
    // 초기 pipes 설정. pipes 는 NULL이므로 초기 메모리 할당
    if(*pipes == NULL)
    {
        // pipe_capacity의 용량을 10을 더해도 pipe_cnt가 클 수 있으므로 pipe_capacity가 pipe_cnt보다 커질 때까지 반복
        while(*pipe_cnt > *pipe_capacity)
            (*pipe_capacity) += 10;

        // 초기 pipes 메모리 할당
        *pipes = (int **)malloc(sizeof(int *) * *pipe_capacity);
        if(*pipes == NULL)
        {
            fprintf(stderr, "Initial pipes malloc at create_pipe() failed: %s", strerror(errno));
            goto error_exit;
        }
        // 초기화
        for(int i = 0; i < *pipe_capacity; i++)
            (*pipes)[i] = NULL;
    }
    // 이미 pipes 초기 설정이 된 상태
    // pipes가 가리키는 배열 재사용
    // pipes[i]가 가리키는 int[2] 배열은 free (free하기 전 close 필요)
    // pipes[i]가 가리키는 int[2] 배열은 pipe_and_redirect() 함수에서 free
    // pipes[i][j] file descriptor를 close하는 과정은 pipe_redirect() 내부에서 dup2를 할 때 진행 
    // 아래의 pipes 초기화 과정에는 close하는 과정이 필요 없음. 이미 close와 free가 되었다고 설정
    // **중요한 점**: create_pipe() 함수는 pipe을 생성하기 전에 pipes가 가리키는 배열의 요소들은 모두
    //               NULL을 가리켜야 됨. 이 부분은 pipe_and_redirect() 함수에서 진행
    else if(*pipes != NULL)
    {
        if(*pipe_cnt > *pipe_capacity)
        {
            int last_pipe_capacity = *pipe_capacity;
            while(*pipe_cnt > *pipe_capacity)
                (*pipe_capacity) += 10;

            int **temp = realloc(*pipes, sizeof(int *) * *pipe_capacity);
            if(temp = NULL)
            {
                fprintf(stderr, "Realloc to pipes at create_pipe() failed: %s", strerror(errno));
                goto error_exit;
            }
            (*pipes) = temp;
            // 새로 할당된 메모리 공간 초기화
            for(int i = last_pipe_capacity; i < *pipe_capacity; i++)
                (*pipes)[i] = NULL;
        }        
    }

    // pipe 배열 생성
    for(int i = 0; i < *pipe_cnt; i++)
    {
        (*pipes)[i] = malloc(sizeof(int) * 2);
        if((*pipes)[i] == NULL)
        {
            fprintf(stderr, "malloc to pipes[%d] at create_pipe failed: %s", i, strerror(errno));
            goto error_exit;
        }
        // 초기화
        (*pipes)[i][0] = -1;
        (*pipes)[i][1] = -1;
    }

    // pipe 생성
    for(int i = 0; i < *pipe_cnt; i++)
    {
        if(pipe((*pipes)[i]) == -1)
        {
            fprintf(stderr, "creating pipe at pipes[%d] failed: %s", i, strerror(errno));
            goto error_exit;
        }
    }

    return 0;

error_exit:
    if(*pipes != NULL)
    {        
        for(int i = 0; i < *pipe_cnt; i++)
        {
            if((*pipes)[i] != NULL)
            {
                if((*pipes)[i][0] != -1 && (*pipes)[i][1] != -1)
                {
                    close((*pipes)[i][0]);
                    close((*pipes)[i][1]);
                }
                free((*pipes)[i]);
                (*pipes)[i] = NULL;
            }
        }
        free(*pipes);
        *pipes = NULL;
    }
    *pipe_capacity = 10;
    return -1;
}

// int fork_process(Pipeline *pipeline, Joblist *joblist)
// {
//     int child_n = (*pipeline).cmd_count;
//     (*joblist).job_count++;
// }