#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

int alarmCount = 0;
time_t startTime;

void handler(int signum) {
    printf("Hello World!\n");
    alarm(1); // Schedule another SIGALRM for 1 second
}

void ctrlCHandler(int signum) {
    time_t endTime = time(NULL);
    double executionTime = difftime(endTime, startTime);
    printf("\nTotal alarms: %d\nExecution time: %.2lf seconds\n", alarmCount, executionTime);
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGALRM, handler); // Register handler to handle SIGALRM
    signal(SIGINT, ctrlCHandler); // Register handler to handle SIGINT (Ctrl+C)

    alarm(1); // Schedule the first SIGALRM for 1 second
    startTime = time(NULL);

    while (1) {
        pause(); // Wait for a signal to be delivered
        alarmCount++;
        printf("Turing was right!\n");
    }

    return 0; // Never reached
}