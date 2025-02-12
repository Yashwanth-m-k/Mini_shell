
#include"mini.h"


char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};

char  external[200][10];

void extract_external_commands(char external[200][10]) {
    int fd = open("command.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    int ret, i = 0, j = 0;
    char buffer[20];

    while ((ret = read(fd, &buffer[i], 1)) > 0) {

        if (buffer[i] == '\n') {
            buffer[i] = '\0';  // Null-terminate the string

           
        strcpy( external[j],buffer);
            j++;

            if (j >= 200) {
                printf("Warning: Maximum command storage limit reached.\n");
                break;
            }

            i = 0;  
           
        }
        else
        i++; 
           
    }
    
    close(fd);  
}

    
int main(){
    char input_string[100];
    char prompt[100] = "Minishell$:";
    system("clear");
    extract_external_commands(external);
     scan_input(prompt,input_string);
    return 0;
}