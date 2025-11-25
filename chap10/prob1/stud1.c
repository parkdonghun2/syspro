#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // perror 사용을 위해 추가

struct student {
    int id;
    char name[20];
};

int main()
{
    struct student *ptr; // struct studnet -> struct student 로 수정
    int n;
    int i;

    printf("How many students will you enter? ");
    scanf("%d", &n);

    if (n <= 0) {
        // 한글 메시지를 영문으로 수정
        fprintf(stderr, "Error: Student count entered incorrectly.\n");
        fprintf(stderr, "Program terminating.\n");
        exit(1);
    }
    
    // malloc 호출
    ptr = (struct student *) malloc(n * sizeof(struct student));
    
    if (ptr == NULL) {
        perror("malloc");
        exit(2);
    }

    // 입력 프롬프트 수정
    printf("Please enter the student ID and name for %d students.\n", n); 
    
    for (i = 0; i < n; i++) {
        // [주의] scanf("%d %s")는 공백을 구분자로 사용하며,
        // name 필드에 '\n'이 남지 않도록 주의해야 합니다.
        scanf("%d %s", &ptr[i].id, ptr[i].name);
    }

    // 출력
    printf("\nStudent Information (Reverse Order)\n");
    
    for (i = n - 1; i >= 0; i--) {
        printf("%d %s\n", ptr[i].id, ptr[i].name);
    }

    printf("\n");
    exit(0);
}
