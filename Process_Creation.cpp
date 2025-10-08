/* 
    Process_Creation.cpp - Demonstrates process creation using fork()
    Author: Gleb Tutubalin C00290944
    Shows parent-child process relationship and concurrent execution
*/

#include <iostream>      // C++ I/O streams
#include <unistd.h>      // POSIX API for fork, getpid
#include <sys/types.h>   // Data types for system calls
#include <sys/wait.h>    // Wait functions for process sync
#include <cstdlib>       // C++ standard library

using namespace std;     // Use standard namespace

int main() {
    pid_t pid;              // Variable to store fork return
    int shared_value = 10;  // Variable to test memory independence
    
    cout << "Process Creation Demonstration" << endl;
    cout << "==============================" << endl;
    cout << "Parent process starting (PID: " << getpid() << ")" << endl << endl;
    
    // Create child process using fork()
    pid = fork();
    
    // Check for fork() failure
    if (pid < 0) {
        cerr << "Fork failed" << endl;
        return 1;
    }
    
    // Child process code block (fork returns 0)
    if (pid == 0) {
        cout << "CHILD PROCESS:" << endl;
        cout << "  Child PID: " << getpid() << endl;
        cout << "  Parent PID: " << getppid() << endl;
        cout << "  Initial shared_value: " << shared_value << endl;
        
        // Modify variable in child process
        shared_value = 25;
        cout << "  Child modified shared_value to: " << shared_value << endl;
        
        // Child performs some work
        cout << "  Child executing task..." << endl;
        sleep(2); // Simulate work
        
        cout << "  Child process terminating" << endl;
        exit(0); // Child exits with status 0
    }
    
    // Parent process code block (fork returns child PID)
    else {
        cout << "PARENT PROCESS:" << endl;
        cout << "  Parent PID: " << getpid() << endl;
        cout << "  Created child with PID: " << pid << endl;
        cout << "  Initial shared_value: " << shared_value << endl;
        
        // Modify variable in parent process
        shared_value = 50;
        cout << "  Parent modified shared_value to: " << shared_value << endl;
        
        // Parent waits for child to complete
        int status;
        pid_t child_pid = wait(&status);
        
        // Check child termination status
        if (WIFEXITED(status)) {
            cout << endl << "  Child process " << child_pid 
                 << " terminated normally" << endl;
            cout << "  Exit status: " << WEXITSTATUS(status) << endl;
        }
        
        cout << "  Parent's final shared_value: " << shared_value << endl;
        cout << endl << "Parent process terminating" << endl;
    }
    
    return 0;
}