#ifndef EXECUTOR_H
#define EXECUTOR_H
#include "data_structure.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// pipe을 생성
int create_pipe(Pipeline *pipeline, int ***pipes, int *pipe_capacity);

// // child process을 생성하고, pgrp를 set해주고, Joblist에 정보 기록
// int fork_process(Pipeline *pipeline, Joblist *joblist);

// // pipe을 설정하고 redirection을 한다
// int pipe_and_redirect(Pipeline *pipeline, Joblist *joblist, int **pipes);

// // child pgrp에게 tcsetpgrp를 하고 exec하고 wait한다.
// int exec_and_wait(Joblist *joblist, int *status);

#endif