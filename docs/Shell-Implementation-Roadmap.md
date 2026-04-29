# Shell Implementation Roadmap.

Step|Related Commands|Key System Calls and Learning Keywords
---|---|---
1\. Basic Commmands|ls, mkdir, rm, mv, cp, cat, grep, wc, sort, head, nano|fork(), execve(), wait()
2\. File I/O|>, <, pwd|open(), close(), dup2(), getcwd()
3\. InterProcess Communicatoin|\||pipe(), I/O Redirection, IPC
4\. Job Control|jobs, fg, bg, kill| signal, waitpid(), tcsetpgrp(), PGID
5\. Shell Built-In Commands| cd, export, set, alias, *|chdir(), getenv(), setenv(), globbing


## 1. Basic Commands

### System Calls
- Basic Commands is run by running external executable program which exists in the file system. 
- The fundamental flow using specific system calls: **Fork-Exec-Wait** Cycle.
    - **fork()**: Copies the parent process and creates a child process.
        - If the shell program does not use fork call and execute basic commands using execve, the new program will overwrite the shell in memory and the shell process will end. 
    - **execve()**: The child process overwrites its memory space into an actual command program(ex: ls, mkdir).
        - `int execve(const char *pathname, char *const _Nullable argv[], char *const _Nullable envp[])`
        - When the shell process wants to execute "ls", the shell looks up the PATH environment variable. But the first argument of execve is `const char * pathname` so we have to parse the value of PATH variable. The PATH variable uses `:` to identify directories, so we can use **strtok** to parse the value using **:** . 
            - Before we parse the value of PATH, we have to **get** the variable PATH. `char *getenv(const char *name)` searches the environment list to find the environment variable 'name' and returns a pointer to the corresponding 'value' string (Note: environ is KEY=VALUE table). 
        - The third argument `char *const _Nullable envp[]` is used to hand over the environment variable vector to the child process. The **environ** is conventionally used for the third input argument. 'environ' is essential for the child process to normally execute their behavior. It is a collection of dynamic configuration values ​​that affect the behavior of processes (running programs) at the operating system (OS) level. Global information necessary for application execution, such as system paths, user information, and debug mode (KEY=VALUE). 
        - `int access(const char *path, int mode)` system call checks whether the calling process can access the file path. To use the 'execve' call, we strtok() the PATH var. There are numbers of file path and we have to check whether the path is accessable. Argument `int mode` specifies the accessibility checks to be performed and is either the value of `F_OK`(tests for the existence of the file), `R_OK`(tests for read), `W_OK`(write), `X_OK`(execute).
    - **wait()**: The parent process waits till the child process ends its execution. When the child process ends its execution, the parent process receives the exit status code and cleans up the resources that the child process had occupied. 
        - **wait()** system call is used to wait for state change in child of the calling process, and obtain information about the child whose state has changed: __Terminated, Stopped, Resumed__. In case of Terminated, performing a wait allows the system to release the resources associated with the child. If a wait call is not performed, the terminated child remains **zombie process**. 
            - `wait(NULL)` is only returned when the child process is terminated.
        - When wait call is not performed, the kernel maintains a set of information(PID, termination status, resource usage informatation) to allow the parent to later perform a wait to obtain information about the child. The zombie process consumes the kernel process table and if this table is filled, it will not be possible to create further processes. 
        - If a parent process is terminated, the zombie process is adopted by **init** process. The init process automatically performs a wait to remove the zombies.
        - `pid_t wait(int *wstatus)` has 1 input arg. If the parent process passes the address of an integer variable, the kernel directly writes the child process's termination information to that address. If 'NULL' is input, the parent waits till the child process is done and finished the execution. 
            - 'wstatus' is a bitmask form and contains informations such as
                1) Exit Status: If the child process calls 'exit(5)', 5 is included to 'wstatus'
                2) Termination Signal: It includes information on which signal (e.g., SIGKILL) caused the forced termination.
                3) Core Dump: It includes whether a core dump file was generated upon termination. Core dump is an information about whether the child process left behind a "black box (core dump file) for debugging" when it died.
            - Because the bits stored in wstatus are difficult for humans to read visually, dedicated macro functions are used in system programming to interpret them.
                1) `WIFEXITED(status)`: It returns True if the child terminates normally with exit() or return
                2) `WEXITSTATUS(status)`: When the above condition is true, extract the actual exit code (0~255) passed by the child.
                3) `WIFSIGNALED(status)`: Returns true if the child died abnormally due to a signal.
                4) `WTERMSIG(status)`: It tells you what the signal number is that caused the child's death.
                5) `WCOREDUMP(wstatus)`: Returns true if the child produced a core dump. This function should be employed when `WIFSIGNALED` returned true.

                
                Signal is a 'asynchronous alarm' or 'software interrupt' which signals the process by the OS. When signal has arrived, the process stops its action immediately and perform the action (signal handler) promised for the corresponding signal. 

                시그널 이름|번호|기본 동작 (Default Action)|상태 변화 결과|주요 발생 상황
                ---|---|---|---|---
                SIGINT|2|Terminate|Terminated|Ctrl + C 입력 시 (인터럽트)
                SIGQUIT|3|Terminate + Core Dump|Terminated|Ctrl + \ 입력 시
                SIGKILL|9|Terminate (강제)|Terminated|kill -9 명령 (절대 무시 불가)
                SIGTERM|15|Terminate|Terminated|kill 기본 명령 (종료 요청)
                SIGSEGV|11|Terminate + Core Dump|Terminated|잘못된 메모리 참조 (Segfault)
                SIGSTOP|17, 19, 23|Stop (강제)|Stopped|프로세스를 즉시 정지 (무시 불가)
                SIGTSTP|18, 20, 24|Stop,Stopped|Ctrl + Z 입력 시 (터미널 정지)
                SIGCONT|19, 18, 25|Continue|Resumed|정지된 프로세스를 다시 실행
                SIGCHLD|17, 20, 18|Ignore|(변화 없음)|자식의 상태가 변했음을 부모에 알림

                * SIGINT (Ctrl+C) can be handled by code within the program to "give it some time to save data before it dies," but stronger signals like SIGSTOP or SIGKILL cannot be rejected by the process.



    - **exit()**: The child process exits and signals the parent process. 
        - 'void exit(int status)' call has two consants, EXIT_SUCCESS(0) and EXIT_FAILURE(1 or other), for input argument. 


