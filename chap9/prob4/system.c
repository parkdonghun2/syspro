#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // fork(), exec(), waitpid() 등을 위해 필요
#include <sys/wait.h>   // waitpid()를 위해 필요
#include <errno.h>      // errno를 위해 필요

// system() 함수와 동일하게 작동하는 mysystem() 함수 구현
int mysystem(const char *cmdstring) {
    int pid, status;

    // 1. 명령어(cmdstring)가 NULL일 경우 처리
    if (cmdstring == NULL) { 
        return 1; // 0이 아닌 값 반환 (명령어 실행 가능성 있음)
    }

    // 2. 프로세스 생성 (fork)
    if ((pid = fork()) < 0) { // 프로세스 생성 실패
        status = -1;
    } 
    
    else if (pid == 0) {
        // 3. 자식 프로세스 경로: 셸(/bin/sh)을 통해 명령어 실행
        // exec("sh", "-c", "명령어", NULL) 구조로 명령어 실행
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        
        // exec 실행에 실패했을 경우:
        _exit(127); // 명령어 실행 오류 코드를 반환하며 종료
    }
    
    else {
        // 4. 부모 프로세스 경로: 자식의 종료를 대기 (waitpid)
        do {
            // waitpid를 사용하여 자식 프로세스의 종료를 기다림
            if (waitpid(pid, &status, 0) == -1) {
                if (errno != EINTR) { // EINTR 오류(시그널 중단)가 아니면
                    return -1; // waitpid로부터의 오류
                }
            } else {
                return status; // 자식의 종료 상태(status)를 반환
            }
        } while (1);
    }
    
    return status;
}

// 28번 슬라이드의 syscall.c를 mysystem()을 사용하도록 재현
int main() {
    int status;
    int exit_code;

    printf("--- system(\"date\") ---\n");
    if ((status = mysystem("date")) < 0) {
        perror("mysystem(\"date\") error");
    }
    // WEXITSTATUS 매크로가 없으므로, 자식의 종료 코드를 추출하는 비트 연산을 사용합니다.
    exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    printf("종료코드 %d\n", exit_code);

    printf("\n--- system(\"hello\") [실패 예상] ---\n");
    if ((status = mysystem("hello")) < 0) {
        perror("mysystem(\"hello\") error");
    }
    exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    printf("종료코드 %d (127 예상)\n", exit_code);

    printf("\n--- system(\"who; exit 44\") ---\n");
    if ((status = mysystem("who; exit 44")) < 0) {
        perror("mysystem(\"who; exit 44\") error");
    }
    exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    printf("종료코드 %d (44 예상)\n", exit_code);

    return 0;
}
