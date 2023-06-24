# Minishell

```
<ALPHA> ::= a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|
             A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z

<DIGIT> ::= 0|1|2|3|4|5|6|7|8|9

<NUMBER> ::= <DIGIT>
           | <NUMBER> <DIGIT>

<WORD> ::= <ALPHA>
         | <WORD> <ALPHA>
         | <WORD> '_'

<WORD-LIST> ::= <WORD>
             |  <WORD-LIST> <WORD>


<REDIRECTION> ::=  '>' <WORD>
                |  '<' <WORD>
                |  '>>' <WORD>
                |  '<<' <WORD>

<REDIRECTION-LIST> ::= <REDIRECTION>
                    |  <REDIRECTION-LIST> <REDIRECTION>

<SIMPLE-COMMAND-ELEMENT> ::= <WORD>
                          |  <REDIRECTION>

<SIMPLE-COMMAND> ::=  <SIMPLE-COMMAND-ELEMENT>
                   |  <SIMPLE-COMMAND> <SIMPLE-COMMAND-ELEMENT>

<SUBSHELL> ::=  '(' <LIST>')'

<SHELL-COMMAND> ::=  <SUBSHELL>

<COMMAND> ::=  <SIMPLE-COMMAND>
            |  <SHELL-COMMAND>
            |  <SHELL-COMMAND> <REDIRECTION-LIST>
            |  <PIPELINE>

<PIPELINE> ::=  <PIPELINE> '|' <COMMAND>
             |  <COMMAND>

<LIST> ::=  <COMMAND>
         |  <LIST> '&&' <COMMAND>
         |  <LIST> '||' <COMMAND>
         |  <PIPELINE>


```

## Resources:

[https://cmdse.github.io/pages/appendix/bash-grammar.html](https://cmdse.github.io/pages/appendix/bash-grammar.html)

[https://github.com/fraqioui/minishell](https://github.com/fraqioui/minishell)

// test
echo f << h word
echo 1 && echo 2 | echo 3
// segv whith int the second child bc its a subshell:
echo 1 | (echo 2 && echo 3) | echo 4
// problem with pipe :
(echo 1 && echo 2) | echo 3 