### simple blueprint
<img src="../img/basic-commands.png" style="width: 80%;">

### pseudo code for Basic Command

    ```C
    pid = fork();
    if (pid != 0)       // parent process
        wait(NULL);
    else if (pid == 0)  // child process
        {
            get PATH value string.
            dupstr = duplicate the value string. -> strtok changes the original value string.
            dir = strtok(dupstr, ":")
            while(dir != NULL)
            {
                full_path = dir + command_name
                if (access(full_path, X_OK) == 0)
                    execve(full_path, argv, environ)
                    break
                dir = strtok(NULL, ":")
            }
            exit();
        }
    ```

    
## 2. File I/O


### File Descriptor
- File Descriptor is an __Non-negative Integer__ that kernel manages. In Linux, when a process opens a file, the kernel writes the address of the kernel object containing the file's information into an empty space in the 'File Descriptor Table' dedicated to that process. It then informs the process of that 'space number (index),' which is the FD. 

- Linux follows the philosophy that "everything is a file." Therefore, not only ordinary files but also hardware devices, network sockets, and inter-process communication (IPC) channels are all handled through FDs using the same read() and write() interfaces.

- Standard FD
    - 0: stdin
    - 1: stdout (buffer)
    - 2: stderr (no buffer)

### 3-level File Management Structure Inside the Kernel
- Per-Process FD Table
    - Per-Process FD Table is included in Linux's task_struct(PCB).
    - The index is FD, and each entry hase a pointer (that points to a Wide-System Open Table) and a FD flag.
    - Valid only within the process. (Process A's 3 and Process B's 3 can point to completely different files.)

- System-wide Open File Table
    - This is a table that manages the status of all files currently open across the entire OS
    - File Offset: Location information regarding how far the current file has been read or written.
    - Status flags: Read-only, Write-only, Non-blocking, etc. (O_RDONLY, O_APPEND, etc.).
    - Reference Count: Records the number of FDs pointing to this open file description. (It is removed from the table when it reaches 0.)
- V-node / Inode Table
    - It is a table containing physical information about files stored on the actual disk.
    - Content: Metadata regarding file type, permissions, file size, location of data blocks on the disk, etc.
    - Features: Multiple open file descriptions can exist for the same file, but only one inode exists.

<img src="../img/3-phase-file-management.png" style="width: 100%;">

### FD Sharing and Independence
- Behavior of fork() (FD inheritance)
    - When you use fork(), the child copies the parent's process-specific FD table exactly as it is.
    - Effect: If the child reads the file and shifts the offset, the parent will also start reading from that shifted position when reading the file. (Offset shared)
