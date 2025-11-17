// bookupdate.c (Final Code with Write and Print Logic)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "book.h" // struct book, START_ID, RECORD_SIZE 사용

/* Student Database Modification Program using File I/O */
int main(int argc, char *argv[])
{
    FILE *fp;
    int option, book_id;
    struct book record;

    if (argc != 2) {
        fprintf(stderr, "How to use: %s <data_file.dat>\n", argv[0]);
        exit(1);
    }
    
    // 파일 열기 (읽기/쓰기 모드: "rb+")
    if ((fp = fopen(argv[1], "rb+")) == NULL) { 
        perror(argv[1]);
        exit(2);
    }

    printf("0 bookID: borrow book (Status=1), 1 bookID: return book (Status=0)\n");
    
    while (printf("\n입력 (Option BookID): "), scanf("%d %d", &option, &book_id) == 2) {
        
        long offset = (long)(book_id - START_ID) * RECORD_SIZE;
        fseek(fp, offset, SEEK_SET); // 해당 레코드 시작 위치로 포인터 이동

        if (fread(&record, RECORD_SIZE, 1, fp) > 0) {
            
            // 유효성 검사 (ID가 0이 아닌 유효한 레코드인지 확인)
            if (record.id == 0) {
                 printf("No record found for book ID %d (Invalid ID).\n", book_id);
                 continue;
            }
            
            // [대출/반납 로직]
            if (option == 0) { /* Borrow Logic */
                if (record.borrow_status == 1) { 
                    printf("You cannot borrow book. It is already checked out.\n"); 
                } else { 
                    record.numofborrow++; 
                    record.borrow_status = 1; 
                    printf("You've borrowed book.\n"); 
                }
            } else if (option == 1) { /* Return Logic */
                if (record.borrow_status == 0) { 
                    printf("You cannot return book. It is not currently checked out.\n"); 
                } else { 
                    record.borrow_status = 0; 
                    printf("You've returned book.\n"); 
                }
            } else {
                 fprintf(stderr, "Invalid option. Use 0 (Borrow) or 1 (Return).\n");
                 continue;
            }

            // --- 누락된 수정 및 출력 부분 추가 ---
            
            // 1. 포인터를 레코드 시작 위치로 되돌리기 (수정된 레코드를 덮어쓰기 위해)
            fseek(fp, (long)-RECORD_SIZE, SEEK_CUR); 
            
            // 2. 레코드 파일에 쓰기 (수정된 레코드를 덮어씀)
            if (fwrite(&record, RECORD_SIZE, 1, fp) != 1) {
                fprintf(stderr, "Error writing record to file.\n");
            }
            
            // 3. 변경된 레코드 내용 출력 (정렬된 형식)
            printf("%-5s %-20s %-15s %-6s %-12s %-6s\n", 
                   "id", "bookname", "author", "year", "numofborrow", "borrow");
            
            printf("%-5d %-20s %-15s %-6d %-12d %-6s\n", 
                   record.id, record.bookname, record.author, record.year, record.numofborrow, 
                   (record.borrow_status == 1 ? "True" : "False")); 

        } else {
            // 레코드 검색 실패
            printf("No record found for book ID %d.\n", book_id);
        }
    } // while 루프 종료

    fclose(fp);
    exit(0);
}
