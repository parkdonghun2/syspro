// book.h 파일 내용

#define MAX_TITLE 20
#define MAX_AUTHOR 20
#define START_ID 1

// 책 레코드 구조체
struct book {
    int id;                   
    char bookname[MAX_TITLE]; 
    char author[MAX_AUTHOR];  
    int year;                 
    int numofborrow;          
    int borrow_status;       // 정수(int)로 복원. (0: False, 1: True)
};

// 레코드 파일에 저장할 레코드의 총 크기
#define RECORD_SIZE sizeof(struct book)
