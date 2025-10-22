/* 
    Process_Destruction.cpp - Demonstrates process termination mechanisms
    Author: Gleb Tutubalin C00290944
    Shows normal termination, signals, and zombie process handling
*/

#include <iostream>      // C++ I/O streams
#include <unistd.h>      // POSIX API access
#include <sys/types.h>   // Data types for system calls
#include <sys/wait.h>    // Wait functions
#include <csignal>       // Signal handling functions
#include <cerrno>        // Error definitions
#include <cstdlib>       // C++ standard library

using namespace std;     // Use standard namespace

// Signal handler for graceful termination
void signal_handler(int signum) {
    cout << endl << "[Process " << getpid() << "] Received signal " 
         << signum << " (SIGTERM)" << endl;
    cout << "[Process " << getpid() << "] Performing cleanup before termination..." 
         << endl;
    // Cleanup operations would go here
    cout << "[Process " << getpid() << "] Cleanup complete, exiting gracefully" 
         << endl;
    exit(0);
}

int main() {
    pid_t pid1, pid2, pid3;
    
    cout << "Process Destruction Demonstration" << endl;
    cout << "==================================" << endl;
    cout << "Parent PID: " << getpid() << endl << endl;
    
    // Create child 1: Normal termination with exit code 0
    pid1 = fork();
    if (pid1 == 0) {
        cout << "CHILD 1 (PID " << getpid() << "): Normal termination example" << endl;
        cout << "  Performing work..." << endl;
        sleep(1);
        cout << "  Work complete, exiting with status 0" << endl;
        exit(0); // Successful termination
    }
    
    // Create child 2: Termination with error code
    pid2 = fork();
    if (pid2 == 0) {
        cout << "CHILD 2 (PID " << getpid() << "): Error termination example" << endl;
        cout << "  Simulating error condition..." << endl;
        sleep(1);
        cout << "  Error occurred, exiting with status 1" << endl;
        exit(1); // Error termination
    }
    
    // Create child 3: Signal-based termination
    pid3 = fork();
    if (pid3 == 0) {
        // Register signal handler for SIGTERM
        signal(SIGTERM, signal_handler);
        
        cout << "CHILD 3 (PID " << getpid() << "): Signal termination example" << endl;
        cout << "  Waiting for termination signal..." << endl;
        
        // Infinite loop until signal received
        while(1) {
            sleep(1);
        }
        exit(0);
    }
    
    // Parent waits briefly then signals child 3
    sleep(2);
    cout << endl << "PARENT: Sending SIGTERM to Child 3 (PID " << pid3 << ")" << endl;
    kill(pid3, SIGTERM); // Send termination signal
    
    // Wait for all children and collect exit status
    cout << endl << "PARENT: Collecting child exit statuses..." << endl;
    
    int status;
    pid_t finished_pid;
    
    // Wait for first child
    finished_pid = wait(&status);
    if (WIFEXITED(status)) {
        cout << "  Child " << finished_pid 
             << " terminated normally with exit status: " 
             << WEXITSTATUS(status) << endl;
    }
    
    // Wait for second child
    finished_pid = wait(&status);
    if (WIFEXITED(status)) {
        cout << "  Child " << finished_pid 
             << " terminated normally with exit status: " 
             << WEXITSTATUS(status) << endl;
    }
    
    // Wait for third child
    finished_pid = wait(&status);
    if (WIFEXITED(status)) {
        cout << "  Child " << finished_pid 
             << " terminated normally with exit status: " 
             << WEXITSTATUS(status) << endl;
    } else if (WIFSIGNALED(status)) {
        cout << "  Child " << finished_pid 
             << " terminated by signal: " 
             << WTERMSIG(status) << endl;
    }
    
    // Demonstrate zombie prevention
    cout << endl << "PARENT: All children properly cleaned up (no zombies)" << endl;
    cout << "PARENT: Process destruction demonstration complete" << endl;
    
    // Display key concepts
    cout << endl << "Key Concepts Demonstrated:" << endl;
    cout << "- Normal termination with exit codes (0 = success, non-zero = error)" << endl;
    cout << "- Signal-based termination using kill() and SIGTERM" << endl;
    cout << "- Proper zombie prevention using wait()" << endl;
    cout << "- Parent collection of child exit statuses" << endl;
    
    return 0;
}
