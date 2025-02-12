#include "mini.h"  

char external[200][10];
char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};


void scan_input(char *prompt, char *input_string) {
    while (1) {
        printf("%s", prompt);
        scanf(" %[^\n]", input_string);  // Added space before %[^\n] to consume '\n'
        char str[100];  // Increased size to avoid buffer overflow
        char *buffer = strstr(input_string, "PS1");
        if (buffer != NULL && strncmp(buffer, "PS1=", 4) == 0) {
            int i = 4, j = 0;
            int len = strlen(input_string);
            

            while (i < len) {
                if (input_string[i] == ' ') {
                    printf("PS1: command not found\n");
                    break;
                }
                str[j++] = input_string[i++];
            }

            str[j] = '\0';  // Null-terminate
          
            strcpy(prompt, str);  // Update prompt
        }
get_command(input_string,str);
printf("%s",str);
  int ret = check_command_type(str);

 if(ret == 1){
     printf("No commands\n");
  }
  else if(ret == 2){
    printf("External\n");
 }
  else if(ret == 3){
         printf("Builtin\n");
  }
     }

}
char *get_command(char *input_string,char *str)
{
   
    int i=0,j=0;
    while (input_string[i] != '\0')
    {
        if(input_string[i] == ' '){
            break;
        }
       str[j]=input_string[i]; 
       i++;
       j++;
    }
    str[j]='\0';
    return str;
}
int check_command_type(char *command) {
    int i = 0;

    // Check built-in commands
    while (builtins[i] != NULL) {
        if ((strcmp(builtins[i], command)) == 0) {
            return 3;  // Built-in command
        }
        i++;
    }

    // Check external commands
    for (i = 0; i < 200; i++) {
        if ((strcmp(external[i], command)) == 0) {
            return 2;  // External command
        }
    }

    return 1;  // No command found
}


