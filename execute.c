#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./headers/execute.h"

#define BUFFER_LIMIT 1024
#define MAX_OP_TOKENS 32
#define MAX_COMMANDS 32


// spilting if thir is a pipe or not ,
//not so clean but easer for now

void the_execution(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        if (chdir(args[1]) == -1) {
            perror("cd failed");
        }
    }

    pid_t pid = fork();

    if (pid == 0) {
        execvp(args[0], args);
        perror("exec failed");
        _exit(1);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        printf("\nexit code: \033[38;2;130;168;216m%d\033[0m\n", status);
    } else {
        perror("fork failed");
        _exit(-1);
    }
}

/*
 * Splits args on "))" tokens into separate commands, wires them together
 * with pipes (N commands need N-1 pipes), forks one child per command,
 * and waits for all of them.
 *
 * Example: ls -l )) grep foo )) wc -l
 *   -> 3 commands, 2 pipes
 */

 
void execute_pipeline(char **args) {
    int num_cmds = 1;
    for (int i = 0; args[i]; i++) {
        if (strcmp(args[i], "))") == 0) num_cmds++; // count the commands
    }

    if (num_cmds > MAX_COMMANDS) {
        fprintf(stderr, "too many piped commands (max %d)\n", MAX_COMMANDS);
        return;
    }

    // Split args into MAX_COMMANDS command arrays, each MAX_OP_TOKENS long
    char *commands[MAX_COMMANDS][MAX_OP_TOKENS];
    int cmd_idx = 0, tok_idx = 0;


    for (int i = 0; args[i]; i++) {
        if (strcmp(args[i], "))") == 0) {
            if (tok_idx >= MAX_OP_TOKENS) {
                fprintf(stderr, "too many tokens in one command\n");
                return;
            }
            commands[cmd_idx][tok_idx] = NULL;
            cmd_idx++;
            tok_idx = 0;
        } else {
            if (tok_idx >= MAX_OP_TOKENS - 1) {
                fprintf(stderr, "too many tokens in one command\n");
                return;
            }
            commands[cmd_idx][tok_idx++] = args[i];
        }
    }
    commands[cmd_idx][tok_idx] = NULL; //last command

    int num_pipes = num_cmds - 1;
    int pipefds[MAX_COMMANDS][2];

    for (int i = 0; i < num_pipes; i++) {
        if (pipe(pipefds[i]) == -1) {
            perror("pipe failed");
            _exit(1);
        }
    }

    pid_t pids[MAX_COMMANDS];

    for (int i = 0; i < num_cmds; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // child: read from previous pipe if there is
            if (i > 0) {
                dup2(pipefds[i - 1][0], STDIN_FILENO);
            }
            // child: write to next pipe if there is
            if (i < num_cmds - 1) {
                dup2(pipefds[i][1], STDOUT_FILENO);
            }

            for (int j = 0; j < num_pipes; j++) {
                close(pipefds[j][0]);
                close(pipefds[j][1]);
            }

            execvp(commands[i][0], commands[i]);
            perror("exec failed");
            _exit(1);
        } else if (pid < 0) {
            perror("fork failed");
            _exit(1);
        }

        pids[i] = pid;
    }

    // parent close 
    for (int j = 0; j < num_pipes; j++) {
        close(pipefds[j][0]);
        close(pipefds[j][1]);
    }

    int status;
    for (int i = 0; i < num_cmds; i++) {
        waitpid(pids[i], &status, 0);
    }
}




void main_execute(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        if (chdir(args[1]) == -1) {
            perror("cd failed");
        }
        return;
    }

    int has_pipe = 0;
    for (int i = 0; args[i]; i++) {
        if (strcmp(args[i], "))") == 0) {
            has_pipe = 1;
            break;
        }
    }

    if (!has_pipe) {
        the_execution(args);
        
    } else {
        execute_pipeline(args);
    }
}
