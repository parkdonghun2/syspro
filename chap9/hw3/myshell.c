#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 128

// 앞뒤 공백 제거
char *trim(char *s)
{
    char *end;

    // 앞쪽 공백 스킵
    while (*s == ' ' || *s == '\t' || *s == '\n')
        s++;

    if (*s == 0)  // 전부 공백
        return s;

    // 뒤쪽 공백 제거
    end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t' || *end == '\n'))
        end--;

    *(end + 1) = '\0';
    return s;
}

// 한 개의 명령(세미콜론으로 나눈 조각)을 실행
void run_command(char *cmd)
{
    char *argv[MAX_ARGS];
    int argc = 0;
    int background = 0;         // & 여부
    char *infile = NULL;        // < infile
    char *outfile = NULL;       // > outfile

    // 공백 정리
    cmd = trim(cmd);
    if (*cmd == '\0')
        return;

    // 맨 끝에 & 있는지 확인 (백그라운드)
    int len = strlen(cmd);
    for (int i = len - 1; i >= 0; i--) {
        if (cmd[i] == ' ' || cmd[i] == '\t')
            continue;
        if (cmd[i] == '&') {
            background = 1;
            cmd[i] = '\0';      // & 제거
        }
        break;
    }
    cmd = trim(cmd);
    if (*cmd == '\0')
        return;

    // 토큰 분해 (공백 기준)
    char *token = strtok(cmd, " \t");
    while (token != NULL && argc < MAX_ARGS - 1) {
        if (strcmp(token, "<") == 0) {
            // 입력 리다이렉션
            token = strtok(NULL, " \t");
            if (token != NULL)
                infile = token;
        } else if (strcmp(token, ">") == 0) {
            // 출력 리다이렉션
            token = strtok(NULL, " \t");
            if (token != NULL)
                outfile = token;
        } else {
            // 일반 인자
            argv[argc++] = token;
        }
        token = strtok(NULL, " \t");
    }
    argv[argc] = NULL;

    if (argc == 0)
        return;

    // exit 명령은 쉘 종료
    if (strcmp(argv[0], "exit") == 0)
        exit(0);

    // fork 후 exec
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // --- child process ---

        // 입력 리다이렉션
        if (infile != NULL) {
            int fd = open(infile, O_RDONLY);
            if (fd < 0) {
                perror(infile);
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // 출력 리다이렉션
        if (outfile != NULL) {
            int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror(outfile);
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(argv[0], argv);
        // exec 실패 시
        perror(argv[0]);
        exit(1);

    } else {
        // --- parent process ---
        if (!background) {
            // 포그라운드: 자식이 끝날 때까지 기다림
            waitpid(pid, NULL, 0);
        } else {
            // 백그라운드: 바로 프롬프트로 돌아감
            printf("[background pid %d]\n", pid);
        }
    }
}

int main(void)
{
    char line[MAX_LINE];

    while (1) {
        printf("[shell] ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            // Ctrl+D 등으로 EOF 들어오면 종료
            printf("\n");
            break;
        }

        // 세미콜론(;)으로 여러 명령 나누기
        char *cmd = strtok(line, ";");
        while (cmd != NULL) {
            run_command(cmd);
            cmd = strtok(NULL, ";");
        }
    }

    return 0;
}

