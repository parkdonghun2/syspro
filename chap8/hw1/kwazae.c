#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

extern char **environ;   // 환경변수 전체에 접근하기 위한 전역변수

void print_usage(const char *prog)
{
    fprintf(stderr,
            "사용법:\n"
            "  %s -e [환경변수이름]\n"
            "  %s -u\n"
            "  %s -g\n"
            "  %s -i\n"
            "  %s -p\n",
            prog, prog, prog, prog, prog);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    /* ------------ -e : 환경변수 출력 ------------ */
    if (strcmp(argv[1], "-e") == 0) {
        if (argc == 2) {
            // 이름이 없으면 모든 환경변수 출력
            char **p;
            for (p = environ; *p != NULL; p++) {
                printf("%s\n", *p);
            }
        } else if (argc == 3) {
            // 이름이 있으면 해당 환경변수만 출력
            const char *name = argv[2];
            char *value = getenv(name);

            if (value != NULL)
                printf("%s = %s\n", name, value);
            else
                printf("%s : 그런 환경변수가 없습니다.\n", name);
        } else {
            print_usage(argv[0]);
            return 1;
        }
    }
    /* ------------ -u : 실제/유효 사용자 ID ------------ */
    else if (strcmp(argv[1], "-u") == 0) {
        uid_t ruid = getuid();
        uid_t euid = geteuid();
        struct passwd *rpw = getpwuid(ruid);
        struct passwd *epw = getpwuid(euid);

        printf("My Realistic User ID  : %d(%s)\n",
               (int)ruid, rpw ? rpw->pw_name : "unknown");
        printf("My Valid User ID      : %d(%s)\n",
               (int)euid, epw ? epw->pw_name : "unknown");
    }
    /* ------------ -g : 실제/유효 그룹 ID ------------ */
    else if (strcmp(argv[1], "-g") == 0) {
        gid_t rgid = getgid();
        gid_t egid = getegid();
        struct group *rg = getgrgid(rgid);
        struct group *eg = getgrgid(egid);

        printf("My Realistic Group ID : %d(%s)\n",
               (int)rgid, rg ? rg->gr_name : "unknown");
        printf("My Valid Group ID     : %d(%s)\n",
               (int)egid, eg ? eg->gr_name : "unknown");
    }
    /* ------------ -i : 프로세스 ID ------------ */
    else if (strcmp(argv[1], "-i") == 0) {
        printf("my process number : [%d]\n", (int)getpid());
    }
    /* ------------ -p : 부모 프로세스 ID ------------ */
    else if (strcmp(argv[1], "-p") == 0) {
        printf("my parent's process number : [%d]\n", (int)getppid());
    }
    /* ------------ 알 수 없는 옵션 ------------ */
    else {
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}

