#include "mini.h"  

char external[200][50];

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};

void extract_external_commands(char external[200][50]) {
    memset(external, 0, 200 * 50 * sizeof(char)); 
    int fd = open("command.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    int ret, i = 0, j = 0;
    char buffer;

    while ((ret = read(fd, &buffer, 1)) > 0) {
        if (buffer == '\n') {
            i--;
            external[j][i] = '\0';  // Null-terminate the string

         //   printf("Stored command: %s\n", external[j]);
            j++;

            if (j >= 200) {
                printf("Warning: Maximum command storage limit reached.\n");
                break;
            }

            i = 0;  // Reset index for the next command
        } else {
            if (i < 49) {  // Prevent buffer overflow
                external[j][i] = buffer;
                i++;
            }
        }
    }

    close(fd);
}

void scan_input(char *prompt, char *input_string) {
    while (1) {
        
        printf("%s", prompt);
    scanf("%[^\n]", input_string);  // Added space before %[^\n] to consume '\n'
        __fpurge(stdin);
        // printf("%c", input_string[0]);
        int len = strlen(input_string);
        // printf("len = %d", len);
        //  if(input_string == NULL){
        //         continue;
        // }
        char str[100];  // Increased size to avoid buffer overflow
        char *buffer = strstr(input_string, "PS1");

        if (buffer != NULL && strncmp(buffer, "PS1", 3) == 0) {
            int i = 4, j = 0;
            
                 
            while (j < len) {

                if (input_string[j] == ' ') {
                    printf("PS1: command not found\n");
                    i=1;
                    break;
                }
                else{
                      str[j++] = input_string[i++];
                }
            }

            
          if(i!=1){
            str[j] = '\0';  // Null-terminate
            strcpy(prompt, str);  // Update prompt
        }
        }
        else{
get_command(input_string,str);
//printf("%s",str);
  int ret = check_command_type(str,external);

 if(ret == 1){
     printf("Builtin\n");
  }
  else if(ret == 2){
    printf("External\n");
    pid_t child;
    child=fork();
    if(child == 0){
        //child
     execute_exteranl_command(input_string);
    }
    else{
        //parent

    }
 }                                                           	

  else if(ret == 3){
    printf("Command '%s' not found\n",str);
         
  }
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
int check_command_type(char *command, char external[200][50]) {
    int i = 0;

    // Check built-in commands
    while (builtins[i] != NULL) {
        if ((strcmp(builtins[i], command)) == 0) {
            return BUILTIN;  // Built-in command
        }
        i++;
    }

   for (i = 0; i < 152; i++) {
      

       // printf("Comparing Input %s with External[%d]: %s\n", command, i, external[i]); // Debug

        if ((strcmp(external[i], command)) == 0) {
            
            return EXTERNAL;  // External command
        }
        // int len1=strlen(external[i]);
        //  int len2=strlen(command);
        //     printf("l1 = %d\n l2 = %d\n",len1,len2);
    }
            
           
    return NO_COMMAND;  // No command found
}
void execute_exteranl_command(char *input_string){

    char commend[10][20];
    int i=0,j=0;
    while(input_string[i] != '\0' ){
        if(input_string[i] == ' ' ){
            commend[j][i]='\0';
            j++;
            i=0;
            continue;
        }
        else{
                commend[j][i]=input_string[i];
                i++;
        }
    }
    printf("->%s\n",commend[j]);
}

