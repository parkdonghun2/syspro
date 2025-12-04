#include <stdio.h>
#include <unistd.h>

int main()
{
    int pid, gid;

    printf("PARENT: PID = %d  GID = %d\n", getpid(), getpgrp());

    pid = fork();

    if (pid == 0) {   /* 자식 프로세스 */
        setpgid(0, 0);   // 자식 프로세스를 새로운 프로세스 그룹 리더로 설정
        printf("CHILD : PID = %d  GID = %d\n", getpid(), getpgrp());
    }

    return 0;
}

