#include <stdio.h>
#include <unistd.h> // Required for getpid() and getppid()

/* Outputs the process number. */
int main()
{
    printf("My process ID : [%d]\n", getpid());
    printf("My parent process ID : [%d]\n", getppid());
    
    return 0; // Explicit return
}
