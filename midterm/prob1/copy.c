#include <stdio.h>
#include <stdlib.h> // exit(), atoi()를 위해 필요
#include <ctype.h>  // toupper(), tolower()를 위해 필요

// 파일 복사 및 변환 함수
void copy_and_transform(FILE *fp_in, FILE *fp_out, int option);

int main(int argc, char *argv[])
{
    FILE *fp_in, *fp_out;
    int option;

    // 1. 인자 개수 확인: 총 4개 필요 (./copy, option, source, dest)
    if (argc != 4) {
        fprintf(stderr, "How to use: %s <option: 0, 1, 2> <source> <destination>\n", argv[0]);
        exit(1);
    }
    
    // 2. 옵션 값(argv[1])을 정수형으로 변환
    option = atoi(argv[1]); 

    // 3. 옵션 유효성 검사
    if (option < 0 || option > 2) {
        fprintf(stderr, "Error: Invalid option %d. Use 0, 1, or 2.\n", option);
        exit(1);
    }

    // 4. 원본 파일 열기 (읽기 모드: "r")
    if ((fp_in = fopen(argv[2], "r")) == NULL) { 
        perror(argv[2]); // <조건>: 복사 중간 문제가 발생하면 오류 메시지 출력
        exit(2);
    }
    
    // 5. 복사본 파일 열기 (쓰기 모드: "w")
    if ((fp_out = fopen(argv[3], "w")) == NULL) { 
        perror(argv[3]); // <조건>: 복사 중간 문제가 발생하면 오류 메시지 출력
        fclose(fp_in); 
        exit(3);
    }
    
    // 6. 파일 복사 및 변환 함수 호출
    copy_and_transform(fp_in, fp_out, option);

    // 7. 파일 닫기
    fclose(fp_in);
    fclose(fp_out);
    
    printf("\nFile copy and transformation successful (Option %d).\n", option);

    return 0; 
}

// 파일 복사 및 변환 로직 함수
void copy_and_transform(FILE *fp_in, FILE *fp_out, int option)
{
    int c; // EOF 처리를 위해 char가 아닌 int 사용
    
    printf("--- Copied Content Output ---\n"); // <조건>: 복사된 파일의 내용을 화면에 출력
    
    while ((c = getc(fp_in)) != EOF) {
        int output_char = c;
        
        switch (option) {
            case 1: 
                // 옵션 1: 모든 대문자를 소문자로 변환하여 복사본 작성
                output_char = tolower(c);
                break;
            case 2: 
                // 옵션 2: 모든 소문자를 대문자로 변환하여 복사본 작성
                output_char = toupper(c);
                break;
            case 0:
            default:
                // 옵션 0: 원본과 동일하게 복사본 작성 (변환 없음)
                break;
        }

        // 1. 복사본 파일에 쓰기
        if (putc(output_char, fp_out) == EOF) {
            fprintf(stderr, "Error writing to destination file.\n"); // <조건>: 쓰기 오류 시 프로그램 종료
            exit(4); 
        }

        // 2. 화면에 출력 (복사된 내용 출력 조건)
        putc(output_char, stdout); 
    }
    printf("\n-------------------------------\n");
}
