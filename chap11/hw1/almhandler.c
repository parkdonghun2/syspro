#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef void (*sighandler_t)(int);

sighandler_t my_signal(int signo, sighandler_t handler)
{
    struct sigaction act, oldact;

    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (sigaction(signo, &act, &oldact) < 0) {
        return SIG_ERR;
    }

    return oldact.sa_handler;
}

void alarmHandler(int signo)
{
    printf("Wake up\n");
    exit(0);
}

int main(void)
{
    short i = 0;

    if (my_signal(SIGALRM, alarmHandler) == SIG_ERR) {
        perror("my_signal");
        exit(1);
    }

    alarm(5);

    while (1) {
        sleep(1);
        i++;
        printf("%d second\n", i);
    }

    printf("end\n");
    return 0;
}

