# 1. Files and Directories
Shell can store data in the file system. File system has two components, file and directory. Through shell, we can view and change the file system. 

command|description
---|---
pwd|print working directory
ls|listing
cd|change directory

- pwd
    - print working directory

    ```bash
    $ pwd
    /home/jhparkk1023
    ```

    - The first / is the root directory. When / is inside, it is a seperator.

- ls
    - listing

    ```bash
    $ ls -F
    code  cpp/  csapp_exer_code/  dev/  eBPF/  lab/  project/  shell/  snap/  testwsl/
    ```

    - -F flag adds a trailing '/' to directory names.

    ```bash
    $ ls -F project
    a.out*  jhpsh/  test/
    ```

    - relative path relative to current working directory.

    ```bash
    $ ls -F /home/jhparkk1023/project
    a.out*  jhpsh/  test/
    ```

    - absolute path starting from root directory '/'.

- cd 
    - change directory
    ```bash
    $ cd jhpsh
    ```
    - Does not change the directory. Changes the shell's idae of which directory we are in.
    ```bash
    $ cd ..
    ```
    - Go back to the parent directory.
    ```bash
    $ ls -F -a
    ./             .bash_logout  .docker/     .lesshst     .sudo_as_admin_successful  code              lab/
    ../            .bashrc       .dotnet/     .local/      .viminfo                   cpp/              project/
    .aws@          .cache/       .gitconfig   .motd_shown  .vscode/                   csapp_exer_code/  shell/
    .azure@        .codegpt/     .kube/       .opam/       .vscode-server/            dev/              snap/
    .bash_history  .copilot/     .landscape/  .profile     .wget-hsts                 eBPF/             testwsl/
    ```

    - -a: show all.
    - ./: current directory
    - ../: parent directory


# 2. Creating and Deleting

command|description
---|---
mkdir|make directory
nano|text editor
rm|remove
mv|move a file
cp|copy a file


- mkdir
    - make directory
    
    ```bash
    $ mkdir tmp
    $ ls -a tmp
    .  ..
    ```
    
    - Make directory with name 'tmp'
    - . means current directory /home/jhparkk1023/tmp
    - .. menas parent directory /home/jhparkk1023

- nano
    - 'nano' is a text editor that only uses characters.
    ```bash
    $ cd tmp
    $ nano junk
    $ ls
    junk
    $ls -s
    total 4
    4 junk
    $ ls -s -h
    total 4.0K
    4.0K junk
    $
    ```

    - 'nano' has created a new text file named 'junk'.
    - ls -s -h: -s means size, and -h means human friendly. -s prints the disk blocks of the file and -h prints it with human friendly output
    - We can see 'junk' text file occupies 4 disk blocks and is 4.0K bytes (= 4 disk blocks) sized.


- rm
    - Remove (delete) file.
    ```bash
    $ rm junk
    $
    ```

    - There is no un-delete commands. 

    ```bash
    $ rm tmp
    rm: cannot remove 'tmp': Is a directory
    $ rmdir tmp
    $ 
    ```
    - 'rm' command only works on files.
    - To remove a directory, we use 'rmdir' command. Before we delete the 'tmp' file, we need to delete the inner file that is located inside of 'tmp' directory.

- mv
    - Move a file or directory.

    ```bash
    $ ls tmp
    junk
    $ mv tmp/junk tmp/quotes.txt
    $ ls tmp
    quotes.txt
    ```
    - Move tmp/junk to temp/quotes.txt, which in this case is equal to renaming the file.

    ```bash
    $ mv tmp/quotes.txt .
    $ ls tmp
    $ ls
    quotes.txt
    ```

    - Move /home/jhparkk1023/tmp/quotes.txt to /home/jhparkk1023/quotes.txt

- cp
    - Copies the file.

    ```bash
    $ cp quotes.txt tmp/quotations.txt
    $ ls quotes.txt tmp/quotations.txt
    quotes.txt  tmp/quotations.txt
    $ rm quotes.txt
    $ ls quotes.txt tmp/quotations.txt
    ls: cannot access 'quotes.txt': No such file or directory
    tmp/quotations.txt
    ```

# 3. Pipes and Filters
commands|description
---|---
*|wild card
wc|word count
\>|redirection
cat|concatenate
sort|sort
head|head
\||pipe


- other commands

