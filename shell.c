#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include <fcntl.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[] = "> ";

// Function to tokenize the command line
void tokenize(char *command_line, char *arguments[], int *arg_count) {
    *arg_count = 0;
    char *token = strtok(command_line, " \t\r\n");
    while (token != NULL) {
        arguments[(*arg_count)++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    arguments[*arg_count] = NULL;
}

// Function to execute built-in commands
int execute_builtin(char **arguments) {
    if (strcmp(arguments[0], "cd") == 0) {
        if (*arguments[1] == '\0') {
            // Change to the user's home directory
            if (chdir(getenv("HOME")) == -1) {
                perror("cd");
            }
        } else {
            if (chdir(arguments[1]) == -1) {
                perror("cd");
            }
        }
        return 1;  // Command was built-in
    } else if (strcmp(arguments[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("getcwd");
        }
        return 1;  // Command was built-in
    } else if (strcmp(arguments[0], "echo") == 0) {
        // Declare the loop variable outside of the loop to conform to older C standards
        int i;
        for (i = 1; arguments[i] != NULL; i++) {
            if (arguments[i][0] == '$') {
                char *var_name = arguments[i] + 1;  // Remove the '$' character
                char *var_value = getenv(var_name);
                if (var_value != NULL) {
                    printf("%s ", var_value);
                }
            } else {
                printf("%s ", arguments[i]);
            }
        }
        printf("\n");
        return 1;  // Command was built-in
    } else if (strcmp(arguments[0], "exit") == 0) {
        exit(0);
    }
    return 0;  // Not a built-in command
}

int main() {
    char command_line[MAX_COMMAND_LINE_LEN];
    char *arguments[MAX_COMMAND_LINE_ARGS];
    int arg_count;

    while (true) {
        // Print the shell prompt.
        printf("%s", prompt);
        fflush(stdout);

        // Read input from stdin.
        if (fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) {
            perror("fgets");
            exit(1);
        }

        // Tokenize the command line.
        tokenize(command_line, arguments, &arg_count);

        if (arg_count > 0) {
            // Check for built-in commands and execute them.
            int is_builtin = execute_builtin(arguments);

            if (!is_builtin) {
                // Not a built-in command.
                // Check for background process '&' at the end of the command.
                int background = 0;
                if (strcmp(arguments[arg_count - 1], "&") == 0) {
                    background = 1;
                    arguments[arg_count - 1] = NULL;
                }

                // Fork a child process.
                pid_t child_pid = fork();
                if (child_pid == -1) {
                    perror("fork");
                    exit(1);
                } else if (child_pid == 0) {
                    // This is the child process.
                    // Implement I/O redirection here if needed.
                    // Implement piping here if needed.
                    if (execvp(arguments[0], arguments) == -1) {
                        perror("execvp");
                        exit(1);
                    }
                } else {
                    // This is the parent process.
                    if (background) {
                        // If it's a background process, don't wait.
                        printf("Background process with PID %d\n", child_pid);
                    } else {
                        // It's a foreground process, wait for it.
                        int status;
                        if (waitpid(child_pid, &status, 0) == -1) {
                            perror("waitpid");
                            exit(1);
                        }
                    }
                }
            }
        }
    }

    return 0;
}
