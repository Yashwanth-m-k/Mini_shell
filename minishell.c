

#include"mini.h"

int main() {
    char input_string[100]={'\0'};
    char prompt[100] = "Minishell$:";
    system("clear");

    extract_external_commands(external);  // Fill external array with commands
    scan_input(prompt, input_string);     // Start scanning user input

    return 0;
}