connection|description
---|---
tail|
split|
cut|
uniq|
<|
?|



- Such commands can be more powerful and useful when used combined.

- \* & wc
    ```bash
    $ ls tmp
    apple.txt  banana.txt  cat.txt  dinner.txt  egg.txt  fish.txt
    $ cd tmp
    $ wc *.txt
    0 0 0 apple.txt
    0 0 0 banana.txt
    0 0 0 cat.txt
    0 0 0 dinner.txt
    0 0 0 egg.txt
    0 0 0 fish.txt
    0 0 0 total
    ```
    - '\*' is a wild card. It matches 0 or more characters s0 \*.txt matches all filenames ending in .txt.
    - 'wc' is word count command. It counts lines, words, and characters in files.

    ```bash
    $ wc -l *.txt
    0 apple.txt
    0 banana.txt
    0 cat.txt
    0 dinner.txt
    0 egg.txt
    0 fish.txt
    0 total
    ```
    - -l option report only lines. -w for words, -c for characters.

- \>
    ```bash
    $ wc *.txt
    1   1   6 apple.txt
    3   3  12 banana.txt
    4  11  50 cat.txt
    13  13 141 dinner.txt
    0   0   0 egg.txt
    0   0   0 fish.txt
    21  28 209 total
    $ wc -l *.txt > lengths
    ```

    - \> redirects output to a file.

- cat
    - concatenate files

    ```bash
    $ cat lengths
     1 apple.txt
     3 banana.txt
     4 cat.txt
    13 dinner.txt
     0 egg.txt
     0 fish.txt
    21 total
    ```
    - Concatenate 'lengths' file. The output of 'wc -l *.txt' is redirected to 'lengths' file.

- sort
    - Sort the content.
    ```bash
    $ sort lengths
      0 egg.txt
      0 fish.txt
      1 apple.txt
      3 banana.txt
      4 cat.txt
     13 dinner.txt
     21 total
    ```
    - 'sort' does not change the content of the original file.

    ```bash
    $ sort lengths > sorted-lengths
    $ cat sorted-lengths
      0 egg.txt
      0 fish.txt
      1 apple.txt  
      3 banana.txt
      4 cat.txt
     13 dinner.txt
     21 total
    ```
    - The output 'sort lengths' is redirected to 'sorted-lengths' file.

- head
    ```bash
    $ head -1 sorted-lengths
    0 egg.txt
    ```
    - 'head' gets the first line of the file.

- |
    - Pipe

     ```bash
    $ sort lengths | head -1
      0 egg.txt
    ```
    - Use output of left side as input to right side without creating temporary file.

     ```bash
    $ wc -l *.txt | sort | head -1
      0 egg.txt
    ```
    - The output of 'wc -l *.txt' is used as an input for 'sort', and the output of 'sort' is used as an input of 'head -1'.

    - - -
    Every process has a 'stdin' and 'stdout'. 
    
    The shell creates 3 process as a component of the pipe so the process runs simultaneously. The 'wc' stdout is fed to the stdin of 'sort'. 'sort' output goes to the stdin of 'head' and then the stdout of 'head' goes to the screen.

    This programming model called pipes and filters. 
    
    A filter transforms a stream of input into a stream of output. 
    
    A pipe connects two filters. 

    Any program that reads lines of text from standard input, and writes lines of text to standard output, can work with every other.
    - - -

# 4. Permissions

- User
    - User has 'user name' and 'user ID'. User name is a text and User ID is numeric.
- Group
    - Group has unique 'group name' and 'group ID'. User can belongs to zero or more groups.
    - List is stored in '/etc/group'
- All
    - Everyone else.

---
- Read, Write, Execute permissions.
- Ex)
    permission|user|group|all
    ---|---|---|---
    read|O|O|X
    write|O|X|X
    execute|X|X|X

---
```bash
$ ls -F
a.out*  a.txt  jhpsh/  test/
```

- In a.out*, * means executable.

```bash
$ ls -l
total 24
-rwxr-xr-x 1 jhparkk1023 jhparkk1023 16136 Feb 24 01:57 a.out
-rw-r--r-- 1 jhparkk1023 jhparkk1023     0 Apr 10 22:11 a.txt
drwxr-xr-x 6 jhparkk1023 jhparkk1023  4096 Apr  6 00:06 jhpsh
drwxr-xr-x 2 jhparkk1023 jhparkk1023  4096 Mar  6 16:32 test
```
permissions|NULL|user owner|group owner|byte size|last modified|name
---|---|---|---|---|---|---
-rwx r-x r-x |1 |jhparkk1023 |jhparkk1023 |16136 |Feb 24 01:57 |a.out


