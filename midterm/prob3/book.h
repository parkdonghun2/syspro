// book.h 파일 내용

#define MAX_TITLE 20
#define MAX_AUTHOR 20
#define START_ID 1

// 책 레코드 구조체
struct book {
    int id;                   // 책 고유 번호
    char bookname[MAX_TITLE]; // 책 이름
    char author[MAX_AUTHOR];  // 저자 이름
    int year;                 // 출판 년도
    int numofborrow;          // 대출 횟수
    int borrow_status;        // 대출 유무 (0: False, 1: True)
};

// 레코드 파일에 저장할 레코드의 총 크기
#define RECORD_SIZE sizeof(struct book)

struct BookNode {
	struct book record;
	struct BookNode *next;
};
