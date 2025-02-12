#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "mini.h"  
void scan_input(char *prompt, char *input_string){
    char *buffer=strstr(input_string,"PS1");
    int i=0;
            int len=strlen(input_string);
            char str[10];
            int j=0;
    while(1){
        printf("%s",prompt);
        scanf("%[^\n]",input_string);
        
        if(buffer != NULL && (strncmp(buffer,"PS1",3))==0)
        {
            
            while(i < len){
                if(input_string[i] == ' '){
                    printf("PS1: command not found\n");
                    break;
                }
                if(i>3){
                        str[j]=input_string[i];
                        j++;
                }
                i++;
            }
            printf("%s",str);
           strcpy( prompt,str);
           scanf("%[^\n]",input_string);
        }


        
    }
}



