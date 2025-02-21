

#include"mini.h"


int main() {
      // Register signal handler
    char input_string[100];
    char prompt[100] = "Minishell$:";
    
    system("clear");
    signal(SIGINT, handler);   // Handle Ctrl+C
     signal(SIGTSTP, handler);  // Handle Ctrl+Z
    

     
    extract_external_commands(external);  // Fill external array with commands
    scan_input(prompt, input_string);     // Start scanning user input

    return 0;
}
