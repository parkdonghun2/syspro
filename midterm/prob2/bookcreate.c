// bookcreate.c 파일 내용

#include <stdio.h>
#include <stdlib.h>
#include "book.h" 

/* 입력받은 책 정보를 파일에 바이너리 레코드 형태로 저장한다. */
int main(int argc, char *argv[])
{
    struct book record;
    FILE *fp;

    // 인자 개수 확인 (생략)
    if (argc != 2) {
        fprintf(stderr, "How to use: %s <data_file.dat>\n", argv[0]);
        exit(1);
    }

    // 파일 열기 (쓰기 전용, 바이너리 모드: "wb") (생략)
    if ((fp = fopen(argv[1], "wb")) == NULL) {
        perror("Error opening file for write");
        exit(2);
    }

    // 헤더 출력 (입력 가이드)
    printf("Enter BookID BookName Author Year NumBorrow Status(0=False/1=True):\n");
    printf("e.g., 1 Lustor Raven 2020 7 1\n");
    
    // while (scanf())를 이용해 입력받고, fwrite()로 바이너리 저장
    // borrow_status를 정수 (%d)로 받도록 복원
    while (scanf("%d %s %s %d %d %d", 
                 &record.id, record.bookname, record.author, 
                 &record.year, &record.numofborrow, &record.borrow_status) == 6) 
    {
        // fwrite()를 사용하여 레코드 구조체 전체를 파일에 쓴다.
        if (fwrite(&record, sizeof(record), 1, fp) != 1) {
            fprintf(stderr, "Error writing record to file.\n");
            break; 
        }
    }

    fclose(fp);
    printf("Book records successfully saved to %s.\n", argv[1]);
    exit(0);
}
