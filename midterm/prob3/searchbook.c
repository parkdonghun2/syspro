// searchbook.c 파일 내용

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "book.h" 

/* 책 이름을 입력받아 해당 레코드를 검색하여 출력한다. */
int main(int argc, char *argv[])
{
    FILE *fp;
    struct book record;
    char search_name[MAX_TITLE];
    int found = 0;

    // 인자 개수 확인: ./searchbook <검색 이름> <데이터 파일> (총 3개)
    if (argc != 3) {
        fprintf(stderr, "How to use: %s <book_name> <data_file.dat>\n", argv[0]);
        exit(1);
    }
    
    // 1. 검색할 이름과 파일명 추출
    strcpy(search_name, argv[1]); // 검색할 책 이름
    
    // 2. 파일 열기 (읽기 전용: "rb")
    if ((fp = fopen(argv[2], "rb")) == NULL) { 
        perror(argv[2]);
        exit(2);
    }

    // 3. 헤더 출력
    printf("\nSearch Results for: %s\n", search_name);
    printf("%-5s %-20s %-15s %-6s %-12s %-6s\n", 
           "id", "bookname", "author", "year", "numofborrow", "borrow");
    printf("----------------------------------------------------------------------\n");

    // 4. 순차 검색 루프
    while (fread(&record, RECORD_SIZE, 1, fp) > 0) {
        
        // 유효 레코드인지 확인 및 책 이름 비교 (대소문자 구분 문제의 조건)
        if (record.id != 0 && strcmp(record.bookname, search_name) == 0) {
            
            // 일치하는 책 정보 출력
            int is_available = (record.borrow_status == 0);
            printf("%-5d %-20s %-15s %-6d %-12d %-6s\n", 
                   record.id, record.bookname, record.author, record.year, record.numofborrow, 
                   is_available ? "False" : "True");
            found = 1;
            // 계속 찾기 위해 break하지 않습니다.
        }
    }
    
    // 5. 검색 결과 예외 처리 (책이 없을 경우)
    if (!found) {
        fprintf(stderr, "not found book name : '%s'\n", search_name);
    }

    fclose(fp);
    printf("----------------------------------------------------------------------\n");
    return 0;
}
