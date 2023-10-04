#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

// Function to generate a random number between 1 and 10
int getRandomSleepTime() {
    return (rand() % 10) + 1;
}

int main() {
    srand(time(NULL)); // Seed the random number generator

    // Create two child processes
    int i;
    for (i = 0; i < 2; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            // Child process
            int iterations = (rand() % 30) + 1;
            int j;
            for (j = 0; j < iterations; j++) {
                printf("Child Pid: %d is going to sleep!\n", getpid());
                int sleepTime = getRandomSleepTime();
                sleep(sleepTime);
                printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", getpid(), getppid());
            }

            exit(0);
        }
    }

    // Parent process
    int status;
    for (i = 0; i < 2; i++) {
        pid_t child_pid = wait(&status);
        if (WIFEXITED(status)) {
            printf("Child Pid: %d has completed\n", child_pid);
        }
    }

    return 0;
}
