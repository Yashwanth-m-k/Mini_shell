#include "mini.h"  

char external[200][50];

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};
   pid_t child_1;
   pid_t child_2;
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
            // printf("hiii\n");
            strcpy(prompt, str);  // Update prompt
        }
        }
        else{
           
get_command(input_string,str);

//printf("%s",str);
  int ret = check_command_type(str,external);

 

 if(ret == 1){
    
     execute_internal_commend(input_string,str);
  }
  else if(ret == 2){
     
    
    pid_t child;
   
    child=fork();
    
  
    if(child == 0){
        //child    
        
     execute_exteranl_command(input_string);
     exit(0);
    }
    else{
        //parent
        int status;
        child_1 = wait(&status); 
        child_2=getpid();
        printf("Child exit status = %d\n",child_1);
         printf("Child exit status1 = %d\n",getpid());

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
void execute_exteranl_command(char *input_string)
 {

    char *commend[50];  // Array of string pointers
    int i = 0,flag=0;
    
    // Tokenizing input_string into words
    char *token = strtok(input_string, " ");
    while (token != NULL) {
       
        if((strcmp(token, "|"))==0){
         
               flag=1;
           }
       
        commend[i++] = token;
        
        token = strtok(NULL, " ");  
    
    }
    
    commend[i] = NULL;  // NULL-terminate for execvp


   if(flag==1){

    printf("pipe is present\n");

   }
   else{
    execvp(commend[0], commend);  // Execute command
    
    perror("execvp");  // If execvp fails
    exit(EXIT_FAILURE);
   }
}
void execute_internal_commend(char *input_string,char *str)
{
    int ret;
    
    if(strcmp(input_string,"exit")==0)
    {
        exit(0);
    }
    if(strcmp(input_string,"pwd")==0)
    {
       // getpwd(input_string);
    }
    if(strcmp(input_string,"cd")==0)
    {
        ret=chdir(input_string);
        if(ret == -1){
            printf("not a valid path\n");
        }
        
    }
    if(strcmp(input_string,"echo $?")==0)
    {
        printf("%d\n",child_2);
    }
    if(strcmp(input_string,"echo $$")==0)
    {
        printf("%d\n",child_1);
    }
    if(strcmp(input_string,"echo $SHELL")==0)
    {
        char *env;
        env=getenv(input_string);
        if(env == NULL)
        {
            printf("Invalid \n");
        }
        else
        printf("%s",env);

    }

    
}

