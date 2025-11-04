#include <stdio.h>
#include <stdlib.h>

static void exit_handler1(void),
            exit_handler2(void);

int main(void)
{
    // Register exit_handler1
    if (atexit(exit_handler1) != 0)
        perror("Could not register exit_handler1");

    // Register exit_handler2
    if (atexit(exit_handler2) != 0)
        perror("Could not register exit_handler2");

    printf("Main finished\n");
    exit(0);
}

// Exit Handler Example
static void exit_handler1(void)
{
    printf("First exit handler\n");
}

static void exit_handler2(void)
{
    printf("Second exit handler\n");
}
