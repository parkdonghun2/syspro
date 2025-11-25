#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("[Parent Process Starts]\n");

    if (fork() == 0) {
        execl("/bin/echo", "echo", "hello", NULL);
        fprintf(stderr, "First failure\n");
        exit(1);
    }

    if (fork() == 0) {
        execl("/bin/date", "date", NULL);
        fprintf(stderr, "Second failure\n");
        exit(2);
    }
    
    if (fork() == 0) {
        execl("/bin/ls", "ls", "-l", NULL);
        fprintf(stderr, "Third failure\n");
        exit(3);
    }

    printf("[Parent Process Ends]\n");
    
    return 0; 
}
