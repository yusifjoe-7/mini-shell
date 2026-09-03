#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#include "./headers/shell.h"
#include "./headers/token.h"
#include "./headers/execute.h"

#include "./headers/data.h"



void shell(){

    char input[INPUT_LIMET];
    char *args[ARGS_LIMER];
    char cwd[INPUT_LIMET];

    while (1) {
        
        if (getcwd(cwd, INPUT_LIMET) != NULL){
            printf("{ \033[38;2;125;196;168m%s\033[0m } shell \033[38;2;184;161;227m=>\033[0m ", cwd);
        }
        else{
            printf("?? shell \033[38;2;184;161;227m=>\033[0m ");
        }
        fflush(stdout);

        char *result = fgets(input, INPUT_LIMET, stdin);


        if(result == NULL){
            perror("get ERROR");
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        if(strcmp(input, "quit") == 0)
            break;



        get_tokens(input, args);
        

    //    if (strcmp(args[0], "cd") == 0) {
    //     if(chdir(args[1]) == -1){
    //         perror("can not found\n");
    //     }
    //    }
    //    else {
    //     pid_t code = fork();

    //     if( code == 0){
    //         // printf("\n");
    //         execvp(args[0],args );
    //         perror("something whent wrong while exec\n");
    //         _exit(1);
    //     }else if (code > 0) {
    //         int status;
    //        waitpid(code, &status, 0);
    //         printf("\nexit code: \033[38;2;130;168;216m%d\033[0m \n",status);
            

    //     }else {
    //         perror("there is an error");
    //     }
    //    }

    main_execute(args);



    }
    

}




// if thir is not pipes 
//      fork => execvp => 

// if ther is
// calulate => crate 