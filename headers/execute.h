#ifndef EXECUTE_H
#define EXECUTE_H

void the_execution(char **args);

void loop_execution(unsigned char *count_of_pipes, int pipes_in[][2], int pipes_out[][2], char **args);

void main_execute(char **args);

#endif