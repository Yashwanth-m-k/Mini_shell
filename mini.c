#include "mini.h"  
int static flg1=1;
char external[200][50];

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help","jobs","fg", NULL};
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

void scan_input(char *prompt, char *input_string)
 {
      while (1) {
      
        printf("%s", prompt);
        if (!fgets(input_string, 100, stdin)) continue;
        input_string[strcspn(input_string, "\n")] = 0;  // Remove newline

    strcpy(input,input_string);
    
        int len=strlen(input_string);
        if(len == 0)
        {
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
    child_2=getpid();
  
    if(child == 0){
        //child    
      
        signal(SIGINT,SIG_DFL);
        signal(SIGTSTP,SIG_DFL);
        
      execute_exteranl_command(input_string);
     
        
    }
    else{
        //parent
       
        
        flg1=0;
        int status=0;
        waitpid(child, &status, WUNTRACED);  // Ensure the shell properly waits
        child_1= WEXITSTATUS(status);
            child=0;
            child_2 = 0;
        
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

    char *commend[100];  // Array of string pointers
    int i = 0, flag = 0;
    int pipe_count = 0, pos = 0;
    
    // Tokenizing input_string into words
    char *token = strtok(input_string, " ");
    while (token != NULL) {
        
        if (strcmp(token, "|") == 0) {  // Fixing pipe comparison
            flag = 1;
            pipe_count++;
            commend[i++] = NULL;  // Separate commands properly
            pos = i;  // Store position of next command
        } else {
            commend[i++] = token;  // Store command/token
        }
    
        token = strtok(NULL, " ");  // Get next token
    }
    
    commend[i] = NULL;  // NULL-terminate for execvp()
  

   if(flag==1){
   
     pipe_command_execution(commend,&pipe_count);
     exit(0);
   }
   else{
    execvp(commend[0], commend);  // Execute command
    perror("execvp");  // If execvp fails
   }
  
}
void execute_internal_commend(char *input_string,char *str)
{
    
    
    if (strcmp(input_string, "exit") == 0) {
        exit(0);
        
    }
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
        printf("%d\n", child_1);
        return;
    }
    if (strcmp(input_string, "echo $$") == 0) {
        printf("%d\n", child_2);
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
 if(strcmp(input_string,"jobs")==0){
    print_list(head);
 }
if(strcmp(input_string,"fg")==0)
{
    fg_command_execution(&head);
}

}
   
 void handler(int num) 
 {
    if (num == SIGINT) {  // Ctrl+C
        if (child_2 > 0) {  
            kill(child_2, SIGINT);  // Kill foreground process
            child_2 = 0;
           
        } 
        printf("\nMinishell$: ");  // Always display prompt
        fflush(stdout);
    } 
else if (num == SIGTSTP) {  
    
    if (child_2 > 0) {  // Only add jobs when process exists
     printf("\n");
    insert_at_last(&head);
   kill(child_2, SIGTSTP);  // Stop the foreground process properly
      child_2 = 0;  // Reset child tracking
                        
  }       
if(flg1){
    printf("\n");
      printf("Minishell$: ");
        fflush(stdout);
       }
                
    }
 }

  
int insert_at_last(slist **head) {
 static int  i=1;
    slist *new = malloc(sizeof(slist));
    if (!new) {
        printf("Memory allocation failed!\n");
        return 0;  // Return failure
    }

    new->id = child_2;
    strcpy(new->str, input);
    new->link = NULL;

    if (*head == NULL) {
        *head = new;  // Update the head pointer
    } else {
        slist *temp = *head;
        while (temp->link != NULL) {
            temp = temp->link;
         
        }
        temp->link = new;
       
    }
    printf("[%d]+    Stopped         %s\n",i++,new->str);
    return 1;  // Success
}


void print_list(slist *head)
 { 
    
    if (head == NULL) {
        return;
    }

    int i = 1;
    slist *temp = head;  // Use a temp pointer to avoid modifying head
    while (temp) {
        printf("[%d]+  Stopped         %s\n", i++, temp->str);
        temp = temp->link;
    }
}
void fg_command_execution(slist **head)
{
    if(*head == NULL)
    {
        printf("fg: current: no such jobs\n");
        return;
    }
    char str[100];
    pid_t pid;
    slist *temp = *head;
    slist *prev=NULL;
    while (temp->link != NULL) {
        prev=temp;
        temp = temp->link;
    }
   pid=temp->id;
   strcpy(str,temp->str);
   // Remove job from list
            if (prev) {
                prev->link = NULL;
            } else {
                *head = NULL;  // If only one job was in the list
            }

            free(temp);  // Free memory
    printf("%s\n", str);
    kill(pid, SIGCONT);  // Resume process

    int status;
    waitpid(pid, &status, WUNTRACED);  // Wait for it to finish
}
void pipe_command_execution(char **command,int *pipe_count)  
{
  

    int pipes[*pipe_count][2];

    // Creating pipes
    for (int j = 0; j < *pipe_count; j++) {
        if (pipe(pipes[j]) == -1) {
            perror("Pipe failed");
            exit(1);
        }
    }

    // Executing commands
    int cmd_start = 0;
    for (int j = 0; j <= *pipe_count; j++) {
        pid_t pid = fork();
        
        if (pid == 0) {  // Child process

            // If not the first command, read from previous pipe
            if (j > 0) {
                dup2(pipes[j - 1][0], STDIN_FILENO);
            }
            // If not the last command, write to current pipe
            if (j < *pipe_count) {
                dup2(pipes[j][1], STDOUT_FILENO);
            }

            // Close all pipes in child
            for (int k = 0; k < *pipe_count; k++) {
                close(pipes[k][0]);
                close(pipes[k][1]);
            }

            // Execute command
            execvp(command[cmd_start], &command[cmd_start]);
            perror("Exec failed");
            exit(1);
        }

        // Parent process: Move to the next command
        if (j > 0) {
            close(pipes[j - 1][0]);  // Close previous read end
        }
        if (j < *pipe_count) {
            close(pipes[j][1]);  // Close current write end
        }

        // Move to the next command
        while (command[cmd_start] != NULL) {
            cmd_start++;
        }
        cmd_start++;  // Move past NULL separator
    }

    // Wait for all child processes
    for (int j = 0; j <= *pipe_count; j++) {
        
        wait(NULL);
    }
      
    return;
} 