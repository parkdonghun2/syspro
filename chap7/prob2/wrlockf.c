#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

/* 잠금 함수를 이용한 학생 데이터베이스 수정 프로그램 */
int main(int argc, char *argv[])
{
    int fd, id;
    struct student record;

    if (argc < 2) {
        fprintf(stderr, "How to use : %s file\n", argv[0]);
        exit(1);
    }

    /* 파일 열기 (읽기/쓰기 모드) */
    if ((fd = open(argv[1], O_RDWR)) == -1) {
        perror(argv[1]);
        exit(2);
    }

    printf("\nEnter StudentID you want to modify : ");

    while (scanf("%d", &id) == 1) {

        /* 레코드 시작 위치로 이동 */
        lseek(fd, (long)(id - START_ID) * sizeof(record), SEEK_SET);

        /* 쓰기 잠금 */
        if (lockf(fd, F_LOCK, sizeof(record)) == -1) {
            perror(argv[1]);
            exit(3);
        }

        /* 레코드 읽기 */
        if ((read(fd, (char *)&record, sizeof(record)) > 0) && (record.id != 0)) {

            printf("Name:%s\tStuID:%d\tScore:%d\n",
                   record.name, record.id, record.score);

            printf("Enter new score : ");
            scanf("%d", &record.score);

            /* 수정된 점수를 다시 쓰기 */
            lseek(fd, (long)-sizeof(record), SEEK_CUR);
            write(fd, (char *)&record, sizeof(record));

            /* 잠금 해제 */
            lseek(fd, (long)(id - START_ID) * sizeof(record), SEEK_SET);
            lockf(fd, F_ULOCK, sizeof(record));
        }
        else {
            printf("No record %d\n", id);
        }

        printf("\nEnter StudentID you want to modify : ");
    }

    close(fd);
    exit(0);
}

