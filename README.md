# Mini Shell

A simple Unix shell written in **C** for learning how processes, pipes, and command execution work.

## Features

* Execute Linux commands using `fork()` and `execvp()`
* Built-in `cd` command
* Command tokenization with `strtok()`
* Pipelines using `pipe()` and `dup2()`
* Process synchronization with `waitpid()`
* `quit` command

## Build

```bash
gcc main.c shell.c token.c execute.c -o shell
./shell
```

## Examples

```text
{ /home/user } shell => ls
{ /home/user } shell => cd ..
{ /home } shell => ls )) grep ".c"
```

Two pipes:

```text
{ /home/user } shell => ls )) grep ".c" )) wc -l
```

`))` is used as the pipe operator.

## Structure

```text
main.c       → Program entry
shell.c      → Shell loop & input
token.c      → Tokenization
execute.c    → Commands & pipelines
headers/     → Header files
```

A small project focused on understanding **C, Unix processes, file descriptors, and IPC**.

