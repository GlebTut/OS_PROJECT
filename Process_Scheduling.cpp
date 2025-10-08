/* 
    process_scheduling.cpp - Demonstrates process scheduling and priorities
    Author: Gleb Tutubalin C00290944
    Shows how nice values affect process scheduling and CPU allocation
*/

#include <stdio.h>          // Standard I/O functions
#include <stdlib.h>         // Standard library functions
#include <unistd.h>         // POSIX API and nice()
#include <sys/types.h>      // Data types for system calls
#include <sys/wait.h>       // Wait functions
#include <sys/resource.h>   // Resource limit functions
#include <errno.h>          // Error number definitions

// Function to perform CPU-intensive work
void cpu_work(const char* process_name, int iterations) {
    long long counter = 0;
    printf("[%s] Starting CPU work with %d iterations\n", 
           process_name, iterations);
    
    // Simulate CPU-bound task
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 100000; j++) {
            counter += j;
        }
    }
    
    printf("[%s] Completed CPU work. Counter value: %lld\n", 
           process_name, counter);
}

int main() {             // Main function entry point
    pid_t pid1, pid2, pid3;
    
    printf("Process Scheduling Demonstration\n");
    printf("=================================\n");
    printf("Parent PID: %d\n", getpid());
    printf("Creating three child processes with different priorities\n\n");
    
    // Create first child process (high priority)
    pid1 = fork();
    if (pid1 == 0) {
        // Child 1: High priority (nice value -10)
        int current_nice = nice(-10); // Set high priority
        if (current_nice == -1 && errno != 0) {
            perror("Failed to set nice value (may need root privileges)");
            current_nice = nice(0); // Use default if fails
        }
        
        printf("CHILD 1 (High Priority):\n");
        printf("  PID: %d, Nice value: %d\n", getpid(), current_nice);
        cpu_work("Child 1", 50); // Perform work
        exit(0);
    }
    
    // Create second child process (normal priority)
    pid2 = fork();
    if (pid2 == 0) {
        // Child 2: Normal priority (nice value 0)
        int current_nice = nice(0); // Default priority
        
        printf("CHILD 2 (Normal Priority):\n");
        printf("  PID: %d, Nice value: %d\n", getpid(), current_nice);
        cpu_work("Child 2", 50); // Perform work
        exit(0);
    }
    
    // Create third child process (low priority)
    pid3 = fork();
    if (pid3 == 0) {
        // Child 3: Low priority (nice value +10)
        int current_nice = nice(10); // Set low priority
        
        printf("CHILD 3 (Low Priority):\n");
        printf("  PID: %d, Nice value: %d\n", getpid(), current_nice);
        cpu_work("Child 3", 50); // Perform work
        exit(0);
    }
    
    // Parent process monitors children
    printf("\nPARENT: All children created, waiting for completion...\n\n");
    
    // Wait for all children to complete
    int status;
    pid_t finished_pid;
    int child_count = 0;
    
    while (child_count < 3) {
        finished_pid = wait(&status); // Wait for any child
        if (finished_pid > 0) {
            child_count++;
            printf("PARENT: Child process %d finished (%d/3)\n", 
                   finished_pid, child_count);
        }
    }
    
    printf("\nPARENT: All children completed. Scheduling demonstration finished.\n");
    
    // Display scheduling information
    printf("\nScheduling Notes:\n");
    printf("- Child 1 (nice -10) had highest priority\n");
    printf("- Child 2 (nice 0) had normal priority\n");
    printf("- Child 3 (nice +10) had lowest priority\n");
    printf("- The scheduler allocated CPU time based on these priorities\n");
    
    return 0; 
}