// bookquery.c 파일 내용

#include <stdio.h>
#include <stdlib.h>
#include "book.h" // struct book, RECORD_SIZE 사용
// book.h에는 MAX_TITLE 20, MAX_AUTHOR 20 등이 정의되어 있습니다.

/* 저장된 책 레코드를 읽어 옵션에 따라 출력한다. */
int main(int argc, char *argv[])
{
    struct book record;
    FILE *fp;
    int option;

    // 1. 인자 개수 확인: 총 3개 필요 (./bookquery, option, data_file)
    if (argc != 3) {
        fprintf(stderr, "How to use: %s <option: 0/1> <data_file.dat>\n", argv[0]);
        exit(1);
    }
    
    // 2. 옵션 값(argv[1])을 정수형으로 변환 및 검증
    option = atoi(argv[1]); 
    if (option != 0 && option != 1) {
        fprintf(stderr, "Error: Invalid option %d. Use 0 or 1.\n", option);
        exit(1);
    }

    // 3. 파일 열기 (읽기 전용, 바이너리 모드: "rb")
    if ((fp = fopen(argv[2], "rb")) == NULL) { 
        perror(argv[2]);
        exit(2);
    }

    // 4. 헤더 출력 (필드 폭을 넓게 지정하여 정렬 문제 해결)
    printf("\nBook Query Result (Option %d):\n", option);
    printf("%-5s %-20s %-15s %-6s %-12s %-8s\n", 
           "ID", "Bookname", "Author", "Year", "Borrowed", "Status");
    printf("----------------------------------------------------------------------\n");
    
    // 5. 레코드 읽기 루프
    while (fread(&record, RECORD_SIZE, 1, fp) > 0) {
        
        // 유효한 레코드인지 확인
        if (record.id != 0) { 
            
            // borrow_status가 0이면 False(대출 가능), 1이면 True(대출 불가능)
            int is_available = (record.borrow_status == 0); 
            const char *status_str = is_available ? "False" : "True";
            
            // 옵션 0: 모든 레코드 출력
            if (option == 0) {
                // 출력 (헤더와 동일한 폭으로 확보)
                printf("%-5d %-20s %-15s %-6d %-12d %-8s\n", 
                       record.id, record.bookname, record.author, record.year, record.numofborrow, 
                       status_str);

            // 옵션 1: 대출 가능한 책(borrow_status=0)만 출력
            } else if (option == 1 && is_available) {
                // 출력 (상태는 항상 False)
                printf("%-5d %-20s %-15s %-6d %-12d %-8s\n", 
                       record.id, record.bookname, record.author, record.year, record.numofborrow, 
                       "False"); 
            }
        }
    }

    // 6. 파일 닫기
    fclose(fp);
    printf("----------------------------------------------------------------------\n");

    return 0; 
}
