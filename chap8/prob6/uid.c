#include <stdio.h>
#include <unistd.h>  // Required for getuid(), geteuid(), getgid(), getegid()
#include <sys/types.h> // Sometimes needed for getuid/gid types
#include <pwd.h>     // Required for getpwuid()
#include <grp.h>     // Required for getgrgid()

/* Outputs the User IDs and Group IDs. */
int main()
{
    // Real User ID (RUID)
    printf("My Real User ID: %d(%s)\n", 
           getuid(), 
           getpwuid(getuid())->pw_name);

    // Effective User ID (EUID)
    printf("My Effective User ID: %d(%s)\n", 
           geteuid(), 
           getpwuid(geteuid())->pw_name);

    // Real Group ID (RGID)
    printf("My Real Group ID: %d(%s)\n", 
           getgid(), 
           getgrgid(getgid())->gr_name);

    // Effective Group ID (EGID)
    printf("My Effective Group ID: %d(%s)\n", 
           getegid(), 
           getgrgid(getegid())->gr_name);
           
    return 0;
}