\-|rwx|r-x|r-x
---|---|---|---
file type|user owner permissions|group owner permissions|every one's permissions

```bash
$ ls -l -a
total 40
drwxr-xr-x  5 jhparkk1023 jhparkk1023  4096 Apr 10 22:11 .
drwxr-x--- 22 jhparkk1023 jhparkk1023  4096 Apr 10 22:11 ..
drwxr-xr-x  8 jhparkk1023 jhparkk1023  4096 Apr  6 00:24 .git
-rw-r--r--  1 jhparkk1023 jhparkk1023    79 Mar  5 01:28 .gitignore
-rwxr-xr-x  1 jhparkk1023 jhparkk1023 16136 Feb 24 01:57 a.out
-rw-r--r--  1 jhparkk1023 jhparkk1023     0 Apr 10 22:11 a.txt
drwxr-xr-x  6 jhparkk1023 jhparkk1023  4096 Apr  6 00:06 jhpsh
drwxr-xr-x  2 jhparkk1023 jhparkk1023  4096 Mar  6 16:32 test
```

- With -a option, we can see more details. In '.' and'..', 'd' means directory and in others are '-' meaning it is a file.
- In directory information, 'd' has permission 'x'. This means that it has the permission to traverse the directory. The directory 'jhpsh' is 'r-x', which menas it is possible to run 'ls -l jhsph'. But if the directory is '--x', it is not permitted to execute 'ls jhpsh' but it can see information of sub-directories of 'jhpsh', for example 'ls -l jhpsh/include'. It hides the details of the directory but it can traverse it.

---

* chmod
    - change mode.
    ```bash
    $ ls -l -a a.txt
    -rw-r--r-- 1 jhparkk1023 jhparkk1023 0 Apr 10 22:11 a.txt
    ```
    - In this case, the file a.out's permission is rw-r--r--. In order to change the permission, we use 'chmode'.


    ```bash
    $ ls -l -a a.txt
    -rwxr--r-- 1 jhparkk1023 jhparkk1023 0 Apr 10 22:11 a.txt
    ```

    - We change the user's permission 'rw-' to 'rwx'.

    ```bash
    $ chmod u=rw a.txt; ls -a -l a.txt
    -rw-r--r-- 1 jhparkk1023 jhparkk1023 0 Apr 10 22:11 a.txt
    ```

    -  ; allows us to put multiple commands on a single line.
    - u=, g= a= each equals to user, group, all.

    ```bash
    $ ls
    built-ins.c  execute.c  jhpsh_loop.c  main.c  memory.c  parser.c  prompt.c
    $ cat > smallest
    wc -l *.c | sort | head -1
    ^D
    $ ls -a -l smallest
    -rw-r--r-- 1 jhparkk1023 jhparkk1023 27 Apr 10 22:42 smallest
    $ chmod u+x smallest
    $ ls -a -l smallest
    -rwxr--r-- 1 jhparkk1023 jhparkk1023 27 Apr 10 22:42 smallest
    $ ./smallest
    9 memory.c
    ```

    - cat > smallest: 'cat' program waits for the user's input. When the user types 'wc -l *.c | sort | head -1' and '^D', the input of 'cat' is 'wc -l *.c | sort | head -1' and it redirects the output 'wc -l *.c | sort | head -1' to the file name 'smallest'. ^D means 'end of input'.
    - File 'smallest' has rw- permission for the user. We change the permission to rwx using chmod.
    - ./smallest means that in the current directory '.', there is a file name 'smallest' and we execute the file. 
    
# 5. Finding Things

command|decription
---|---
grep|global / regular expression / print
find|find file

