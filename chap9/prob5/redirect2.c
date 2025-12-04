#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/* 자식 프로세스의 표준 출력을 파일에 재지정한다. */
int main(int argc, char* argv[])
{
    int child, pid, fd, status;

    pid = fork();

    if (pid == 0) {   /* 자식 프로세스 */
        fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0600);
        if (fd < 0) {
            perror("open");
            return 1;
        }

        /* 파일을 표준출력(1)으로 복제 */
        dup2(fd, 1);
        close(fd);

        /* argv[2] 프로그램을 실행, 인자는 &argv[2] */
        execvp(argv[2], &argv[2]);

        /* exec 실패 시에만 실행됨 */
        fprintf(stderr, "%s: 실행 불가\n", argv[2]);
        return 1;

    } else if (pid > 0) {   /* 부모 프로세스 */
        child = wait(&status);
        printf("[%d] 자식 프로세스 %d 종료\n", getpid(), child);

    } else {
        perror("fork");
        return 1;
    }

    return 0;
}

