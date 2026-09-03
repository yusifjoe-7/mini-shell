#include <string.h>

#include "./headers/data.h"
#include "./headers/token.h"



void get_tokens(char *input, char **args){
    char *token = strtok(input, " ");

        int i = 0;

        while (i<ARGS_LIMER && token != NULL) {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;   
        }
        args[i] = NULL;
}