#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256
#define MAXARG  32

int main(void)
{
    char line[MAXLINE];
    char *args[MAXARG];

    while (1) {
        printf("Pls input cmd : ");
        fflush(stdout);

        // 입력 끝(CTRL+D 등)이면 종료
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }

        // 개행 제거
        line[strcspn(line, "\n")] = '\0';

        // 빈 줄이면 다시
        if (line[0] == '\0')
            continue;

        // Exit 입력 시 쉘 종료
        if (strcmp(line, "Exit") == 0 || strcmp(line, "exit") == 0)
            break;

        int background = 0;     // & 여부
        int argc = 0;

        // strtok_r 로 토큰 분리 (공백 기준)
        char *saveptr;
        char *token = strtok_r(line, " \t", &saveptr);

        while (token != NULL && argc < MAXARG - 1) {
            if (strcmp(token, "&") == 0) {
                // & 나오면 백그라운드 실행 표시 후 종료
                background = 1;
                break;
            }
            args[argc++] = token;
            token = strtok_r(NULL, " \t", &saveptr);
        }
        args[argc] = NULL;

        if (argc == 0)
            continue;

        printf("[%d] Parent process start\n", getpid());

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            continue;
        }
        else if (pid == 0) {
            // ----- child -----
            printf("[%d] child process start\n", getpid());

            execvp(args[0], args);    // 명령 실행

            // execvp 가 돌아왔다는 건 실패했다는 뜻
            perror("execvp");
            exit(1);
        }
        else {
            // ----- parent -----
            if (!background) {
                int status;
                pid_t child = wait(&status);
                printf("[%d] child process end %d\n", getpid(), child);
                printf("SUCCESS\n");
            } else {
                // 백그라운드인 경우 기다리지 않고 바로 프롬프트로
                printf("[%d] child process (background) pid = %d\n",
                       getpid(), pid);
            }
        }
    }

    printf("Parent process end\n");
    return 0;
}

