// sortedbookcreate.c 파일 내용 (연결 리스트 구현)

#include <stdio.h>
#include <stdlib.h>
#include "book.h" 

// --- 함수 선언 ---
// 정렬된 위치에 노드를 삽입하는 함수 (연도 내림차순 기준)
struct BookNode *insert_sorted_by_year(struct BookNode *head, struct book new_record);
void free_list(struct BookNode *head);

int main(int argc, char *argv[])
{
    FILE *fp;
    struct book record;
    struct BookNode *head = NULL; 
    
    if (argc != 2) {
        fprintf(stderr, "How to use: %s <data_file.dat>\n", argv[0]);
        exit(1);
    }

    // 1. 파일 열기 (읽기/쓰기 모드: "rb+")
    if ((fp = fopen(argv[1], "rb+")) == NULL) { 
        perror(argv[1]);
        exit(2);
    }

    // 2. 파일에서 모든 레코드를 읽어 정렬된 리스트에 삽입
    while (fread(&record, RECORD_SIZE, 1, fp) > 0) {
        if (record.id != 0) {
            // 연도 내림차순으로 정렬하여 삽입
            head = insert_sorted_by_year(head, record); 
        }
    }
    
    // 3. 파일 포인터를 시작으로 이동 (덮어쓰기 준비)
    rewind(fp); 
    
    // 4. 정렬된 리스트를 파일에 덮어쓰기 저장
    struct BookNode *current = head;
    while (current != NULL) {
        fwrite(&current->record, RECORD_SIZE, 1, fp); 
        current = current->next;
    }
    
    fclose(fp);
    free_list(head); 
    printf("File successfully sorted by year (descending) and saved.\n");
    return 0;
}

// --- 연결 리스트 핵심 함수 구현 ---

// 연도 내림차순 정렬 삽입 로직
struct BookNode *insert_sorted_by_year(struct BookNode *head, struct book new_record) {
    struct BookNode *new_node = (struct BookNode *)malloc(sizeof(struct BookNode));
    if (new_node == NULL) { perror("Node malloc error"); exit(5); }
    
    new_node->record = new_record;
    new_node->next = NULL;

    // 1. 리스트 시작에 삽입 (새 노드가 헤드보다 연도가 클 때)
    if (head == NULL || new_record.year > head->record.year) {
        new_node->next = head;
        return new_node; 
    }

    // 2. 리스트 중간에 삽입
    struct BookNode *current = head;
    while (current->next != NULL && new_record.year <= current->next->record.year) {
        current = current->next;
    }
    
    new_node->next = current->next;
    current->next = new_node;
    return head;
}

// 메모리 해제 함수
void free_list(struct BookNode *head) {
    struct BookNode *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}
