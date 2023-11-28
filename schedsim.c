#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"

void findWaitingTimeRR(ProcessType plist[], int n, int quantum)
{
    int rem_bt[n]; // remaining burst time for each process

    // Create a copy of burst times for remaining burst time
    for (int i = 0; i < n; i++) {
        rem_bt[i] = plist[i].bt;
        plist[i].wt = 0;
    }

    int t = 0; // Initialize time
    int completed = 0; // Counter for completed processes

    // Traverse all processes while some are not done
    while (completed != n) {
        // Traverse each process
        for (int i = 0; i < n; i++) {
            // If the process is not done yet
            if (rem_bt[i] > 0) {
                // If remaining burst time is greater than the quantum
                if (rem_bt[i] > quantum) {
                    // Increment time by quantum
                    t += quantum;
                    // Reduce remaining burst time by quantum
                    rem_bt[i] -= quantum;
                } else {
                    // Last cycle for this process
                    t += rem_bt[i];
                    // Calculate waiting time for the completed process
                    plist[i].wt = t - plist[i].bt;
                    // This process is over
                    rem_bt[i] = 0;
                    completed++;
                }
            }
        }
    }
}

void findWaitingTimeSJF(ProcessType plist[], int n)
{
    int bt_rem[n]; // remaining burst time for each process
    int completed = 0, t = 0, min_bt = INT_MAX, shortest = 0;

    // Initialize remaining burst time and waiting time
    for (int i = 0; i < n; i++) {
        bt_rem[i] = plist[i].bt;
        plist[i].wt = 0;
    }

    // Continue until all processes are completed
    while (completed != n) {
        // Find the process with the minimum remaining burst time
        for (int i = 0; i < n; i++) {
            if (plist[i].art <= t && bt_rem[i] < min_bt && bt_rem[i] > 0) {
                min_bt = bt_rem[i];
                shortest = i;
            }
        }

        // Reduce the remaining burst time of the selected process
        bt_rem[shortest]--;

        // If a process is completed
        if (bt_rem[shortest] == 0) {
            completed++;
            min_bt = INT_MAX;

            // Calculate waiting time for the completed process
            plist[shortest].wt = t - plist[shortest].art - plist[shortest].bt;
            if (plist[shortest].wt < 0)
                plist[shortest].wt = 0;
        }

        // Move to the next time unit
        t++;
    }
}

void findWaitingTime(ProcessType plist[], int n)
{
    // waiting time for the first process is 0, or the arrival time if not
    plist[0].wt = 0 + plist[0].art;

    // calculating waiting time
    for (int i = 1; i < n; i++)
        plist[i].wt = plist[i - 1].bt + plist[i - 1].wt;
}

void findTurnAroundTime(ProcessType plist[], int n)
{
    // calculating turnaround time by adding bt[i] + wt[i]
    for (int i = 0; i < n; i++)
        plist[i].tat = plist[i].bt + plist[i].wt;
}

int my_comparer(const void *this, const void *that)
{
    ProcessType *p1 = (ProcessType *)this;
    ProcessType *p2 = (ProcessType *)that;

    // Compare based on priority
    if (p1->pri < p2->pri)
        return 1;
    else
        return -1;
}

void findavgTimeFCFS(ProcessType plist[], int n)
{
    // Function to find waiting time of all processes
    findWaitingTime(plist, n);

    // Function to find turn around time for all processes
    findTurnAroundTime(plist, n);

    // Display processes along with all details
    printf("\n*********\nFCFS\n");
}

void findavgTimeSJF(ProcessType plist[], int n)
{
    // Function to find waiting time of all processes
    findWaitingTimeSJF(plist, n);

    // Function to find turn around time for all processes
    findTurnAroundTime(plist, n);

    // Display processes along with all details
    printf("\n*********\nSJF\n");
}

void findavgTimeRR(ProcessType plist[], int n, int quantum)
{
    // Function to find waiting time of all processes
    findWaitingTimeRR(plist, n, quantum);

    // Function to find turn around time for all processes
    findTurnAroundTime(plist, n);

    // Display processes along with all details
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

void findavgTimePriority(ProcessType plist[], int n)
{
    // Sort the processes based on priority
    qsort(plist, n, sizeof(ProcessType), my_comparer);

    // Perform FCFS for the sorted processes
    findavgTimeFCFS(plist, n);

    // Display processes along with all details
    printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0;
    float awt, att;

    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n");

    // Calculate total waiting time and total turn
    // around time
    for (int i = 0; i < n; i++) {
        total_wt = total_wt + plist[i].wt;
        total_tat = total_tat + plist[i].tat;
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat);
    }

    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);

    printf("\nAverage waiting time = %.2f", awt);
    printf("\nAverage turn around time = %.2f\n", att);
}

ProcessType *initProc(char *filename, int *n)
{
    FILE *input_file = fopen(filename, "r");
    if (!input_file) {
        fprintf(stderr, "Error: Invalid filepath\n");
        fflush(stdout);
        exit(0);
    }

    ProcessType *plist = parse_file(input_file, n);

    fclose(input_file);

    return plist;
}

int main(int argc, char *argv[])
{
    int n;
    int quantum = 2;

    ProcessType *proc_list;

    if (argc < 2) {
        fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
        fflush(stdout);
        return 1;
    }

    // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);

    findavgTimeFCFS(proc_list, n);

    printMetrics(proc_list, n);

    // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);

    findavgTimeSJF(proc_list, n);

    printMetrics(proc_list, n);

    // Priority
    n = 0;
    proc_list = initProc(argv[1], &n);

    findavgTimePriority(proc_list, n);

    printMetrics(proc_list, n);

    // RR
    n = 0;
    proc_list = initProc(argv[1], &n);

    findavgTimeRR(proc_list, n, quantum);

    printMetrics(proc_list, n);

    return 0;
}