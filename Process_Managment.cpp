/*  
    Process_Managment.cpp - Demonstrates basic process managment concepts
    Author: Gleb Tutubalin C00290944
    Displays PID, PPID, and process information
*/
#include <iostream>     // Standard I/O streams
#include <fstream>      // File Stream Operations
#include <string>       // String class
#include <unistd.h>     // POSIX API acceess
#include <sys/types.h>  // Data types for system calls
using namespace std;

int main(){
    pid_t pid;  // Variable to store process ID
    pid_t ppid; // Variable to store parent PID

    // Retrive current process ID
    pid = getpid();
    // Retrive current parent process ID
    ppid = getppid();

    // Display process identifiers
    cout << "Process Managment Demonstration" << endl;
    printf("=================================\n");
    cout << endl;
    cout << "Current Process ID (PID): " << pid << endl;
    cout << "Parent Process ID (PPID): " << ppid << endl;

    // Display user and group IDs
    cout << "User ID (UID): " << getuid() << endl;
    cout << "Effective User ID (EUID): " << geteuid() << endl;
    cout << "Group ID (GID): " << getgid() << endl;

    // Access /proc filesystem for detailed info
    string proc_path = "/proc/" + to_string(pid) + "/status";

    // Display process status information
    cout << "\nProcess Status Information:" << endl;
    cout << "Reading from: " << proc_path << endl;

    // Open and read processes status file using C++ streams
    ifstream status_file(proc_path);
    if (status_file.is_open()) {
        string line;
        int count = 0;
        // Read first 10 lines of status
        while (getline(status_file, line) && count < 10){
            cout << line << endl;
            count++;
        }
        status_file.close();
    } else {
        cerr << "Error opening status file" << endl;
    }

    return 0;
}