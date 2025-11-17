// list_sort.c 파일 내용

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h" 

// --- 연결 리스트 관련 함수 선언 ---
struct BookNode *insert_sorted(struct BookNode *head, struct book new_record, int sort_by_year);
void free_list(struct BookNode *head);

int main(int argc, char *argv[])
{
    FILE *fp;
    struct book record;
    struct BookNode *head = NULL; // 리스트의 시작 (헤드 포인터)
    int sort_mode = 0; // 0: 횟수, 1: 연도 (예시)

    if (argc != 2) {
        fprintf(stderr, "How to use: %s <data_file.dat>\n", argv[0]);
        exit(1);
    }
    
    // 1. 파일 열기 (읽기 모드: "rb")
    if ((fp = fopen(argv[1], "rb")) == NULL) { 
        perror(argv[1]);
        exit(2);
    }

    // 2. 파일에서 모든 레코드를 읽어 정렬된 리스트에 삽입
    while (fread(&record, RECORD_SIZE, 1, fp) > 0) {
        if (record.id != 0) {
            // [정렬 모드 설정]: 여기서는 연도 내림차순을 위해 1로 가정합니다.
            // book.h의 요구사항에 맞추어 연도(year) 기준 내림차순 정렬
            head = insert_sorted(head, record, 1); 
        }
    }
    fclose(fp);

    // 3. 정렬된 결과를 파일에 저장 (문제 3-(3) 해결)
    if ((fp = fopen(argv[1], "wb")) == NULL) { // 덮어쓰기 위해 "wb"로 다시 엽니다.
        perror(argv[1]);
        free_list(head);
        exit(3);
    }
    
    struct BookNode *current = head;
    while (current != NULL) {
        // 정렬된 리스트 순서대로 파일에 덮어쓰기
        fwrite(&current->record, RECORD_SIZE, 1, fp); 
        current = current->next;
    }
    fclose(fp);
    
    // 4. 정렬된 결과를 출력 (문제 3-(1) 출력 확인 해결)
    printf("--- Sorted List Output (Year Descending) ---\n");
    // (출력 함수는 별도로 정의하여 사용하거나, 위에서 저장한 후 bookquery로 확인)
    
    free_list(head); // 메모리 해제
    return 0;
}

// --- 연결 리스트 핵심 함수 구현 ---

// 정렬된 위치에 노드를 삽입하는 함수
struct BookNode *insert_sorted(struct BookNode *head, struct book new_record, int sort_by_year) {
    struct BookNode *new_node = (struct BookNode *)malloc(sizeof(struct BookNode));
    if (new_node == NULL) { perror("Node malloc error"); exit(5); }
    
    new_node->record = new_record;
    new_node->next = NULL;

    // 1. 리스트가 비었거나, 새 노드가 헤드보다 앞에 와야 할 경우
    // sort_by_year=1: 연도 내림차순 (new_record.year가 head.record.year보다 클 때)
    if (head == NULL || 
        (sort_by_year && new_record.year > head->record.year) ||
        (!sort_by_year && new_record.numofborrow > head->record.numofborrow)) 
    {
        new_node->next = head;
        return new_node; // 새 노드가 헤드가 됨
    }

    // 2. 리스트 중간에 삽입할 위치 찾기
    struct BookNode *current = head;
    while (current->next != NULL && 
           ((sort_by_year && current->next->record.year >= new_record.year) ||
            (!sort_by_year && current->next->record.numofborrow >= new_record.numofborrow))) 
    {
        current = current->next;
    }
    
    new_node->next = current->next;
    current->next = new_node;
    return head; // 헤드 유지
}

// 연결 리스트 메모리 해제 함수
void free_list(struct BookNode *head) {
    struct BookNode *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}
