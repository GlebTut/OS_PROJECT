/* 
    Process_Scheduling.cpp - Demonstrates process scheduling and priorities
    Author: Gleb Tutubalin C00290944
    Shows how nice values affect process scheduling and CPU allocation
*/

#include <iostream>         // C++ I/O streams
#include <unistd.h>         // POSIX API and nice()
#include <sys/types.h>      // Data types for system calls
#include <sys/wait.h>       // Wait functions
#include <sys/resource.h>   // Resource limit functions
#include <cerrno>           // Error number definitions
#include <ctime>            // Time functions for timing

using namespace std;        // Use standard namespace

// Function to perform CPU-intensive work
void cpu_work(const string& process_name, int iterations) {
    long long counter = 0;
    time_t start_time = time(nullptr);
    
    cout << "[" << process_name << "] Starting CPU work with " 
         << iterations << " iterations" << endl;
    
    // Simulate CPU-bound task
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 100000; j++) {
            counter += j;
        }
    }
    
    time_t end_time = time(nullptr);
    cout << "[" << process_name << "] Completed CPU work. Counter value: " 
         << counter << endl;
    cout << "[" << process_name << "] Execution time: " 
         << (end_time - start_time) << " seconds" << endl;
}

int main() {
    pid_t pid1, pid2, pid3;
    
    cout << "Process Scheduling Demonstration" << endl;
    cout << "=================================" << endl;
    cout << "Parent PID: " << getpid() << endl;
    cout << "Creating three child processes with different priorities" << endl << endl;
    
    // Create first child process (high priority)
    pid1 = fork();
    if (pid1 == 0) {
        // Child 1: High priority (nice value -10)
        int current_nice = nice(-10); // Set high priority
        if (current_nice == -1 && errno != 0) {
            cerr << "Failed to set nice value (may need root privileges)" << endl;
            current_nice = nice(0); // Use default if fails
        }
        
        cout << "CHILD 1 (High Priority):" << endl;
        cout << "  PID: " << getpid() << ", Nice value: " << current_nice << endl;
        cpu_work("Child 1", 50); // Perform work
        exit(0);
    }
    
    // Create second child process (normal priority)
    pid2 = fork();
    if (pid2 == 0) {
        // Child 2: Normal priority (nice value 0)
        int current_nice = nice(0); // Default priority
        
        cout << "CHILD 2 (Normal Priority):" << endl;
        cout << "  PID: " << getpid() << ", Nice value: " << current_nice << endl;
        cpu_work("Child 2", 50); // Perform work
        exit(0);
    }
    
    // Create third child process (low priority)
    pid3 = fork();
    if (pid3 == 0) {
        // Child 3: Low priority (nice value +10)
        int current_nice = nice(10); // Set low priority
        
        cout << "CHILD 3 (Low Priority):" << endl;
        cout << "  PID: " << getpid() << ", Nice value: " << current_nice << endl;
        cpu_work("Child 3", 50); // Perform work
        exit(0);
    }
    
    // Parent process monitors children
    cout << endl << "PARENT: All children created, waiting for completion..." 
         << endl << endl;
    
    // Wait for all children to complete
    int status;
    pid_t finished_pid;
    int child_count = 0;
    
    while (child_count < 3) {
        finished_pid = wait(&status); // Wait for any child
        if (finished_pid > 0) {
            child_count++;
            cout << "PARENT: Child process " << finished_pid 
                 << " finished (" << child_count << "/3)" << endl;
        }
    }
    
    cout << endl << "PARENT: All children completed. Scheduling demonstration finished." 
         << endl;
    
    // Display scheduling information
    cout << endl << "Scheduling Notes:" << endl;
    cout << "- Child 1 (nice -10) had highest priority" << endl;
    cout << "- Child 2 (nice 0) had normal priority" << endl;
    cout << "- Child 3 (nice +10) had lowest priority" << endl;
    cout << "- The scheduler allocated CPU time based on these priorities" << endl;
    
    return 0;
}
