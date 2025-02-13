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
#include"mini.h"

char  *external[200];


void extract_external_commands(char *external[200]) {
    int fd = open("command.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    int ret, i = 0, j = 0;
    char buffer;
    char temp[50];
    while ((ret = read(fd, &buffer, 1)) > 0)
     {
       
       // printf("-> %c",buffer);
        if (buffer == '\n') 
        {
            temp[i]='\0';

            
            external[j]=temp;
            printf("First command\n");
           
            printf(" ->%s",external[j]);
            j++;

              if (j >= 200) {
                printf("Warning: Maximum command storage limit reached.\n");
                break;
            }

            i = 0;  
        }
        else{
            temp[i]=buffer;
            
             printf("I value => %d\n",i);
              i++;
        }
            
 } 
            printf(" ->%s",external[10]);
    close(fd);
    }
     


    
int main(){
    char input_string[100];
    char prompt[100] = "Minishell$:";
    system("clear");
    extract_external_commands(external);
   //  scan_input(prompt,input_string);
    return 0;
}