* grep: group / regular expression / print
    - Finds and prints lines in files that match a pattern. 

    ```bash
    $ grep thee eric.txt
    How do I love thee? Let me count the ways.
    I love thee to the depth and breadth and height
    I love thee to the level of every day’s
    I love thee freely, as men strive for right.
    I love thee purely, as they turn from praise.
    I love thee with the passion put to use
    I love thee with a love I seemed to lose
    With my lost saints. I love thee with the breath,
    I shall but love thee better after death.
    ```

    - 'grep' finds the pattern with 'thee' and prints lines that have such pattern in line in eric.txt file.

    ```bash
    $ grep ly eric.txt
    I love thee freely, as men strive for right.
    I love thee purely, as they turn from praise.
    ``` 

    - 'grep' finds pattern that has 'ly'.rn

    ```bash
    $ grep -n ly eric.txt
    7:I love thee freely, as men strive for right.
    8:I love thee purely, as they turn from praise.
    
    ```
    - The default is printing line. -w for the whole word (ex: grep -w day [file] -> this day is great! [O], erverday is great![X]) and -n for which line has the patten
    - -i: case insensitive
    - -v: invert and print non-matches.

- find: finds files (rather than lines in files)

    ```bash
    $ find . -type d
    [directories list]
    ```

    - . means root directory. This commands finds directory that is in the root directory.

    ```bash
    $ find . -maxdepth 1 -type d
    [directories list]
    ```

    - Find all directories that are in maxdepth 1 in root directories.

    ```bash
    $ find . -empty
    [empty directories list]
    ``` 

    - Find all directories that are empty.

    ```bash
    $ find . -perm -u=x
    [directories list with user permission 'x']
    ```

    - Finds directories and files that have exeucte user permission.

    ```bash
    $ find -name *.txt
    [.txt files]
    ```

    - The shell expands *.txt to actual name txt file that is in root directory (depth = 1)before performing 'find' command.
    ```bash
    $ find -name '*.txt'
    [all .txt files that are in root directories]
    ```

    - Single quotes prevent shell from expanding wildcards. This gets the pattern for 'find'.

    ```bash
    $ wc -l `find -name '*.txt'`
      0 ./banana.txt
      0 ./apple.txt
     14 ./eric.txt
     14 total
     ```

    - Back quotes replace what's inside with output from running that command.

# 6. Job Control

command|description
---|---
jobs|shows background processes
bg|run on background
fg|run on foreground
kill|kill the process

-  Can stop, pause, and resume running processes

    ```bash
    $ ./analyze results*.dat
    ...a few minutes pass...
    ^C
    $ ./analyze results*.dat &
    $ jobs
    [1] ./analyze results01.dat results02.dat results03.dat
    $ fg
    ...a few minutes pass...
    $ 
    ```

    - ./analyze results*.dat takes a few minutes to finish the job and the shell waits, preventing us from getting new prompt to command. So we ^C to stop the running program and type ./analyze results*.dat &. & means run in the background. Shell returns right away insead of waiting for the program to finish. 
    - 'jobs' shows background processes
    - fg, fg %1, fg %2.... bring background job to foreground. We use % if there are several background jobs.
    - After the foreground jobs are finished, the prompt appears.

    ```bash
    $ ./analyze results01.dat
    ^Z
    [1] Stopped   ./analyze results01.dat
    $ bg %1
    $ jobs
    [1] ./analyze results01.dat
    $ kill %1
    $
    ```
    - ^Z pauses a program that's already running.
    - 'fg' to resume it in the foreground or 'bg' to resume it as a background job. 'kill %1' is same as stopping the foreground process using ^C.

    - Job control mattered a lot when users only had one terminal window.
    - Less important now: just open another window.
    - Still useful when running programs remotely.  
    - ^C stops the foreground process, 'kill' stops foreground or background process. ^Z pauses the foreground process

# 7. Variables

commands|description
---|---
set|show environment variables
echo|print the value of variable
export|variable is valid in subproc
alias|register long commands to short ones

- The shell is a program and has variables. The variables control the shell execution, and when we change their values, it is possible to change its behavior.

