// sortedbookquery.c 파일 내용

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h" 

// 비교 함수: 대출 횟수 (numofborrow)를 기준으로 내림차순 정렬
int compare_numofborrow(const void *a, const void *b) {
    struct book *rec_a = (struct book *)a;
    struct book *rec_b = (struct book *)b;
    // 내림차순: b.numofborrow - a.numofborrow
    return rec_b->numofborrow - rec_a->numofborrow; 
}

void print_records(struct book *records, int count, int option);

int main(int argc, char *argv[])
{
    FILE *fp;
    struct book *records;
    int option, num_records = 0, current_size = 10;
    
    if (argc != 3 || (option = atoi(argv[1])) < 0 || option > 1) {
        fprintf(stderr, "How to use: %s <option: 0/1> <data_file.dat>\n", argv[0]);
        exit(1);
    }

    if ((fp = fopen(argv[2], "rb")) == NULL) { 
        perror(argv[2]);
        exit(2);
    }

    // 1. 파일에서 모든 레코드를 메모리로 읽기 (동적 할당 및 재할당 포함)
    records = (struct book *)malloc(current_size * RECORD_SIZE);
    if (records == NULL) { perror("malloc error"); exit(3); }

    while (fread(&records[num_records], RECORD_SIZE, 1, fp) > 0) {
        if (records[num_records].id != 0) {
            num_records++;
        }
        if (num_records >= current_size) {
            current_size *= 2;
            records = (struct book *)realloc(records, current_size * RECORD_SIZE);
            if (records == NULL) { perror("realloc error"); exit(4); }
        }
    }
    fclose(fp);

    // 2. qsort를 이용한 정렬 (대출 횟수 내림차순)
	 qsort(records, num_records, RECORD_SIZE, compare_numofborrow);

    // 3. 출력
    print_records(records, num_records, option);

    free(records); 
    return 0;
}

// 출력 함수 (정렬된 배열을 옵션에 따라 출력)
void print_records(struct book *records, int count, int option) {
    printf("\nBook Query Result (Option %d) - Sorted by Borrow Count:\n", option);
    printf("%-5s %-20s %-15s %-6s %-12s %-6s\n", 
           "id", "bookname", "author", "year", "numofborrow", "borrow");
    printf("----------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        struct book rec = records[i];
        int is_available = (rec.borrow_status == 0); 
        const char *status_str = is_available ? "False" : "True";

        // 옵션 0 또는 (옵션 1이면서 대출 가능할 때) 출력
        if (option == 0 || (option == 1 && is_available)) {
            printf("%-5d %-20s %-15s %-6d %-12d %-6s\n", 
                   rec.id, rec.bookname, rec.author, rec.year, rec.numofborrow, status_str);
        }
    }
    printf("----------------------------------------------------------------------\n");
}
