#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/* 표준출력을 파일로 재지정하는 프로그램 */
int main(int argc, char* argv[])
{
    int fd;

    if (argc < 2) {
        fprintf(stderr, "사용법: %s <파일명>\n", argv[0]);
        return 1;
    }

    /* 파일 열기: 없으면 생성, 있으면 내용 비우고 쓰기 전용 */
    fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0600);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    /* fd를 표준출력(1번)으로 복제하여 stdout → 파일로 향함 */
    dup2(fd, 1);
    close(fd);

    /* 출력 */
    printf("Hello stdout !\n");
    fprintf(stderr, "Hello stderr !\n");

    return 0;
}

