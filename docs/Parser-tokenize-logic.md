# Parser

- Parser used in this project (shell) is designed by myself and is very simple, not error-aware. I assumed that the input(command) which the user enters to the shell's prompt is grammarly correct. 


### 1. Lexer

- Few things I considered for tokenizing inputs:
    - Blank Space: Usually, input's tokens are seperated between blank space. But |, <, > tokens do not need to be seperated by blank space but instead can be the "border token" itself.
        - Ex
            - ls -l > list.txt
            - ls -l>list.txt

    - Characters in quotes are judged as ordinary tokens. Characters inside the quotes are all normal characters and not any special tokens.
        - Ex
            - echo hello
            - echo "hello"

    - Default size of memory allocation for Token is 10. If the input's token is bigger than 10, reallocate more memory space for the tokens.

```
memory allocation = 10 memory space for Tokens

while(meets the end of the input (= \0)):
{
    in_quote == false                           in_quote == true

        curr == '|':                            curr != '"', '\'':
            PIPE TOKENIZE                           curr++;
            curr++
            if (curr != space):                 curr == '"', '\'':
                start = curr                        WORD TOKENIZE
                                                    in_quote = false
        curr == '>':                                curr++
            REDIR_OUT TOKENIZE
            curr++
            if (curr != space):
                start = curr

        curr == '<':
            REDIR_IN TOKENIZE
            curr++
            if (curr != space):
                start = curr

        curr == '&':
            BG TOKENIZE
            curr++
            if (curr != space):
                start = curr

        curr == 일반 문자인 경우:
            curr++
            if (curr == isspace, |, <, >, &, \0):
                WORD TOKENIZE

        curr == '"', '\'':
            in_quote = true
            curr++

        curr == isspace:
            curr++
            if (curr != isspace):
                start = curr


    if token size are bigger than capacity, reallocate more memory space
}

END TOKENIZE
```