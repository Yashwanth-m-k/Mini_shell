#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

typedef struct node
{
    /* data */
     pid_t id;
     char str[50];
     struct node *link;
  
     
}slist;


#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

extern char external[200][50]; 

                        
void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string,char *str);

void copy_change(char *prompt, char *input_string);


void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void signal_handler(int sig_num);
void extract_external_commands(char external[200][50]);  
int check_command_type(char *command,char external[200][50]);

//
void execute_exteranl_command(char *input_string);
void execute_internal_commend(char *input_string,char *str);
void handler(int num);
int insert_at_last(slist **head);
void print_list(slist *head);
void fg_command_execution(slist **head);
void pipe_command_execution(char **command,int *pipe_count);
#endif