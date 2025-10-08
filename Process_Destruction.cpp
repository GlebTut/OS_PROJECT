/* 
    process_destruction.cpp - Demonstrates process termination mechanisms
    Author: Gleb Tutubalin C00290944
    Shows normal termination, signals, and zombie process handling
*/

#include <stdio.h>       // Standard I/O functions
#include <stdlib.h>      // Standard library and exit()
#include <unistd.h>      // POSIX API access
#include <sys/types.h>   // Data types for system calls
#include <sys/wait.h>    // Wait functions
#include <signal.h>      // Signal handling functions
#include <errno.h>       // Error definitions

// Signal handler for graceful termination
void signal_handler(int signum) {
    printf("\n[Process %d] Received signal %d (SIGTERM)\n", getpid(), signum);
    printf("[Process %d] Performing cleanup before termination...\n", getpid());
    // Cleanup operations would go here
    printf("[Process %d] Cleanup complete, exiting gracefully\n", getpid());
    exit(0);
}

int main() {             // Main function entry point
    pid_t pid1, pid2, pid3;
    
    printf("Process Destruction Demonstration\n");
    printf("==================================\n");
    printf("Parent PID: %d\n\n", getpid());
    
    // Create child 1: Normal termination with exit code 0
    pid1 = fork();
    if (pid1 == 0) {
        printf("CHILD 1 (PID %d): Normal termination example\n", getpid());
        printf("  Performing work...\n");
        sleep(1);
        printf("  Work complete, exiting with status 0\n");
        exit(0); // Successful termination
    }
    
    // Create child 2: Termination with error code
    pid2 = fork();
    if (pid2 == 0) {
        printf("CHILD 2 (PID %d): Error termination example\n", getpid());
        printf("  Simulating error condition...\n");
        sleep(1);
        printf("  Error occurred, exiting with status 1\n");
        exit(1); // Error termination
    }
    
    // Create child 3: Signal-based termination
    pid3 = fork();
    if (pid3 == 0) {
        // Register signal handler for SIGTERM
        signal(SIGTERM, signal_handler);
        
        printf("CHILD 3 (PID %d): Signal termination example\n", getpid());
        printf("  Waiting for termination signal...\n");
        
        // Infinite loop until signal received
        while(1) {
            sleep(1);
        }
        exit(0);
    }
    
    // Parent waits briefly then signals child 3
    sleep(2);
    printf("\nPARENT: Sending SIGTERM to Child 3 (PID %d)\n", pid3);
    kill(pid3, SIGTERM); // Send termination signal
    
    // Wait for all children and collect exit status
    printf("\nPARENT: Collecting child exit statuses...\n");
    
    int status;
    pid_t finished_pid;
    
    // Wait for first child
    finished_pid = wait(&status);
    if (WIFEXITED(status)) {
        printf("  Child %d terminated normally with exit status: %d\n",
               finished_pid, WEXITSTATUS(status));
    }
    
    // Wait for second child
    finished_pid = wait(&status);
    if (WIFEXITED(status)) {
        printf("  Child %d terminated normally with exit status: %d\n",
               finished_pid, WEXITSTATUS(status));
    }
    
    // Wait for third child
    finished_pid = wait(&status);
    if (WIFEXITED(status)) {
        printf("  Child %d terminated normally with exit status: %d\n",
               finished_pid, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("  Child %d terminated by signal: %d\n",
               finished_pid, WTERMSIG(status));
    }
    
    // Demonstrate zombie prevention
    printf("\nPARENT: All children properly cleaned up (no zombies)\n");
    printf("PARENT: Process destruction demonstration complete\n");
    
    // Display key concepts
    printf("\nKey Concepts Demonstrated:\n");
    printf("- Normal termination with exit codes (0 = success, non-zero = error)\n");
    printf("- Signal-based termination using kill() and SIGTERM\n");
    printf("- Proper zombie prevention using wait()\n");
    printf("- Parent collection of child exit statuses\n");
    
    return 0;
}