- set: With no arguments, shows all the environment variable names and its value.
    ```bash
    $ set
    BASH=/bin/bash
    HOME=/home/jhparkk1023
    PATH='/home/jhparkk1023/.opam/default/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/usr/lib/wsl/lib:/mnt/c/Program Files/WindowsApps/MicrosoftCorporationII.WindowsSubsystemForLinux_2.6.3.0_x64__8wekyb3d8bbwe:/mnt/c/app/jhpar/product/21c/dbhomeXE/bin:/mnt/c/WINDOWS/system32:/mnt/c/WINDOWS:/mnt/c/WINDOWS/System32/Wbem:/mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0/:/mnt/c/WINDOWS/System32/OpenSSH/:/mnt/c/Program Files/Git/cmd:/mnt/c/Program Files/Docker Machine:/mnt/c/minikube:/mnt/c/Program Files/Docker/Docker/resources/bin:/mnt/c/MinGW/bin:/mnt/c/Program Files (x86)/Windows Kits/10/Windows Performance Toolkit/:/mnt/c/Program Files/Bandizip/:/mnt/c/Users/jhpar/pilsaProject/jdk-17/bin:/mnt/c/Program Files/dotnet/:/mnt/c/Users/jhpar/AppData/Local/Microsoft/WindowsApps:/mnt/c/Users/jhpar/AppData/Local/Programs/Microsoft VS Code/bin:/mnt/c/Program Files/Docker Machine:/mnt/c/Users/jhpar/AppData/Local/GitHubDesktop/bin:/mnt/c/Program Files/JetBrains/IntelliJ IDEA 2025.3.1.1/bin:/mnt/c/Users/jhpar/pilsaProject/jdk-17/bin:/snap/bin'
    PPID=15950
    SHELL=/bin/bash
    UID=1000
    USER=jhparkk1023
    ...
    ```
    - All variables are upper-case.
    - All values are strings. Programs must convert to other types when/as necessary. Ex: In PPID=15950, 15950 is a string and must be converted into int.
    - ":"is to split the string values.
        - PATH controls where the shell looks for programs.
        ```bash
        $ ./analyze
        ``` 
        - ./analyze means to run the 'analyze' program in the current directory.
        ```bash
        $ /bin/analyze
        ```
        - Run the analyze program in the /bin directory. 
        ```bash
        $ analyze
        ```
        - The shell looks the variable PATH, and splits its value with ":". Then for each directory, if directory/analyze exists, the shell runs the program. 

        ```
        /usr/local/bin
        /usr/bin
        /bin                            /bin/analyze
        /cygdrive/c/Windows/systems32   
        /cygdrive/c/Windows
        /cygdrive/c/bin                 /cygdrive/c/bin/analyze
        /cygdrive/c/Python27
                                        /users/vlad/analyze
        ```
        - Let's assume that PATH has value that is described above and the program 'analyze' is in 3 different directories. The shell looks the PATH variable to see if 'analyze' program is in the directories described above. It founds out that 'analyze' program is in /bin directory and runs the program. The program 'analyze' that is in /users/vlad directory will never be found by the shell who looks for the directory in PATH. 

- echo
    - echo prints its argurment. It can be used to print the value of variables. 

    ```bash
    $ echo hello
    hello
    $ echo HOME
    HOME
    $ echo $HOME
    /home/jhparkk1023
    ```
    - $ asks shell to replace variable name with value before program runs just like * and ? are expanded before the program runs
        - echo $HOME -> echo /home/jhparkk1023

- Create variable by assigning to it and change values by reassiging to existing variables
    ```bash
    $ SECRET_VAR=Eric_Park
    $ echo $SECRET_VAR
    Eric_Park
    ```

    - Assignment only changes variable's value in the current shell that you are working on. 

    ```bash
    $ SECRET_VAR=Eric_Park
    $ echo $SECRET_VAR
    Eric_Park
    $ bash
    $ echo $SECRET_VAR

    $ exit
    exit
    $ echo $SECRET_VAR
    Eric_Park
    ```

    - The variable is not valid in other child process shell.

- Use export to signal that the variable should be visible to subprocesses.

    ```bash
    $ export SECRET_VAR
    $ bash
    $ echo $SECRET_VAR
    Eric_Park
    $ exit
    exit
    $ 
    ```

- When we want to set some variables automaticlly everytime we run a shell, put the commands in $HOME/.bashrc and the commands will be executed when shell starts.
    - ./bashrc file
    ```
    export SECRET_VAR=Eric_Park
    export BACK_UP=$HOME/backup
    ```

- alias: This feature allows you to create short shortcuts for complex or long commands, typically registered in the ~/.bashrc or ~/.zshrc files for permanent use.
    ```
    alias backup=/bin/zarble -v --nostir -R 20000 $HOME $BACKUP_DIR
    ```


