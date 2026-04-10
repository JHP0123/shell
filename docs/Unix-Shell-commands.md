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
    