- Behavior of double open()
    - Two FDs are created and Open File Table entry is created for each FD but both points to the same Inode. 
    - Effect: The two FDs have independent offsets. In other words, reading from one side does not affect the read position of the other.

### stdout, stdin, stderr
- __stdout(Standard Output)__: Sending data to a montior
    1. When a process calls printf, the data remains in the process's internal memory, having not yet even reached the kernel. 
        - Library Buffer: printf belongs to the C library (libc). The string "Hi" is initially stored in the user-level buffer managed by the stdout FILE struct object.
        - Flush & System Call: When a newline character (\n) is encountered or the buffer is full (Lined Buffer), the library calls the `write(1, "Hi", 2)` system call. At this point, it encounters the PCB's FD table holding the number '1 (stdout)
        - stdout is a FILE struct object that holds a few information about FD and user buffer. 
        ```C
        #include <stdio.h>

        struct _iobuf {
        char* _ptr;      // 버퍼 내의 현재 읽기/쓰기 위치
        int   _cnt;      // 버퍼에 남아있는 데이터의 양 (바이트 단위)
        char* _base;     // 버퍼의 시작 주소
        int   _flag;     // 파일 상태 플래그 (에러, EOF, 읽기/쓰기 모드 등)
        int   _file;     // 운영체제로부터 할당받은 파일 식별자 (File Descriptor)
        int   _charbuf;  // 버퍼가 없을 때 사용하는 임시 버퍼
        int   _bufsiz;   // 버퍼의 크기
        char* _tmpfname; // 임시 파일 이름
        };
        typedef struct _iobuf FILE;
        ```
    2. The kernel starts searching the process's resources using the received FD number 1.
        - Process FD Table: The kernel looks at the FD table in the corresponding process's PCB. The address of the 'open file table' is written in column 1 of the index.
        - [Open File Table]: The kernel follows the address to check the 'Open File Table'.
        - Since the hardware may be busy, the kernel safely copies the data to the kernel's internal terminal (TTY) buffer for the time being.
        - The open file table entry's information tells that the destination of this data is **Terminal (TTY)**, and it is write-only.
        - Inode Table (Device): It follows the pointer in the open file table to locate the **Device Inode**. This is the entry point connected to the actual terminal device driver.
    3. Device Driver & Monitor: "Hi" contained in the kernel buffer is passed to the graphics card according to the device driver's schedule and ultimately appears as a pixel on the monitor screen.

- __stderr__: Performs similarly as stdout, but does not go through a process of a user buffer and instead immediately writes to the kernel buffer.

- __stdin(Standard Input)__: Sending data from keyboard to a process memory
    1. When a process calls scanf, it does not run directly to the kernel but checks if there is data in the user space.
        - Check Library Buffer: scanf first flips the user-level input buffer managed by the stdin object (FILE structure)
            - If there is leftover data from a previous input (e.g., if "Hello World" was entered but the previous scanf only retrieved "Hello"), it is retrieved directly from the user buffer without requesting it from the kernel.
        - System Call Invocation: If the buffer is empty, the library calls the read(0, buf, size) system call. At this time, it requests data by presenting '0 (stdin)' to the kernel.
    2. Kernel Space
        - Process FD Table: The kernel looks at index 0 of the FD table. The address of the 'open file table' is written there.
        - Open File Table: It is recorded that this data is from the terminal (TTY) and is read-only.
        - Inode Table & Sleep: The kernel checks whether there is current keyboard input through the terminal device inode.
            - If there is no input data, the kernel switches the process to a sleep/waiting state. At this time, the process waits quietly without using the CPU.
    3. Hardware & Interrupt
        - Hardware Interrupt: When a user presses a key, the keyboard controller sends an interrupt signal to the CPU that data has arrived and processes it
        - Kernel TTY Buffer: The kernel's device driver intercepts input characters and stacks them sequentially in the kernel's terminal buffer. When Enter is pressed, the kernel determines that it is now safe to send the data.
    4. Process Wake-up and Parsing
        - Process Wake-up: The kernel wakes up a sleeping process (transitions to the Ready state).
        - Data Copy: Data contained in the kernel buffer is copied(read) to the library buffer in user space.
        - Parsing: Finally, scanf converts the string (e.g., "123") entered into the library buffer into a number according to the format requested by the user (e.g., %d) and stores it in a variable.

---
### System Calls
- File I/O and Redirection are a procedure that the shell and the __file__ communicate.


