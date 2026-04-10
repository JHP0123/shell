# Project Motivation & Background

- Deep Understanding of Operating Systems
    - The primary goal was to build a personal shell to gain hands-on experience with various system calls and analyze the core functions of a shell to understand how they operate within an OS environment.
- Learning through Implementation
    - While developing the initial "jhpsh" shell, I realized that implementing a shell from scratch is one of the most effective ways to master system programming.
- Limitations of the Previous Version (jhpsh)
    - The initial version had basic implementations of ls, cd, and echo.
    - However, it lacked scalability; adding new commands or features often required extensive refactoring of the existing codebase.
- Technical Growth
    - Through the previous iteration, I gained a solid understanding of critical system calls such as fork(), execve(), and wait(), as well as a high-level grasp of I/O flow.
- Ultimate Goal
    - To move beyond the limitations of the original jhpsh and build a shell that functions like a real-world shell (e.g., Bash). This involves:
        - Implementing a general parser.
        Creating a consistent execution pipeline that clearly distinguishes between built-in functions and external commands.
        - Shell commands such as cd, ls, echo, and other complex commands like grep, >, |, etc. These commands will help me understand how file descriptor or redirection actually works. 