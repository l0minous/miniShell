#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

// Function declarations
void shell_loop();
void execute_command(char **args);
char **parse_input(char *input);
int handle_builtin(char **args);

int main() {
    shell_loop();
    return 0;
}

void shell_loop() {
    char input[MAX_INPUT];
    char **args;
    int status = 1;

    while (status) {
        printf("mini-shell> ");
        if (!fgets(input, MAX_INPUT, stdin)) {
            break;
        }
        // Remove trailing newline
        input[strcspn(input, "\n")] = 0;
        args = parse_input(input);
        if (args[0] == NULL) {
            free(args);
            continue;
        }
        status = handle_builtin(args);
        if (status == 1) {
            execute_command(args);
        }
        free(args);
    }
}

// Parsing input
char **parse_input(char *input) {
    char **args = malloc(MAX_ARGS * sizeof(char*));
    char *token;
    int i = 0;
    token = strtok(input, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    return args;
}

// built-in commands like cd and exit
int handle_builtin(char **args) {
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "mini-shell: expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("mini-shell");
            }
        }
        return 0;
    }
    return 1;
}

// Executing external commands
void execute_command(char **args) {
    pid_t pid, wpid;
    int status;
    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("mini-shell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("mini-shell");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
} 