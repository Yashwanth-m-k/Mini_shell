#include "mini.h"  
int static flg1=0;
char external[200][50];

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};
   pid_t child_1,child_2,child;
   char input[100];
   slist *head=NULL;
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
                external[j][i++] = buffer;
                
            }
        }
       
    }

    close(fd);
}

void scan_input(char *prompt, char *input_string) {
    
 
    
    while (1) {
      
        printf("%s", prompt);
        if (!fgets(input_string, 100, stdin)) continue;
        input_string[strcspn(input_string, "\n")] = 0;  // Remove newline

        // printf("%s\n",input_string);
   
        // scanf("%[^\n]", input_string);  // Added space before %[^\n] to consume '\n'
    //     __fpurge(stdin);
    strcpy(input,input_string);
        int len=strlen(input_string);
      if(len == 0){
        flg1=1;
         continue;
      }
      
        char str[100];  // Increased size to avoid buffer overflow
        char *buffer = strstr(input_string, "PS1");

        if (buffer != NULL && strncmp(buffer, "PS1", 3) == 0)
         {
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

  int ret = check_command_type(str,external);

 if(ret ==BUILTIN){
    
     execute_internal_commend(input_string,str);
  }
  else if(ret ==EXTERNAL){
     
    child=fork();
    
  
    if(child == 0){
        //child    
      
        signal(SIGINT,SIG_DFL);
      execute_exteranl_command(input_string);
        
    }
    else{
        //parent
       
        int status;
        child_1 = wait(&status); 
        child_2=getpid();
        
        
    }
 }                                                           	
  else if(ret == NO_COMMAND)
  {
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

        if ((strcmp(external[i], command)) == 0) {
           
            return EXTERNAL;  // External command
        }
      
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
   }
  
}
void execute_internal_commend(char *input_string,char *str)
{
    int ret;
    
    if (strcmp(input_string, "exit") == 0) exit(0);
    if (strcmp(input_string, "pwd") == 0) {
        char cwd[100];
        getcwd(cwd, 100);
        printf("%s\n", cwd);
        return;
    }
    if (strncmp(input_string, "cd ", 3) == 0) {
        char *dir = input_string + 3;
        if (chdir(dir) == -1) perror("Invalid path");
        return;
    }
    if (strcmp(input_string, "echo $?") == 0) {
        printf("%d\n", child_2);
        return;
    }
    if (strcmp(input_string, "echo $$") == 0) {
        printf("%d\n", child_1);
        return;
    }
    if(strcmp(input_string,"echo $SHELL")==0)
    {
        char *env;
        env=getenv("SHELL");
        if(env == NULL)
        {
            printf("Invalid \n");
        }
        else{
        printf("%s\n",env);
        }

    }

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
   }
    
}
void handler(int num) {
    
    if (num == SIGINT) {
          // Handle Ctrl+C
         printf("\n"); 
        if(child == 0 || flg1==1)          
        printf("Minishell$: ");
        fflush(stdout);
        flg1=0;
        }
  
    
    else if (num == SIGTSTP) {  // Handle Ctrl+Z
        printf("\n");
        if(child == 0 || flg1==1){
        
        printf("Minishell$: ");
        fflush(stdout);
        flg1=0;
        }
        else if(child > 0){
         
            insert_at_last(head);
        }
}     // Ensure it appears immediately

  
}
int insert_at_last(slist *head)
{
   
    slist *new = malloc(sizeof(slist));
    if(new == NULL)
    {
        return 0;
    }
    new->id=child_2;
    strcpy(new->str,input);
    new->link=NULL;

    if(head == NULL){
        head=new;
       
    }
    else{
        slist *temp=head;
        while(temp->link != NULL)
        {
            temp=temp->link;
        }
    }

    if (head == NULL)
	{
		printf("INFO : List is empty\n");
	}
    else
    {
	    while (head)		
	    {
		    printf("[%d]+  ", head ->id);
            printf("Stopped         %s\n",head->str);
		    head = head->link;
	    }

	   
    }
    printf("Minishell$: ");
        fflush(stdout);

}