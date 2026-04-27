# Shell Implementation Roadmap.

Step|Related Commands|Key System Calls and Learning Keywords
---|---|---
1\. Basic Commmands|ls, mkdir, rm, mv, cp, cat, grep, wc, sort, head, nano|fork(), execve(), wait()
2\. File I/O|>, <, pwd|open(), close(), dup2(), getcwd()
3\. InterProcess Communicatoin|\||pipe(), I/O Redirection, IPC
4\. Job Control|jobs, fg, bg, kill| signal, waitpid(), tcsetpgrp(), PGID
5\. Shell Built-In Commands| cd, export, set, alias, *|chdir(), getenv(), setenv(), globbing


## 1. Basic Commands
- Basic Commands is run by running external executable program which exists in the file system. 
- The fundamental flow using specific system calls: **Fork-Exec-Wait** Cycle.
    - fork(): Copies the parent process and creates a child process.
    - execve(): The child process overwrites its memory space into an actual command program(ex: ls, mkdir).
    - wait(): The parent process waits till the child process ends its execution. When the child process ends its execution, the parent process receives the exit status code and cleans up the resources that the child proces had occupied. 
    - exit(): The child process exits and signals the parent process. 